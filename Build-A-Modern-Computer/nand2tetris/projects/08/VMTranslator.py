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
    # "static": 16,   # R16-255
}
CONSTANT = "constant"
STATIC = "static"

# project 8
# branching
LABEL = "label"
GOTO = "goto"
IFGOTO = "if-goto"

# function
CALL = "call"
FUNCTION = "function"
RTN = "return"


class VMTranslator:
    def __init__(self, vm_path):
        self.vm_path = vm_path             # ProgramFlow/BasicLoop
        if os.path.isdir(vm_path):
            self.is_multi_file = True       # a.vm, a.mp4,  b.fxf,  c.vm
            dirpath, dirnames, filenames = next(os.walk(self.vm_path), [[],[],[]])
            self.vm_file_paths = [os.path.join(dirpath, f) for f in filenames if f.endswith(".vm")]
            # print(vm_path.split("/")[-1])  # BasicLoop
            self.asm_path = os.path.join(vm_path, vm_path.split("/")[-1]+".asm")
        else:
            self.is_multi_file = False
            self.vm_file_paths = [vm_path]
            self.asm_path = os.path.splitext(vm_path)[0] + ".asm"
        self.asm = []
        self.jmp_count = 0
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
            self.asm.append(f"// {line}")  # write original command to comments
            cmd, *args = line.split()    # i.e. cmd = pop, args = [local, 2]
            num_args = len(args)
            try:
                if   cmd in ARITH_CMDS and num_args == 0:  # i.e. add
                    self.parse_arith_ops(cmd)
                elif cmd in STACK_CMDS and num_args == 2:  # i.e. push, pop
                    self.parse_stack_ops(cmd, *args)
                elif cmd == LABEL and num_args == 1:       # i.e. label LOOP_START
                    self.parse_label(*args)
                elif cmd == GOTO and num_args == 1:        # i.e. goto END_PROGRAM 
                    self.parse_goto(*args)
                elif cmd == IFGOTO and num_args == 1:      # i.e. if-goto COMPUTE_ELEMENT
                    self.parse_ifgoto(*args)
                elif cmd == FUNCTION and num_args == 2:
                    self.parse_function(*args)
                elif cmd == CALL and num_args == 2:
                    self.parse_call(*args)
                elif cmd == RTN and num_args == 0:
                    self.parse_return()
                else:
                    raise ValueError(f"line {i}: {line}")
            except Exception as e:
                print(line, e)
                #raise ValueError(f"line {i}: {line}, msg: {e}")

    # project08 - branching
    def parse_label(self, label):
        self.asm.append(f"({self.vm_file_name}.{label})")

    def parse_goto(self, label):
        self.asm.extend([f"@{self.vm_file_name}.{label}", "0;JMP"]) # unconditional jump
   
    def parse_ifgoto(self, label):
        self.pop_stack_to_D()                                         # pop the result of condition
        self.asm.extend([f"@{self.vm_file_name}.{label}", "D;JNE"])   # jump to that @address when D not equal to 0 (condition is met)

    # project08 - function
    def parse_function(self, func_name, local_num):
        """

        (func_name)
        repeat n(local_num) times:
        push 0 for local variables

        """
        self.asm.append(f"({func_name})")

        # push 0 - k times to create local variables
        for _ in range(int(local_num)):
            self.asm.append("D=0")
            self.push_D_to_stack()

    def parse_call(self, func_name, arg_num):
        """

        generate a label for return address and push it to stack
        push LCL
        push ARG
        push THIS
        push THAT
        ARG = SP-n-5
        LCL = SP
        goto func_name
        (return address)

        """

        RET = func_name + 'RET' + str(self.call_count) # Unique return label
        self.call_count += 1

        # push return address
        self.asm.append('@' + RET)
        self.asm.append('D=A')
        self.push_D_to_stack()

        # push LCL
        # push ARG
        # push THIS
        # push THAT
        for address in ["@LCL", "@ARG", "@THIS", "@THAT"]:
            self.asm.append(address)
            self.asm.append("D=M")
            self.push_D_to_stack()

        # LCL = SP
        self.asm.append('@SP')
        self.asm.append('D=M')
        self.asm.append('@LCL')
        self.asm.append('M=D')

        # ARG = SP-n-5
        # Reposition ARG
        self.asm.append("@" + str(int(arg_num)+5))
        self.asm.append("D=D-A")
        self.asm.append("@ARG")
        self.asm.append("M=D")

        # goto function
        self.asm.append('@' + func_name)
        self.asm.append("0;JMP")

        # (return address)
        self.asm.append(f"({RET})")
        
    def parse_return(self):
        """
        moves the return value to the caller 
        reinstates the caller's state
        and then go to return address, copy the top most value to it
        
        endFrame = LCL                 // temp variable
        RET = *(endFrame-5)  // get the return address
        *ARG = pop()                   // repositions the return value to ARG[0]
        SP = ARG[0] + 1                // reposition SP 
        // restore 
        THAT = *(endFrame-1)
        THIS = *(endFrame-2)
        ARG = *(endFrame-3)
        LCL = *(endFrame-4)
        goto RET

        """
        endFrame = "R13"
        RET = "R14"

        # endFrame = LCL
        self.asm.extend(["@LCL", "D=M", "@"+endFrame, "M=D"])

        # retAddr = *(endFrame-5)  
        self.asm.extend(["@"+endFrame, "D=M", "@5", "D=D-A", "A=D", "D=M", "@"+RET, "M=D"])

        # *ARG = pop()    
        self.pop_stack_to_D()
        self.asm.extend(["@ARG", "A=M", "M=D"])

        # SP = ARG[0] + 1   
        self.asm.extend(["@ARG", "D=M", "@SP", "M=D+1"])

        # // restore 
        # THAT = *(endFrame-1)
        # THIS = *(endFrame-2)
        # ARG = *(endFrame-3)
        # LCL = *(endFrame-4)
        for i, address in enumerate(["@THAT", "@THIS", "@ARG", "@LCL"], start=1):
            self.asm.extend(["@"+endFrame, "D=M", "@"+str(i), "AD=D-A", "D=M", address, "M=D"])

        # goto RET
        self.asm.extend(["@"+RET, "A=M", "0;JMP"])


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
        elif segment == STATIC:
            self.asm.extend([f"@{self.vm_file_name}-{index}"])
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