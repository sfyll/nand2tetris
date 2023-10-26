import os
import sys 

from JackTokenizer import JackTokenizer

class JackAnalyzer:
    def __init__(self, file_location):
        self.input_file = file_location
        self.output_file = self.get_output_file_location()
        self.tokenizer = JackTokenizer(file_location)
        print(self.tokenizer.tokens)

    def get_output_file_location(self) -> str:
        base_name = os.path.splitext(self.input_file)[0]
        output_file_path = base_name + "est.xml"
        return output_file_path

    def generate_tokens_xml(self) -> str:
        xml_output = "<tokens>\n"

        while self.tokenizer.hasMoreTokens():

            token_type = self.tokenizer.advance_and_get_token_type()
            token = self.tokenizer.current_token

            if token_type == "symbol" and token in ['<', '>', '&', '"']:
                if token == '<':
                    token = '&lt;'
                elif token == '>':
                    token = '&gt;'
                elif token == '&':
                    token = '&amp;'
                elif token == '"':
                    token = '&quot'
            if token_type == "stringConstant":
                token = token[1:-1] 

            xml_output += f"  <{token_type}> {token} </{token_type}>\n"

        xml_output += "</tokens>"
        return xml_output

    def generate_and_write_xml(self) -> None:
        xml_content = self.generate_tokens_xml()
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
            executor.generate_and_write_xml()
        else:
            print("Provided file is not a .jack file.")
    elif os.path.isdir(input_path):
        for file_name in os.listdir(input_path):
            if file_name.endswith('.jack'):
                full_file_path = os.path.join(input_path, file_name)
                executor = JackAnalyzer(full_file_path)
                executor.generate_and_write_xml()
    else:
        print("Provided path is neither a file nor a directory.") 
    

