all: index search

index: index.o linkedList.o
	gcc -o index index.o linkedList.o

index.o: linkedList.h index.c
	gcc -c -o index.o index.c

linkedList.o: linkedList.h linkedList.c
	gcc -c -o linkedList.o linkedList.c

search: search.o dataStructure.o
	gcc -o search search.o dataStructure.o

search.o: dataStructure.h search.c
	gcc -c -o search.o search.c

dataStructure.o: dataStructure.h dataStructure.c
	gcc -c -o dataStructure.o dataStructure.c

.PHONY: clean
clean:
	rm -f index.o
	rm -f linkedList.o
	rm -f search.o
	rm -f dataStructure.o
	rm -f index
	rm -f search

