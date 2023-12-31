from typing import Dict

class SymbolTable:
    def __init__(self, className=None, subroutineReturnType=None, subroutineName = None, subroutineType = None):
        self.className = className
        self.classTable: Dict[str, Entry] = {}  
        self.subroutineTable: Dict[str, Entry] = {} 
        self.subroutine_return_type = subroutineReturnType
        self.subroutine_name = subroutineName
        self.subroutine_type = subroutineType
        self.partial_entries = {}  # To hold the accumulating attributes
        self.staticCount = 0
        self.fieldCount = 0
        self.localCount = 0
        self.argumentCount = 0

    def find_entry(self, key):
        if key in self.classTable:
            return self.classTable[key]
        elif key in self.subroutineTable:
            return self.subroutineTable[key]
        else:
            raise KeyError(f"'{key}' not found in classTable or subroutineTable.")

    def pretty_print_table(self, table_name):
        if table_name == "classTable":
            table = self.classTable
        elif table_name == "subroutineTable":
            table = self.subroutineTable
        else:
            raise ValueError(f"Unknown table: {table_name}")

        entries_str = ", ".join([str(entry) for entry in table.values()])
        return f"{table_name}=[{entries_str}]"

    def _get_and_increment_count(self, kind):
        if kind == 'static':
            index = self.staticCount
            self.staticCount += 1
        elif kind == 'field':
            index = self.fieldCount
            self.fieldCount += 1
        elif kind == 'local':
            index = self.localCount
            self.localCount += 1
        elif kind == 'argument':
            index = self.argumentCount
            self.argumentCount += 1
        else:
            raise ValueError(f"Unknown kind: {kind}")
        return index
    
    def startSubroutine(self):
        self.subroutineTable = {}
        self.localCount = 0
        self.argumentCount = 0

    def define(self, name, type_, kind):
        index = self._get_and_increment_count(kind)
        entry = Entry(name, type_, kind, index)
        
        if kind in ['static', 'field']:
            self.classTable[name] = entry
        else:
            self.subroutineTable[name] = entry

    def defineThisForMethod(self):
        # Automatically add the 'this' argument for methods
        self.define("this", self.className, "argument")

    def accumulate(self, name=None, kind=None, type_=None):
        if name is None or name not in self.partial_entries:
            self.partial_entries[name] = {"name": name}

        if kind is not None:
            if 'kind' in self.partial_entries[name]:
                raise ValueError(f"'kind' for {name} is already set. Overwriting is not allowed.")
            self.partial_entries[name]['kind'] = kind
            
        if type_ is not None:
            if 'type_' in self.partial_entries[name]:
                raise ValueError(f"'type' for {name} is already set. Overwriting is not allowed.")
            self.partial_entries[name]['type_'] = type_

        # Check if the entry is complete and can be defined
        entry = self.partial_entries.get(name, {})
        if 'name' in entry and 'kind' in entry and 'type_' in entry:
            self.define(entry['name'], entry['type_'], entry['kind'])
            del self.partial_entries[name]  # remove the entry from accumulator once defined

class Entry:
    def __init__(self, name, type_, kind, index):
        self.name = name
        self.type_ = type_
        self.kind = kind
        self.index = index

    def __str__(self):
        return f"Entry(name={self.name}, type_={self.type_}, kind={self.kind}, index={self.index})"

# Usage
st = SymbolTable("Point")
st.define("x", "int", "field")
st.define("y", "int", "field")
st.define("pointCount", "int", "static")

# Starting a new subroutine (method)
st.startSubroutine()
st.defineThisForMethod()  # this will add 'this' of type 'Point' as an argument
st.define("other", "Point", "argument")
st.define("dx", "int", "local")
st.define("dy", "int", "local")
