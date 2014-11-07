# PSZT Neural Network Project Makefile
# 	data: 07.11
# 
# Every new .cpp file must be included in variable OBJECTS with .o extension.

OBJECTS=Source.o \

CXX=g++ -std=c++11
CFLAGS=-Wall -pedantic
EXEC=Project.o

# Final executable file
$(EXEC): $(OBJECTS)
	$(CXX) -o $(EXEC) $(OBJECTS)

%.o: %.c
	$(CXX) -c -o $@ $< $(CFLAGS)

# Treats following words as instructions, not filenames.
.PHONY: clean del

# Clears all temporary files.
clean:
	rm -rf $(OBJECTS)

# Deletes executable file.
del:
	rm -rf $(EXEC)