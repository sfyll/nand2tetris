# Output executable name
OUTFILE = VMTranslator

# Source files
SOURCES = VMTranslator.c hack_ram.c instructions.c template.c file_handler.c

# Build rule
all:
	gcc -Wall -g $(SOURCES) -o $(OUTFILE)
	chmod +x $(OUTFILE)

clean:
	rm -f $(OUTFILE)
