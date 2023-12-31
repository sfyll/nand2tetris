import os
import sys 

from JackTokenizer import JackTokenizer
from CompilationEngine import CompilationEngine 
from VMWriter import VMWriter

class JackAnalyzer:
    def __init__(self, file_location):
        self.input_file = file_location
        self.output_file = self.get_output_file_location()
        self.tokenizer = JackTokenizer(file_location)
        self.vm_writer = VMWriter(self.get_vm_output_file_location())
        print(self.tokenizer.tokens)
        self.compilation_engine = CompilationEngine(self.tokenizer, self.vm_writer)
        self.run()

    def run(self) -> None:
        self.compilation_engine.compileClass()

    def get_output_file_location(self) -> str:
        base_name = os.path.splitext(self.input_file)[0]
        output_file_path = base_name + ".xml"
        return output_file_path

    def get_vm_output_file_location(self) -> str:
        base_name = os.path.splitext(self.input_file)[0]
        output_file_path = base_name + ".vm"
        return output_file_path

    def write_xml(self, xml_content) -> None:
        with open(self.output_file, "w") as f:
            f.write(xml_content)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Please provide a file or directory as an argument.")
        sys.exit(1)

    input_path = sys.argv[1]

    if os.path.isfile(input_path):
        if input_path.endswith('.jack'):
            executor = JackAnalyzer(input_path)
        else:
            print("Provided file is not a .jack file.")
    elif os.path.isdir(input_path):
        for file_name in os.listdir(input_path):
            if file_name.endswith('.jack'):
                full_file_path = os.path.join(input_path, file_name)
                executor = JackAnalyzer(full_file_path)
    else:
        print("Provided path is neither a file nor a directory.") 
    

