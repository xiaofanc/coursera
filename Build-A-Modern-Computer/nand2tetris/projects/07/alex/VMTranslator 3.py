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
    "or": "M=M|D",
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
    "static": 16,   # R16-255
}
CONSTANT = "constant"

class VMTranslator:
    def __init__(self, vm_path):
        self.vm_path = vm_path
        self.vm_file_name = vm_path.split("/")[-1].rstrip(".vm")
        self.vm = [line.strip() for line in open(vm_path)]
        self.asm_path = os.path.splitext(vm_path)[0] + ".asm"
        self.asm = []
        self.jmp_count = 0

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
                else:
                    raise ValueError(f"line {i}: {line}")
            except Exception as e:
                raise ValueError(f"line {i}: {line}, msg: {e}")

    def parse_stack_ops(self, cmd, segment, index):
        idx = int(index)
        if cmd == PUSH:
            self.set_address_to_A(segment, idx) # A = resolved address
            if segment == "constant":
                self.asm += ["D=A"]             # push constant 10  --> @10; D=10
            else:
                self.asm += ["D=M"]             # push argument 10  --> A=addr; D=RAM[A]
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
                "D=M-D",
                f"@True.{self.jmp_count}",      # @True.1
                ARITH_CMDS_REQUIRE_JUMP[cmd],   # D;JEQ
                "@SP", "A=M", "M=0",            # stack.push(false)
                f"@End.{self.jmp_count}",       # @End.1
                "0;JMP",                        # 0;JMP
                f"(True.{self.jmp_count})",     # (True1)
                "@SP", "A=M", "M=-1",           # stack.push(true)
                f"(End.{self.jmp_count})",      # (End.1)
            ]
            self.increment_SP()
            self.jmp_count += 1

    def set_A_to_stack(self): self.asm += ["@SP", "A=M"]
    def decrement_SP(self): self.asm += ["@SP", "M=M-1"]
    def increment_SP(self): self.asm += ["@SP", "M=M+1"]

    def save_asm_file(self):
        with open(self.asm_path, "w") as asm_file:
            for line in self.asm:
                asm_file.write(line+"\n")

    def run(self):
        self.remove_comments()
        self.parse()
        self.save_asm_file()

if __name__ == "__main__":
    trans = VMTranslator(sys.argv[1])
    trans.run()