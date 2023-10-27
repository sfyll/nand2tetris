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

    def is_operator(self):
        operators = ['+', '-', '*', '/', '&', '|', '<', '>', '=']
        return self.current_token() in operators

    def hasMoreTokens(self):
        return self.current_index + 1 < len(self.tokens)

    def hasLessTokens(self):
        return self.current_index - 1 > len(self.tokens)
    
    def advance(self):
        if self.hasMoreTokens():
            self.current_index += 1
            self.current_token = self.tokens[self.current_index]

    def retreat(self):
        if self.hasLessTokens():
            self.current_index -= 1
            self.current_token = self.tokens[self.current_index]
    
    def advance_and_get_token(self) -> Optional[Union[int, str]]:
        self.advance()
        return self.current_token

    def advance_and_get_token_type(self) -> Optional[Union[int, str]]:
        self.advance()
        return self.token_type()
    
    def peek_next_token(self):
        self.advance()
        next_token = self.current_token
        self.retreat()
        return next_token

    def expect_symbol(self, symbol = False) -> bool:
        if not symbol:
            return self.token_type() == "symbol"
        else:    
            return self.token_type() == "symbol" and self.current_token == symbol

    def expect_keyword(self, keyword = False) -> bool:
        if not keyword:
            return self.token_type() == "keyword"
        else:    
            return self.token_type() == "keyword" and self.current_token == keyword
    
    def expect_integerConstant(self, integerConstant = False) -> bool:
        if not integerConstant:
            return self.token_type() == "integerConstant"
        else:    
            return self.token_type() == "integerConstant" and self.current_token == integerConstant

    def expect_stringConstant(self, stringConstant = False) -> bool:
        if not stringConstant:
            return self.token_type() == "stringConstant"
        else:    
            return self.token_type() == "stringConstant" and self.current_token == stringConstant
    
    def expect_identifier(self, identifier = False) -> bool:
        if not identifier:
            return self.token_type() == "identifier"
        else:    
            return self.token_type() == "identifier" and self.current_token == identifier
    
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

