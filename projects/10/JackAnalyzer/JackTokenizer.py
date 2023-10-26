import re
from typing import Optional, Union

class JackTokenizer:
    def __init__(self, file_path):
        # Remove comments and tokenize input
        self.tokens = self.tokenize_jack_file(file_path)
        self.current_token: Optional[Union[int, str]] = None
        self.current_index = -1

    @staticmethod
    def tokenize_jack_file(file_path):
        with open(file_path, 'r') as f:
            content = f.read()
        content = JackTokenizer.remove_comments(content)
        tokenized_content = re.findall(r'[\w]+|["].+["]|[\{\}\(\)\[\]\.,;\+\-\*/&|<>=~]', content.replace('//', '\n'))
        return tokenized_content
    
    @staticmethod
    def remove_comments(content):
        # Remove single line comments
        content = re.sub(r'//.*', '', content)
        
        # Remove multiline comments
        content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
        
        return content

    def hasMoreTokens(self):
        return self.current_index + 1 < len(self.tokens)

    def advance(self):
        if self.hasMoreTokens():
            self.current_index += 1
            self.current_token = self.tokens[self.current_index]

    def advance_and_get_token(self) -> Optional[Union[int, str]]:
        self.advance()
        return self.current_token

    def advance_and_get_token_type(self) -> Optional[Union[int, str]]:
        self.advance()
        return self.token_type()
    
    def token_type(self):
        if not self.current_token:
            return None
        elif self.current_token in ['class', 'constructor', 'function', 'method', 'field', 'static', 'var', 'int', 'char', 'boolean', 'void', 'true', 'false', 'null', 'this', 'let', 'do', 'if', 'else', 'while', 'return']:
            return "keyword"
        elif self.current_token in ['{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~']:
            return "symbol"
        elif self.current_token.isnumeric():
            return "integerConstant"
        elif self.current_token.startswith('"'):
            return "stringConstant"
        else:
            return "identifier"

