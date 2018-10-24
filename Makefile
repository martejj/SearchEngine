DEPS=graph.c list.c readData.c BTree.c Item.c SortedList.c Queue.c 
CFLAGS=-Werror -Wall -lm -std=c11 -gdwarf-2 -ggdb3

pagerank :
	gcc $(CFLAGS) -o pagerank pagerank.c $(DEPS)
inverted :
	gcc $(CFLAGS) -o inverted inverted.c $(DEPS)
searchPagerank :
	gcc $(CFLAGS) -o searchPagerank searchPagerank.c $(DEPS) 
searchTfIdf :
	gcc $(CFLAGS) -o searchTfIdf searchTfIdf.c $(DEPS)
scaledFootrule :
	gcc $(CFLAGS) -o scaledFootrule scaledFootrule.c $(DEPS) 
clean :
	rm -f pagerank inverted searchPagerank searchTfIdf scaledFootrule *.o
