"""
Translate asm to hack 
run python3 HackAssembler.py add/Add.asm
output: Add.hack
"""

import sys
import os

COMMENT = "//"
# 16进制 predefined symbols
PREDEFINED_SYMBOLS = {
    'SP':   0x0000,
    'LCL':  0x0001,
    'ARG':  0x0002,
    'THIS': 0x0003,
    'THAT': 0x0004,
    'R0':   0x0000,
    'R1':   0x0001,
    'R2':   0x0002,
    'R3':   0x0003,
    'R4':   0x0004,
    'R5':   0x0005,
    'R6':   0x0006,
    'R7':   0x0007,
    'R8':   0x0008,
    'R9':   0x0009,
    'R10':  0x000a,
    'R11':  0x000b,
    'R12':  0x000c,
    'R13':  0x000d,
    'R14':  0x000e,
    'R15':  0x000f,
    'SCREEN': 0x4000,  # 16384
    'KBD':    0x6000,  # 24576
}

COMP_MAP = {
    '0':    '0101010',
    '1':    '0111111',
    '-1':   '0111010',
    'D':    '0001100',
    'A':    '0110000',  'M':    '1110000',
    '!D':   '0001101',
    '!A':   '0110001',  '!M':   '1110001',
    '-D':   '0001111',
    '-A':   '0110011',  '-M':   '1110011',
    'D+1':  '0011111',
    'A+1':  '0110111',  'M+1':  '1110111',
    'D-1':  '0001110',
    'A-1':  '0110010',  'M-1':  '1110010',
    'D+A':  '0000010',  'D+M':  '1000010',
    'D-A':  '0010011',  'D-M':  '1010011',
    'A-D':  '0000111',  'M-D':  '1000111',
    'D&A':  '0000000',  'D&M':  '1000000',
    'D|A':  '0010101',  'D|M':  '1010101',        
}

JUMP_MAP = {
    'JGT': '001',
    'JEQ': '010',
    'JGE': '011',
    'JLT': '100',
    'JNE': '101',
    'JLE': '110',
    'JMP': '111',
}


class Assembler:
    def __init__(self, asm_path):
        self.asm_path = asm_path
        self.asm = [line.strip() for line in open(asm_path)]
        self.hack_path = os.path.splitext(asm_path)[0] + '.hack'
        self.hack = []
        self.symbols = dict(PREDEFINED_SYMBOLS) # create a new one without changing original dict
        self.next_free_ram = 0x0010             # the next position after R15
    
    def run(self):
        self.remove_comments()
        self.mark_labels()
        self.parse()
        self.save_hack_file()

    def remove_comments(self):
        def clean(line):
            if COMMENT in line:
                return line.split(COMMENT)[0].strip()  # only keep codes before //
            else:
                return line
        self.asm = [clean(line) for line in self.asm if clean(line)]

    def mark_labels(self):
        filtered = []
        i = 0
        for line in self.asm:
            if line.startswith('('):
                label = line[1:-1].strip()
                self.symbols[label] = i   # store the next instruction for label
            else:
                filtered.append(line)
                i += 1
        self.asm = filtered

    def parse(self):
        for line in self.asm:
            if line.startswith('@'):  # A-instruction
                self.hack.append(self.parseA(line))
            else:
                self.hack.append(self.parseC(line))

    def parseA(self, line):
        '''
        A instruction format: @address
        hex16 is a function to tranform deicmal numbers into 16-bit binary
        hex16(10) = '0000000000001010'
        '''
        hex16 = lambda x: f'{x:>016b}'
        symbol = line[1:].strip()     # string
        if symbol.isdigit():          # @18
            return hex16(int(symbol))
        elif symbol in self.symbols:  # @R1
            value = self.symbols[symbol]
            return hex16(value)
        else:                         # @variable
            value = self.next_free_ram
            self.symbols[symbol] = value
            self.next_free_ram += 1
            return hex16(value)

    def parseC(self, line):
        # dest = comp; jump
        dest, comp, jump = None, None, None
        # print(line, dest, comp, jump)
        if "=" in line:   
            dest, line = line.split("=")
        if ";" in line:    # D = D+M; JMP / D; JMP
            comp, jump = line.split(";")
        else:              # D = A
            comp = line
        # print(dest, comp, jump)  # D A None

        # transform to binary
        def dest_map(dest):
            bin = ['0', '0', '0']
            if not dest:
                return ''.join(bin)
            if 'A' in dest:
                bin[0] = '1'
            if 'D'  in dest:
                bin[1] = '1'
            if 'M' in dest:
                bin[2] = '1'
            # print(dest, bin)
            return ''.join(bin)

        c = COMP_MAP.get(comp, '000000')
        j = JUMP_MAP.get(jump, '000')

        return '111'+ c + dest_map(dest) + j
        # print(f"{line},\t{comp},\t{dest},\t{jump},\t{output}")


    def save_hack_file(self):
        # print(self.hack_path)
        with open(self.hack_path, 'w') as hack_file:
            for line in self.hack:
                hack_file.write(line+'\n')

if __name__ == '__main__':
    assembler = Assembler(sys.argv[1])  # terminal传进去的第二个参数
    assembler.run()








