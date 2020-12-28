import sys
import os

COMMENT = "//"
ARITH_OPS = {"add","sub","neg","eq","gt","lt","and","or","not"}
STACK_OPS = {"push", "pop"}
PREDEFINED_SEGMENTS = {
    "local": "LCL",     # LCL's  address is stored in R1
    "argument": "ARG",  # ARG's  address is stored in R2
    "this": "THIS",     # THIS's address is stored in R3
    "that": "THAT",     # THAT's address is stored in R4
}
UN_DEFINED_SEGMENTS = {
    "pointer": 3,   # R3, R4, basically this and that
    "temp": 5,      # R5-12
                    # R13-15 are free
    "static": 16,   # R16-255
}
class VMTranslator:
    def __init__(self, vm_path):
        self.vm_path = vm_path
        self.vm = [line.strip() for line in open(vm_path)]
        self.asm_path = os.path.splitext(vm_path)[0] + ".asm"
        self.asm = []

    def remove_comments(self):
        def clean(line):
            if COMMENT in line:
                return line.split(COMMENT)[0].strip()
            else:
                return line
        self.vm = [clean(line) for line in self.vm if clean(line)]

    def error(self, *args):
        raise ValueError(f"{args} invalid")

    def parse(self):
        for line in self.vm:
            cmd, *args = line.split()
            num_args = len(args)
            if   cmd in ARITH_OPS and num_args == 0:
                self.parse_arith_ops(cmd)
            elif cmd in STACK_OPS and num_args == 2:
                self.parse_stack_ops(cmd, *args)

    def parse_arith_ops(self, cmd):
        self.asm.append(f"// {cmd}")
        self.asm.append(f"...")

    def parse_stack_ops(self, cmd, segment, index):
        self.asm.append(f"// {cmd} {segment} {index}")
        self.set_segment_address_to_A(segment, index)
        self.asm.append(f"...")


    def set_segment_address_to_A(self, segment, index):
        if segment in PREDEFINED_SEGMENTS:
            self.asm += [f"@{PREDEFINED_SEGMENTS[segment]}", "D=M", f"@{index}", "A=D+A"]


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