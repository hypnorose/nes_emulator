run: build 
	main.exe
build: clean all
all: cpu.o decoder.o instructions.o main.o memory.o
	g++ -o main.exe  main.o glut32.lib -lopengl32 -lglu32


cpu.o: cpu.cpp
	g++ -c cpu.cpp -o cpu.o
main.o: main.cpp
	g++ -c main.cpp -o main.o

decoder.o: decoder.cpp
	g++ -c decoder.cpp -o decoder.o

instructions.o: instructions.cpp
	g++ -c instructions.cpp -o instructions.o

memory.o: memory.cpp
	g++ -c memory.cpp -o memory.o

clean:
	del *.o
	del *.exe