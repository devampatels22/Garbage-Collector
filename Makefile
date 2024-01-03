Compile: test.c ObjectManager.c ObjectManager.h
	clang++ -Wall -DNDEBUG -c ObjectManager.c -o objM.o
	clang++ -Wall -DNDEBUG objM.o test.c -o test
	
clean:
	rm objM.o test