COMPILER = g++
OUTPUT_NAME = todoApp
LINKER = -lsqlite3

all :  run_FLTK

run :  
	clear
	$(COMPILER) todos.cpp sql_functions.cpp -o $(OUTPUT_NAME) $(LINKER)
	./$(OUTPUT_NAME)

clean: 
	clear
	rm $(OUTPUT_NAME)

run_FLTK :
	clear
	$(COMPILER) FLTK_wireframe.cpp -o FLTK_wireframe_app -lfltk
	./FLTK_wireframe_app
	
      
