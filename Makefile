COMPILER = g++
OUTPUT_NAME = todoApp
LINKER = -lsqlite3 -lfltk

all : run 

run :  
	clear
	$(COMPILER) todos.cpp sql_functions.cpp FLTK_wireframe.cpp -o $(OUTPUT_NAME) $(LINKER)
	./$(OUTPUT_NAME)

clean: 
	clear
	rm $(OUTPUT_NAME)

fltk :
	clear
	$(COMPILER) FLTK_wireframe.cpp -o FLTK_wireframe_app -lfltk
	./FLTK_wireframe_app
	
      
