all :
	gcc -Wall -lm -std=c11 -gdwarf-2 -ggdb3 -o pagerank *.c
clean :
	rm -f pagerank *.o