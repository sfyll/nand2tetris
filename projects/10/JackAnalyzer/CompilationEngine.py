class CompilationEngine:

    def __init__(self, tokenizer):
        self.tokenizer = tokenizer
        self.output = ""
        self.indent = 0

    """This method is to generate the token test files"""
    def generate_tokens_xml(self) -> None:
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
        self.write(xml_output)
    
    def consume_token(self) -> None:

        token_type = self.tokenizer.token_type()
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

        xml_output = f"<{token_type}> {token} </{token_type}>"
        self.tokenizer.advance()
        self.write(xml_output)
    
    def compileClass(self):
        self.write("<class>")
        self.indent += 1
        self.tokenizer.advance()
        # 'class' keyword
        if self.tokenizer.expect_keyword("class"):
            self.consume_token()
        
        # ClassName
        if self.tokenizer.expect_identifier():
            self.consume_token()

        # '{' symbol
        if self.tokenizer.expect_symbol('{'):
            self.consume_token()
                                             
        # Compile class variable declarations
        while self.tokenizer.current_token in ["static", "field"]:
            self.compileClassVarDec()

        # Compile subroutines
        while self.tokenizer.current_token in ["constructor", "function", "method"]:
            self.compileSubroutine()

        # '}' symbol
        if self.tokenizer.expect_symbol('}'):
            self.consume_token()
        self.indent -= 1 
        self.write("</class>")

    def compileClassVarDec(self):
        # Begin the class variable declaration with XML tag
        self.write("<classVarDec>")
        self.indent += 1
        # ('static' | 'field')
        if self.tokenizer.expect_keyword() and self.tokenizer.current_token in ["static", "field"]:
            self.consume_token()

        # Type (which can be either a primitive type or a className)
        if self.tokenizer.expect_keyword() or self.tokenizer.expect_identifier():
            self.consume_token()

        # varName
        if self.tokenizer.expect_identifier():
            self.consume_token()

        # (',' varName)*
        while self.tokenizer.expect_symbol(','):
            self.consume_token()  # consume the comma
            if self.tokenizer.expect_identifier():
                self.consume_token()  # consume the next varName

        # ';' symbol
        if self.tokenizer.expect_symbol(';'):
            self.consume_token()

        self.indent -= 1
        # End the class variable declaration
        self.write("</classVarDec>")

    def compileSubroutine(self):
        self.write("<subroutineDec>")
        self.indent += 1 
        # Subroutine type ('constructor', 'function', or 'method')
        if self.tokenizer.expect_keyword() and self.tokenizer.current_token in ["constructor", "function", "method"]:
            self.consume_token()
        
        # Return type ('void' or type)
        if self.tokenizer.expect_keyword() or self.tokenizer.expect_identifier():
            self.consume_token()
        
        # Subroutine name
        if self.tokenizer.expect_identifier():
            self.consume_token()

        # '(' symbol
        if self.tokenizer.expect_symbol('('):
            self.consume_token()

        # Compile parameter list
        self.compileParameterList()
        
        # ')' symbol
        if self.tokenizer.expect_symbol(')'):
            self.consume_token()

        # Subroutine body: '{' statements '}'
        if self.tokenizer.expect_symbol('{'):
            self.compileSubroutineBody()
 
        self.indent -= 1 
        self.write("</subroutineDec>")

    def compileParameterList(self):
        self.write("<parameterList>")
        # Check if there's a type, indicating at least one parameter.
        if self.tokenizer.expect_identifier() or self.tokenizer.expect_keyword():
            self.indent += 1
            # Compile first type-varName pair
            self.consume_token()  # Consume the type
            if self.tokenizer.expect_identifier():
                self.consume_token()  # Consume the variable name
            
            # While the next token is a comma, continue compiling type-varName pairs
            while self.tokenizer.expect_symbol(','):
                self.consume_token()  # Consume the comma
                if self.tokenizer.expect_identifier() or self.tokenizer.expect_keyword():
                    self.consume_token()  # Consume the type
                if self.tokenizer.expect_identifier():
                    self.consume_token()  # Consume the variable name
            self.indent -= 1 
        self.write("</parameterList>")

    def compileSubroutineBody(self):
        self.write("<subroutineBody>")
        self.indent += 1

        # '{' symbol
        if self.tokenizer.expect_symbol('{'):
            self.consume_token()

        # Compile variable declarations (zero or more)
        while self.tokenizer.expect_keyword("var"):
            self.compileVarDec()

        # Compile statements
        self.compileStatements()

        # '}' symbol
        if self.tokenizer.expect_symbol('}'):
            self.consume_token()

        self.indent -= 1 
        self.write("</subroutineBody>")

    def compileVarDec(self):
        self.write("<varDec>")
        self.indent += 1 
        
        # 'var' keyword
        if self.tokenizer.expect_keyword('var'):
            self.consume_token()

        # Type (either a primitive type or a className, both can be represented as identifiers in this context)
        if self.tokenizer.expect_keyword() or self.tokenizer.expect_identifier():
            self.consume_token()

        # First varName
        if self.tokenizer.expect_identifier():
            self.consume_token()

        # Comma-separated list of additional varNames
        while self.tokenizer.expect_symbol(','):
            self.consume_token()
            if self.tokenizer.expect_identifier():
                self.consume_token()

        # ';' symbol
        if self.tokenizer.expect_symbol(';'):
            self.consume_token()

        self.indent -= 1 
        self.write("</varDec>")
    
    def compileStatements(self):
        self.write("<statements>")
        self.indent += 1 
        
        # Continually compile statements while valid ones are detected
        while True:
            # Peek the next token to determine the statement type
            token = self.tokenizer.current_token
            
            if token == "let":
                self.compileLet()
            elif token == "if":
                self.compileIf()
            elif token == "while":
                self.compileWhile()
            elif token == "do":
                self.compileDo()
            elif token == "return":
                self.compileReturn()
            else:
                # If no valid statement is detected, break out of the loop
                break

        self.indent -= 1 
        self.write("</statements>")

    def compileLet(self):
        self.write("<letStatement>")
        self.indent += 1 

        # 'let' keyword
        if self.tokenizer.expect_keyword("let"):
            self.consume_token()

        # varName
        if self.tokenizer.expect_identifier():
            self.consume_token()

        # Optional '[' expression ']'
        if self.tokenizer.expect_symbol("["):
            self.consume_token()
            self.compileExpression()
            if self.tokenizer.expect_symbol("]"):
                self.consume_token()

        # '='
        if self.tokenizer.expect_symbol("="):
            self.consume_token()

        # expression
        self.compileExpression()

        # ';'
        if self.tokenizer.expect_symbol(";"):
            self.consume_token()

        self.indent -= 1 
        self.write("</letStatement>")

    def compileIf(self):
        self.write("<ifStatement>")
        self.indent += 1

        # 'if' keyword
        if self.tokenizer.expect_keyword("if"):
            self.consume_token()

        # '(' expression ')'
        if self.tokenizer.expect_symbol("("):
            self.consume_token()
            self.compileExpression()
            if self.tokenizer.expect_symbol(")"):
                self.consume_token()

        # '{' statements '}'
        if self.tokenizer.expect_symbol("{"):
            self.consume_token()
            self.compileStatements()
            if self.tokenizer.expect_symbol("}"):
                self.consume_token()

        # Optional 'else' clause
        if self.tokenizer.expect_keyword("else"):
            self.consume_token()
            if self.tokenizer.expect_symbol("{"):
                self.consume_token()
                self.compileStatements()
                if self.tokenizer.expect_symbol("}"):
                    self.consume_token()

        self.indent -= 1 
        self.write("</ifStatement>")

    def compileWhile(self):
        self.write("<whileStatement>")
        self.indent += 1 

        # 'while' keyword
        if self.tokenizer.expect_keyword("while"):
            self.consume_token()

        # '(' expression ')'
        if self.tokenizer.expect_symbol("("):
            self.consume_token()
            self.compileExpression()
            if self.tokenizer.expect_symbol(")"):
                self.consume_token()

        # '{' statements '}'
        if self.tokenizer.expect_symbol("{"):
            self.consume_token()
            self.compileStatements()
            if self.tokenizer.expect_symbol("}"):
                self.consume_token()
                
        self.indent -= 1 
        self.write("</whileStatement>")

    def compileDo(self):
        self.write("<doStatement>")
        self.indent += 1 
        
        # 'do' keyword
        if self.tokenizer.expect_keyword("do"):
            self.consume_token()

        # subroutineCall (It involves the function call logic and might need a dedicated method.)
        self.compileSubroutineCall()

        # ';'
        if self.tokenizer.expect_symbol(";"):
            self.consume_token()
            
        self.indent -= 1 
        self.write("</doStatement>")

    def compileReturn(self):
        self.write("<returnStatement>")
        self.indent += 1 
        
        # 'return' keyword
        if self.tokenizer.expect_keyword("return"):
            self.consume_token()

        # Optional expression (Check if the next token is not ';', if so compile expression)
        if not self.tokenizer.expect_symbol(";"):
            self.compileExpression()

        # ';'
        if self.tokenizer.expect_symbol(";"):
            self.consume_token()
            
        self.indent -= 1 
        self.write("</returnStatement>")

    def compileExpression(self):
        self.write("<expression>")
        self.indent += 1 

        # Compile the initial term
        self.compileTerm()

        # While the current token is an operator, compile the operator and the subsequent term
        while self.tokenizer.is_operator():
            # Consume the operator
            self.consume_token()
            
            # Compile the subsequent term
            self.compileTerm()


        self.indent -= 1 
        self.write("</expression>")

    def compileTerm(self):
        self.write("<term>")
        self.indent += 1 

        # Check for integer constant
        if self.tokenizer.expect_integerConstant():
            self.consume_token()

        # Check for string constant
        elif self.tokenizer.expect_stringConstant():
            self.consume_token()

        # Check for keyword constant (true, false, null, this)
        elif self.tokenizer.expect_keyword() and self.tokenizer.current_token in ["true", "false", "null", "this"]:
            self.consume_token()

        # Check for varName (start with identifier but not followed by '[' or '(' or '.')
        elif self.tokenizer.expect_identifier():
            # lookahead without consuming
            next_token = self.tokenizer.peek_next_token()
            # varName '[' expression ']'
            if next_token == "[":
                self.consume_token()
                if self.tokenizer.expect_symbol("["):
                    self.consume_token()
                self.compileExpression()
                if self.tokenizer.expect_symbol("]"):
                    self.consume_token()

            # subroutineCall
            elif next_token in ["(", "."]:
                self.compileSubroutineCall()

            # plain varName
            else:
                self.consume_token()

        # '(' expression ')'
        elif self.tokenizer.expect_symbol("("):
            self.consume_token()
            self.compileExpression()
            if self.tokenizer.expect_symbol(")"):
                self.consume_token()

        # unaryOp term (unaryOp being '-' or '~')
        elif self.tokenizer.expect_symbol() and self.tokenizer.current_token in ["-", "~"]:
            self.consume_token()
            self.compileTerm()

        self.indent -= 1 
        self.write("</term>")

    def compileSubroutineCall(self):
        # subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'
        if self.tokenizer.expect_identifier():
            self.consume_token()
        if self.tokenizer.expect_symbol("."):
            self.consume_token()
            
        self.consume_token()

        if self.tokenizer.expect_symbol("("):
            self.consume_token()
        
        self.compileExpressionList()
        if self.tokenizer.expect_symbol(")"):
            self.consume_token()

    def compileExpressionList(self):
        # Initialization of expression count
        expression_count = 0
        self.write("<expressionList>")
        
        # Check if the next token is not ')', if so it means there's at least one expression
        while not self.tokenizer.expect_symbol(")"):
            # Compile an expression
            self.compileExpression()
            expression_count += 1
            
            # If the next token is a comma, there are more expressions. Consume the comma.
            if self.tokenizer.expect_symbol(","):
                self.consume_token()
        
        self.write("</expressionList>")
        
        return expression_count


    def write(self, data):
        self.output += f"  " * self.indent +  data + "\n"

