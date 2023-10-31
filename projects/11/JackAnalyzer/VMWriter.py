import os

from SymbolTable import SymbolTable

class VMWriter:
    def __init__(self, output_file):
        self.file = open(output_file, 'w')
        self.fileName = os.path.splitext(os.path.basename(output_file))[0]
        self.if_label_counter = -1
        self.while_label_counter = -1

    def reset(self):
        self.if_label_counter = -1
        self.while_label_counter = -1
 
    def get_next_if_label(self, type_: "TRUE"):
        return f"IF_{type_}{self.if_label_counter}"
    
    def get_next_while_label(self, type_: "EXP"):
        return f"WHILE_{type_}{self.while_label_counter}"
    
    def writePush(self, segment, index):
        if segment == "field":
            segment = "this"
        self.file.write(f"push {segment} {index}\n")

    def writePop(self, segment, index):
        if segment == "field":
            segment = "this"
        self.file.write(f"pop {segment} {index}\n")

    def writeOperator(self, command):
        if command == "+":
            self.file.write(f"add\n")
        elif command == "-":
            self.file.write(f"sub\n")
        elif command == "*":
            self.writeCall("Math.multiply", 2)
        elif command == "/":
            self.writeCall("Math.divide", 2)
        elif command == ">":
            self.file.write(f"gt\n")
        elif command == "<":
            self.file.write(f"lt\n")
        elif command == "=":
            self.file.write(f"eq\n")
        elif command == "&":
            self.file.write(f"and\n")
        else:
            raise NotImplementedError(f"{command=}")

    def writeUnaryOperator(self, command):
        if command == "-":
            self.file.write(f"neg\n")
        elif command == "~":
            self.file.write(f"not\n")

    def writeLabel(self, label):
        self.file.write(f"label {label}\n")

    def writeGoto(self, label):
        self.file.write(f"goto {label}\n") 

    def writeIf(self, label = None):
        self.file.write(f"if-goto {label}\n") 

    def writeCall(self, name, nArgs):
        self.file.write(f"call {name} {nArgs}\n")

    def writeConstructorFunction(self, className, nVars):
        self.file.write(f"function {className}.new {nVars}\n")

    def writeFunction(self, name, nVars):
        self.file.write(f"function {self.fileName}.{name} {nVars}\n")

    def writeReturn(self, symbol_table: SymbolTable):
        if symbol_table.subroutine_return_type == 'void':
            self.writePush("constant", 0)
        self.file.write("return\n")

    def close(self):
        self.file.close()

