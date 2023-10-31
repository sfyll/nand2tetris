from SymbolTable import SymbolTable
from VMWriter import VMWriter
from JackTokenizer import JackTokenizer

class CompilationEngine:

    def __init__(self, tokenizer, vm_writer):
        self.tokenizer: JackTokenizer= tokenizer
        self.vm_writer: VMWriter = vm_writer
        self.symbol_table = SymbolTable()
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

        if token_type == "keyword" and token == "method":
            self.symbol_table.defineThisForMethod()

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
            self.symbol_table.className = self.tokenizer.current_token
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
        kind = self.tokenizer.current_token
        if self.tokenizer.expect_keyword() and self.tokenizer.current_token in ["static", "field"]:
            self.consume_token()

        # Type (which can be either a primitive type or a className)
        type_=self.tokenizer.current_token
        if self.tokenizer.expect_keyword() or self.tokenizer.expect_identifier():
            self.consume_token()

        # varName
        name = self.tokenizer.current_token
        if self.tokenizer.expect_identifier():
            self.consume_token()

        # (',' varName)*
        self.symbol_table.define(name, type_, kind)
        while self.tokenizer.expect_symbol(','):
            self.consume_token()  # consume the comma
            if self.tokenizer.expect_identifier():
                name = self.tokenizer.current_token
                self.symbol_table.define(name, type_, kind)
                self.consume_token()  # consume the next varName

        # ';' symbol
        if self.tokenizer.expect_symbol(';'):
            self.consume_token()

        self.indent -= 1
        # End the class variable declaration
        self.write("</classVarDec>")

    def compileSubroutine(self):
        self.write("<subroutineDec>")
        self.symbol_table.startSubroutine()
        self.vm_writer.reset()
        self.indent += 1 
        # Subroutine type ('constructor', 'function', or 'method')
        self.symbol_table.subroutine_type = self.tokenizer.current_token
        if self.tokenizer.expect_keyword() and self.tokenizer.current_token in ["constructor", "function", "method"]:
            self.consume_token()
        
        # Return type ('void' or type)
        self.symbol_table.subroutine_return_type = self.tokenizer.current_token
        if self.tokenizer.expect_keyword() or self.tokenizer.expect_identifier():
            self.consume_token()
        
        # Subroutine name
        if self.tokenizer.expect_identifier():
            self.symbol_table.subroutine_name = self.tokenizer.current_token
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

    def compileParameterList(self) -> int:
        self.write("<parameterList>")
        parameter_count = 0
        # Check if there's a type, indicating at least one parameter.
        if self.tokenizer.expect_identifier() or self.tokenizer.expect_keyword():
            parameter_count += 1
            self.indent += 1
            # Compile first type-varName pair
            type_ = self.tokenizer.current_token
            self.consume_token()  # Consume the type
            if self.tokenizer.expect_identifier():
                name = self.tokenizer.current_token
                self.symbol_table.define(name, type_, "argument")
                self.consume_token()  # Consume the variable name
            
            # While the next token is a comma, continue compiling type-varName pairs
            while self.tokenizer.expect_symbol(','):
                parameter_count += 1
                self.consume_token()  # Consume the comma
                if self.tokenizer.expect_identifier() or self.tokenizer.expect_keyword():
                    type_ = self.tokenizer.current_token
                    self.consume_token()  # Consume the type
                if self.tokenizer.expect_identifier():
                    name = self.tokenizer.current_token
                    self.symbol_table.define(name, type_, "argument")
                    self.consume_token()  # Consume the variable name
            self.indent -= 1 
        self.write("</parameterList>")
        return parameter_count

    def compileSubroutineBody(self):
        self.write("<subroutineBody>")
        self.indent += 1

        # '{' symbol
        if self.tokenizer.expect_symbol('{'):
            self.consume_token()

        # Compile variable declarations (zero or more)
        while self.tokenizer.expect_keyword("var"):
            self.compileVarDec()
 
        self.vm_writer.writeFunction(self.symbol_table.subroutine_name, self.symbol_table.localCount)

        #Add memory space for constructor arguments if needed
        if self.symbol_table.subroutine_type == "constructor":
            self.vm_writer.writePush("constant", self.symbol_table.fieldCount)
            self.vm_writer.writeCall("Memory.alloc", 1)
            self.vm_writer.writePop("pointer", 0)

        if self.symbol_table.subroutine_type == "method":
            self.vm_writer.writePush("argument", 0)
            self.vm_writer.writePop("pointer", 0)

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
        type_ = self.tokenizer.current_token
        if self.tokenizer.expect_keyword() or self.tokenizer.expect_identifier():
            self.consume_token()

        # First varName
        name = self.tokenizer.current_token
        self.symbol_table.define(name,  type_, "local")
        if self.tokenizer.expect_identifier():
            self.consume_token()

        # Comma-separated list of additional varNames
        while self.tokenizer.expect_symbol(','):
            self.consume_token()
            if self.tokenizer.expect_identifier():
                name = self.tokenizer.current_token
                self.symbol_table.define(name,  type_, "local")
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
                # We do not care about the return value of a do function call!
                self.vm_writer.writePop("temp", 0)
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

        varName = None
        indexed_array = False

        # 'let' keyword
        if self.tokenizer.expect_keyword("let"):
            self.consume_token()

        # varName
        if self.tokenizer.expect_identifier():
            varName = self.tokenizer.current_token
            self.consume_token()

        # Optional '[' expression ']'
        if self.tokenizer.expect_symbol("["):
            self.consume_token()
            self.compileExpression()
            try:
                entry = self.symbol_table.find_entry(varName)
                self.vm_writer.writePush(entry.kind, entry.index)
                self.vm_writer.writeOperator("+")
                indexed_array = True
            except Exception as e:
                raise e
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
        #Assign to the variable
        try:
            entry = self.symbol_table.find_entry(varName)
            if indexed_array:
                        self.vm_writer.writePop("temp", 0)
                        self.vm_writer.writePop("pointer", 1)
                        self.vm_writer.writePush("temp", 0)
                        self.vm_writer.writePop("that", 0)
            else:
                self.vm_writer.writePop(entry.kind, entry.index)
        except Exception as e:
            raise e
        self.write("</letStatement>")

    def compileIf(self):
        self.write("<ifStatement>")
        self.indent += 1
        self.vm_writer.if_label_counter += 1

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
        success_label = self.vm_writer.get_next_if_label("TRUE")
        failure_label = self.vm_writer.get_next_if_label("FALSE")
        end_label = self.vm_writer.get_next_if_label("END")
        self.vm_writer.writeIf(success_label)
        self.vm_writer.writeGoto(failure_label)
        self.vm_writer.writeLabel(success_label)
        if self.tokenizer.expect_symbol("{"):
            self.consume_token()
            self.compileStatements()
            if self.tokenizer.expect_symbol("}"):
                self.consume_token()

        self.vm_writer.writeGoto(end_label)
        self.vm_writer.writeLabel(failure_label)

        # Optional 'else' clause
        if self.tokenizer.expect_keyword("else"):
            self.consume_token()
            if self.tokenizer.expect_symbol("{"):
                self.consume_token()
                self.compileStatements()
                if self.tokenizer.expect_symbol("}"):
                    self.consume_token()

        self.vm_writer.writeLabel(end_label)
        
        self.indent -= 1 
        self.write("</ifStatement>")

    def compileWhile(self):
        self.write("<whileStatement>")
        self.vm_writer.while_label_counter += 1
        self.indent += 1 

        success_label = self.vm_writer.get_next_while_label("EXP")
        failure_label = self.vm_writer.get_next_while_label("END")
        self.vm_writer.writeLabel(success_label)

        # 'while' keyword
        if self.tokenizer.expect_keyword("while"):
            self.consume_token()

        # '(' expression ')'
        if self.tokenizer.expect_symbol("("):
            self.consume_token()
            self.compileExpression()
            if self.tokenizer.expect_symbol(")"):
                self.consume_token()

        self.vm_writer.writeUnaryOperator('~')

        self.vm_writer.writeIf(failure_label)

        # '{' statements '}'
        if self.tokenizer.expect_symbol("{"):
            self.consume_token()
            self.compileStatements()
            if self.tokenizer.expect_symbol("}"):
                self.consume_token()

        self.vm_writer.writeGoto(success_label)
        self.vm_writer.writeLabel(failure_label)
                
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
        
        self.vm_writer.writeReturn(self.symbol_table)

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
            operator = self.tokenizer.current_token
            self.consume_token()
            
            # Compile the subsequent term
            self.compileTerm()

            self.vm_writer.writeOperator(operator)

        self.indent -= 1 
        self.write("</expression>")

    def compileTerm(self):
        self.write("<term>")
        self.indent += 1 

        # Check for integer constant
        if self.tokenizer.expect_integerConstant():
            self.vm_writer.writePush("constant", self.tokenizer.current_token)
            self.consume_token()

        # Check for string constant
        elif self.tokenizer.expect_stringConstant():
            self.vm_writer.writePush("constant", len(self.tokenizer.current_token[1:-1]))
            self.vm_writer.writeCall("String.new", 1)
            for character in self.tokenizer.current_token[1:-1]:
                self.vm_writer.writePush("constant", ord(character))
                self.vm_writer.writeCall("String.appendChar", 2)
            self.consume_token()

        # Check for keyword constant (true, false, null, this)
        elif self.tokenizer.expect_keyword() and self.tokenizer.current_token in ["true", "false", "null", "this"]:
            if self.tokenizer.current_token == "true":
                self.vm_writer.writePush("constant", 0)
                self.vm_writer.writeUnaryOperator("~")
            elif self.tokenizer.current_token in ["false", "null"]:
                self.vm_writer.writePush("constant", 0)
            else:
                self.vm_writer.writePush("pointer", 0)
            self.consume_token()

        # Check for varName (start with identifier but not followed by '[' or '(' or '.')
        elif self.tokenizer.expect_identifier():
            # lookahead without consuming
            next_token = self.tokenizer.peek_next_token()
            # varName '[' expression ']'
            if next_token == "[":
                array = self.symbol_table.find_entry(self.tokenizer.current_token)
                
                self.consume_token()
                if self.tokenizer.expect_symbol("["):
                    self.consume_token()

                self.compileExpression()

                try:
                    self.vm_writer.writePush(array.kind, array.index)
                except Exception as e:
                    raise e
                self.vm_writer.writeOperator("+")

                try:
                    self.vm_writer.writePop("pointer", "1")
                    self.vm_writer.writePush("that", 0)
                except Exception as e:
                    raise e
                
                if self.tokenizer.expect_symbol("]"):
                    self.consume_token()

            # subroutineCall
            elif next_token in ["(", "."]:
                self.compileSubroutineCall()

            # plain varName
            else:
                try:
                    entry = self.symbol_table.find_entry(self.tokenizer.current_token)
                    self.vm_writer.writePush(entry.kind, entry.index)
                except Exception as e:
                    raise e
                self.consume_token()

        # '(' expression ')'
        elif self.tokenizer.expect_symbol("("):
            self.consume_token()
            self.compileExpression()
            if self.tokenizer.expect_symbol(")"):
                self.consume_token()

        # unaryOp term (unaryOp being '-' or '~')
        elif self.tokenizer.expect_symbol() and self.tokenizer.current_token in ["-", "~"]:
            unary_operator = self.tokenizer.current_token
            self.consume_token()
            self.compileTerm()
            self.vm_writer.writeUnaryOperator(unary_operator)

        self.indent -= 1 
        self.write("</term>")

    def compileSubroutineCall(self):
        # subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'
        function_name = ""
        expression_count = 0
        entry = None
        if self.tokenizer.expect_identifier():
            #fix name maping
            try:
                entry = self.symbol_table.find_entry(self.tokenizer.current_token)
                function_name += entry.type_
                if entry.type_ not in ["bool", "int", "char"]:
                    expression_count += 1
                    self.vm_writer.writePush(entry.kind, entry.index)
            except Exception as e:
                function_name += self.tokenizer.current_token
            finally:
                self.consume_token()
        if self.tokenizer.expect_symbol("."):
            self.consume_token()
            function_name += "." + self.tokenizer.current_token
        else:
            #in class function
            function_name = self.symbol_table.className + "." + function_name
            expression_count += 1
            self.vm_writer.writePush("pointer", 0)

        self.consume_token()
        
        if self.tokenizer.expect_symbol("("):
            self.consume_token()
            
        expression_count += self.compileExpressionList()
        self.vm_writer.writeCall(function_name, expression_count)
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
