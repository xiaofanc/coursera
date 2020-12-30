import sys
import os

COMMENT = "//"
PUSH, POP = "push", "pop"
STACK_CMDS = {PUSH, POP} # Memory Access Command
ARITH_CMDS = { # Arithmetic & Logical Command 
    #cmd => num_operands
    "add":2,"sub":2,"neg":1,"eq":2,"gt":2,"lt":2,"and":2,"or":2,"not":1
}
ARITH_CMDS_NOJUMP = {
    "add": "M=M+D",
    "sub": "M=M-D",
    "and": "M=M&D",
    "neg": "M=-M",
    "not": "M=!M",
    "or":  "M=M|D",
}
ARITH_CMDS_REQUIRE_JUMP = {
    "eq":"D;JEQ",
    "gt":"D;JGT",
    "lt":"D;JLT",
}
PREDEFINED_SEGMENTS = {
    "local": "LCL",     # LCL   base address is stored in R1
    "argument": "ARG",  # ARG   base address is stored in R2
    "this": "THIS",     # THIS  base address is stored in R3
    "that": "THAT",     # THAT  base address is stored in R4
}
OTHER_SEGMENTS = {
    "pointer": 3,   # R3, R4, basically this and that
    "temp": 5,      # R5-12
                    # R13-15 are free
    #"static": 16,   # R16-255
}
CONSTANT = "constant"
STATIC = "static"
LABEL = "label"
GOTO = "goto"
IFGOTO = "if-goto"
FUNCTION = "function"
CALL = "call"
RETURN = "return"

class VMTranslator:
    def __init__(self, vm_path):
        self.vm_path = vm_path
        if os.path.isdir(vm_path):
            self.is_multi_file = True
            dirpath, dirnames, filenames = next(os.walk(self.vm_path), [[],[],[]])
            self.vm_file_paths = [os.path.join(dirpath, f) for f in filenames if f.endswith(".vm")]
            self.asm_path = os.path.join(vm_path, vm_path.split("/")[-1]+".asm")
        else:
            self.is_multi_file = False
            self.vm_file_paths = [vm_path]
            self.asm_path = os.path.splitext(vm_path)[0] + ".asm"
        self.asm = []
        self.jump_count = 0
        self.call_count = 0

    def remove_comments(self):
        def clean(line):
            if COMMENT in line:
                return line.split(COMMENT)[0].strip()
            else:
                return line
        self.vm = [clean(line) for line in self.vm]

    def parse(self):
        for i, line in enumerate(self.vm):
            if not line: continue
            self.asm.append(f"// {line}")
            cmd, *args = line.split()
            num_args = len(args)
            try:
                if   cmd in ARITH_CMDS and num_args == 0:
                    self.parse_arith_ops(cmd)
                elif cmd in STACK_CMDS and num_args == 2:
                    self.parse_stack_ops(cmd, *args)
                elif cmd == LABEL and num_args == 1:
                    self.parse_label(args[0])
                elif cmd == GOTO and num_args == 1:
                    self.parse_goto(args[0])
                elif cmd == IFGOTO and num_args == 1:
                    self.parse_if_goto(args[0])
                elif cmd == FUNCTION and num_args == 2:
                    self.parse_function(args[0], int(args[1]))
                elif cmd == CALL and num_args == 2:
                    self.parse_call(args[0], int(args[1]))
                elif cmd == RETURN and num_args == 0:
                    self.parse_return()
                else:
                    raise ValueError(f"line {i}: {line}")
            except Exception as e:
                raise ValueError(f"line {i}: {line}, msg: {e}")

    def parse_label(self, label):
        self.asm.append(f"({self.vm_file_name}.{label})")

    def parse_goto(self, label):
        self.asm += [f"@{self.vm_file_name}.{label}", "0;JMP"]

    def parse_if_goto(self, label):
        self.pop_stack_to_D()
        self.asm += [f"@{self.vm_file_name}.{label}", "D;JNE"]

    def parse_function(self, func_name, nvars):
        self.asm.append(f"({func_name})")
        for _ in range(nvars):
            self.asm.append("D=0")
            self.push_D_to_stack()

    def parse_call(self, func_name, nargs):
        return_address = f"{func_name}-ret-{self.call_count}" # unique
        self.call_count += 1
        # push return_address to stack
        self.asm += ["@"+return_address, "D=A"]
        self.push_D_to_stack()
        # push frame to stack
        for address in ["@LCL", "@ARG", "@THIS", "@THAT"]:
            self.asm += [address, "D=M"]
            self.push_D_to_stack()
        # LCL = SP
        self.asm += ["@SP", "D=M", "@LCL", "M=D"]
        # ARG = SP-5-nargs
        # no need to @SP; D=M again
        self.asm += [f"@{5+nargs}", "D=D-A", "@ARG", "M=D"]
        # goto func_name, return_address
        self.asm += ["@"+func_name, "0;JMP", f"({return_address})"]

    def parse_return(self):
        # tmp vars
        ENDFRAME, RET = "R13", "R14"
        # ENDFRAME = LCL
        self.asm += ["@LCL", "D=M", "@"+ENDFRAME, "M=D"]
        # RET = *(ENDFRAME-5)           combine "D=D-A", "A=D"?
        self.asm += ["@"+ENDFRAME, "D=M", "@5", "D=D-A", "A=D", "D=M", "@"+RET, "M=D"]
        # *ARG = pop()
        self.pop_stack_to_D()
        self.asm += ["@ARG", "A=M", "M=D"]
        # SP = ARG+1
        self.asm += ["@ARG", "D=M", "@SP", "M=D+1"]
        # THAT = *(ENDFRAME-1)
        # THIS = *(ENDFRAME-2)
        # ARG  = *(ENDFRAME-3)
        # LCL  = *(ENDFRAME-4)
        for i, address in enumerate(["@THAT", "@THIS", "@ARG", "@LCL"], start=1):
            self.asm += ["@"+ENDFRAME, "D=M", f"@{i}", "D=D-A", "A=D", "D=M", address, "M=D"]
        # goto RET
        self.asm += ["@"+RET, "A=M", "0;JMP"]

    def parse_stack_ops(self, cmd, segment, index):
        idx = int(index)
        if cmd == PUSH:
            self.set_address_to_A(segment, idx) # A = resolved address
            if segment == "constant":
                self.asm += ["D=A"]             # push constant 10 --> @10;D=10
            else:
                self.asm += ["D=M"]             # push argument 10 --> A=addr;D=RAM[A]
            self.push_D_to_stack()              # stack.push(D)
        elif cmd == POP:
            self.set_address_to_A(segment, idx) # A = resolved address
            self.asm += ["D=A"]                 # D = A
            self.asm += ["@R13", "M=D"]         # *R13 = D
            self.pop_stack_to_D()               # D = stack.pop()
            self.asm += ["@R13", "A=M", "M=D"]  # RAM[*R13] = RAM[address] = D

    def set_address_to_A(self, segment, index):
        # A = resolved address
        if segment in PREDEFINED_SEGMENTS:
            address_sym = PREDEFINED_SEGMENTS[segment]
            self.asm += [f"@{address_sym}", "D=M", f"@{index}", "A=D+A"]
        elif segment in OTHER_SEGMENTS:
            address = OTHER_SEGMENTS[segment]
            self.asm += [f"@R{address+index}"]
        elif segment == STATIC:
            self.asm += [f"@{self.vm_file_name}-{index}"]
        elif segment == CONSTANT:
            self.asm += [f"@{index}"]

    def push_D_to_stack(self):
        # stack.push(D); write D to stack top and incr SP
        self.asm += ["@SP", "A=M", "M=D", "@SP", "M=M+1"]

    def pop_stack_to_D(self):
        # D = stack.pop(); decr SP and write D to stack top
        self.asm += ["@SP", "M=M-1", "A=M", "D=M"]

    def parse_arith_ops(self, cmd):
        if cmd not in ARITH_CMDS:
            raise ValueError("undefined cmd: {cmd}")
        num_operands = ARITH_CMDS[cmd]
        if num_operands == 1:
            self.decrement_SP()
            self.set_A_to_stack()
        elif num_operands == 2:
            self.pop_stack_to_D()
            self.decrement_SP()
            self.set_A_to_stack()

        if cmd in ARITH_CMDS_NOJUMP:
            self.asm.append(ARITH_CMDS_NOJUMP[cmd])
            self.increment_SP()
        elif cmd in ARITH_CMDS_REQUIRE_JUMP:
            self.asm += [
                "D=M-D",                        # current stack top - prev stack top
                f"@True.{self.jump_count}",     # @True.1
                ARITH_CMDS_REQUIRE_JUMP[cmd],   # D;JEQ
                "@SP", "A=M", "M=0",            # stack.push(false)
                f"@End.{self.jump_count}",      # @End.1
                "0;JMP",                        # 0;JMP
                f"(True.{self.jump_count})",    # (True.1)
                "@SP", "A=M", "M=-1",           # stack.push(true)
                f"(End.{self.jump_count})",     # (End.1)
            ]
            self.increment_SP()
            self.jump_count += 1

    def set_A_to_stack(self): self.asm += ["@SP", "A=M"]
    def decrement_SP(self): self.asm += ["@SP", "M=M-1"]
    def increment_SP(self): self.asm += ["@SP", "M=M+1"]

    def save_asm_file(self):
        with open(self.asm_path, "w") as asm_file:
            for line in self.asm:
                asm_file.write(line+"\n")

    def write_init(self):
        self.asm += ["//init", "@256", "D=A", "@SP", "M=D"]
        self.parse_call("Sys.init", 0)

    def run(self):
        if self.is_multi_file:
            self.write_init()
        for vm_file_path in self.vm_file_paths:
            self.vm_file_name = vm_file_path.split("/")[-1].rstrip(".vm")
            self.asm.append(f"\n// ==={self.vm_file_name}===\n")
            self.vm = [line.strip() for line in open(vm_file_path)]
            self.remove_comments()
            self.parse()
        self.save_asm_file()


if __name__ == "__main__":
    trans = VMTranslator(sys.argv[1])
    trans.run()
