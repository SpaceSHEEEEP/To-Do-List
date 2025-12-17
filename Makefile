COMPILER = g++
OUTPUT_NAME = todoApp
LINKER = -lsqlite3 -lfltk

all : run 

run :  
	clear
	$(COMPILER) main.cpp Database.cpp GUI.cpp -o $(OUTPUT_NAME) $(LINKER)
	./$(OUTPUT_NAME)

clean: 
	clear
	rm $(OUTPUT_NAME)

fltk :
	clear
	$(COMPILER) FLTK_wireframe.cpp -o FLTK_wireframe_app -lfltk
	./FLTK_wireframe_app
	
      
