SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
CC = g++ -std=c++17
EXECNAME = breakout
OPTIM_FLAG = -O2
SFML_PATH = 

$(EXECNAME): shape.o gameObjects.o utils.o game.o
	$(CC) shape.o gameObjects.o utils.o game.o -o $(EXECNAME) $(OPTIM_FLAG) $(SFML_FLAGS)
	rm -f *.o

game.o:
	$(CC) game.cpp -c $(OPTIM_FLAG) $(SFML_FLAGS)

shape.o:
	$(CC) shape.cpp -c $(OPTIM_FLAG) -lsfml-system

gameObjects.o:
	$(CC) gameObjects.cpp -c $(OPTIM_FLAG) $(SFML_FLAGS)

utils.o:
	$(CC) utils.cpp -c $(OPTIM_FLAG) -lsfml-system

clean:
	rm -f *.o
	rm -f $(EXECNAME)
