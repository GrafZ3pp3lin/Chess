all: MoveGenerator.o Player.o Evaluation.o
	g++ -g -o main.exe main.cpp MoveGenerator.o Player.o Evaluation.o KI.o

MoveGenerator.o: MoveGenerator.hpp MoveGenerator.cpp
	g++ -g -o MoveGenerator.o -c MoveGenerator.cpp

Player.o: Player.hpp Player.cpp
	g++ -g -o Player.o -c Player.cpp

Evaluation.o: Evaluation.hpp Evaluation.cpp
	g++ -g -o Evaluation.o -c Evaluation.cpp

KI.o: KI.hpp KI.cpp
	g++ -g -o KI.o -c KI.cpp

clean:
	del *.o main.exe
