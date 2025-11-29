COMPILER = g++
OUTPUT_NAME = todoApp
LINKER = -lsqlite3

all :  run

run :  
	clear
	$(COMPILER) todos.cpp -o $(OUTPUT_NAME) $(LINKER)
	./$(OUTPUT_NAME)

clean: 
	clear
	rm $(OUTPUT_NAME)
      
