"""
Translate vm to asm file
run python3 VMTranslator.py /Users/XiaofangChu/Documents/code/coursera/Build-A-Modern-Computer/nand2tetris/projects/08/ProgramFlow/BasicLoop/BasicLoop.vm

Focus on VM language:
# project08
Branching commands:
goto 
if-goto 
label

Function commands:
call 
function
return

# already implemented in project07
Arithmetic / Logic commands:
add
sub
neg
eq
gt
lt
and
or
not

Memory access commands:
pop segment i
push segment i

Standard Mapping:
SP:  RAM[0]
LCL: RAM[1]
ARG: RAM[2]
THIS: RAM[3] 
THAT: RAM[4]
temp segment: RAM[5] - RAM[12]
general purpose register: RAM[13] - RAM[15]
static variables: RAM[16] - RAM[255]
stack: RAM[256] - RAM[2047]
Pointer: RAM[3] - RAM[4]

"""


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

# project 8
# branching
LABEL = "label"
GOTO = "goto"
IFGOTO = "if-goto"

# function
CALL = "call"
FUNC = "function"
RTN = "return"


class VMTranslator:
    def __init__(self, vm_path):
        self.vm_path = vm_path
        self.vm_file_name = vm_path.split("/")[-1].rstrip(".vm")  # get vm file name
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
            self.asm.append(f"// {line}")  # write original command to comments
            cmd, *args = line.split()    # i.e. cmd = pop, args = [local, 2]
            num_args = len(args)
            try:
                if   cmd in ARITH_CMDS and num_args == 0:  # i.e. add
                    self.parse_arith_ops(cmd)
                elif cmd in STACK_CMDS and num_args == 2:  # i.e. push, pop
                    self.parse_stack_ops(cmd, *args)
                elif cmd == LABEL and num_args == 1:       # i.e. label LOOP_START
                    self.parse_label_ops(*args)
                elif cmd == GOTO and num_args == 1:        # i.e. goto END_PROGRAM 
                    self.parse_goto_ops(*args)
                elif cmd == IFGOTO and num_args == 1:      # i.e. if-goto COMPUTE_ELEMENT
                    self.parse_ifgoto_ops(*args)
                elif cmd == FUNC and num_args == 2:
                    self.parse_function_ops()
                elif cmd == CALL and num_args == 2:
                    self.parse_call_ops()
                elif cmd == RTN and num_args == 0:
                    self.parse_return_ops()
                else:
                    raise ValueError(f"line {i}: {line}")
            except Exception as e:
                print(line)
                #raise ValueError(f"line {i}: {line}, msg: {e}")

    # project08 - branching
    def parse_label_ops(self, label):
        self.asm.append(f"({self.vm_file_name}.{label})")

    def parse_goto_ops(self, label):
        self.asm.extend([f"@({self.vm_file_name}.{label})", "0;JMP"]) # unconditional jump
   
    def parse_ifgoto_ops(self, label):
        self.pop_stack_to_D()                                         # pop the result of condition
        self.asm.extend([f"@{self.vm_file_name}.{label}", "D;JNE"])   # jump to that @address when D not equal to 0 (condition is met)

    # project08 - function
    def parse_function_ops(self, func_name, local_num):
        pass
    def parse_call_ops(self, func_name, arg_num):
        pass
    def parse_return_ops(self):
        pass

    # project07
    def parse_stack_ops(self, cmd, segment, index):
        idx = int(index)
        if cmd == PUSH:
            self.set_address_to_A(segment, idx) # A = resolved address
            if segment == "constant":
                self.asm.extend(["D=A"])             # push constant 10  --> @10; D=10
            else:
                self.asm.extend(["D=M"])             # push argument 10  --> A=addr; D=RAM[A]
            self.push_D_to_stack()              # stack.push(D)
        elif cmd == POP:
            self.set_address_to_A(segment, idx) # A = resolved address
            self.asm.extend(["D=A"])                 # D = A
            self.asm.extend(["@R13", "M=D"])         # *R13 = D
            self.pop_stack_to_D()               # D = stack.pop()
            self.asm.extend(["@R13", "A=M", "M=D"])  # RAM[*R13] = RAM[address] = D

    def set_address_to_A(self, segment, index):
        # A = resolved address
        if segment in PREDEFINED_SEGMENTS:
            address_sym = PREDEFINED_SEGMENTS[segment]
            self.asm.extend([f"@{address_sym}", "D=M", f"@{index}", "A=D+A"])
        elif segment in OTHER_SEGMENTS:
            address = OTHER_SEGMENTS[segment]
            self.asm.extend([f"@R{address+index}"])
        elif segment == CONSTANT:
            self.asm.extend([f"@{index}"])

    def push_D_to_stack(self):
        # stack.push(D); write D to stack top and incr SP
        self.asm.extend(["@SP", "A=M", "M=D", "@SP", "M=M+1"])

    def pop_stack_to_D(self):
        # D = stack.pop(); decr SP and write D to stack top
        self.asm.extend(["@SP", "M=M-1", "A=M", "D=M"])

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
            self.asm.extend([
                "D=M-D",
                f"@True.{self.jmp_count}",      # @True.1
                ARITH_CMDS_REQUIRE_JUMP[cmd],   # D;JEQ
                "@SP", "A=M", "M=0",            # stack.push(false)
                f"@End.{self.jmp_count}",       # @End.1
                "0;JMP",                        # 0;JMP
                f"(True.{self.jmp_count})",     # (True1)
                "@SP", "A=M", "M=-1",           # stack.push(true)
                f"(End.{self.jmp_count})",      # (End.1)
            ])
            self.increment_SP()
            self.jmp_count += 1

    def set_A_to_stack(self): self.asm.extend(["@SP", "A=M"])
    def decrement_SP(self): self.asm.extend(["@SP", "M=M-1"])
    def increment_SP(self): self.asm.extend(["@SP", "M=M+1"])

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