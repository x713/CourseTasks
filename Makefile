all: hzip

hzip: hzip.o bitbuffer.o huff_tree.o 
	g++ hzip.o bitbuffer.o huff_tree.o -o hzip

main.o: hzip.c
	g++ -c hzip.c

bitbuffer.o: bitbuffer.c
	g++ -c bitbuffer.c
	
huff_tree.o: huff_tree.c
	g++ -c huff_tree.c

clean:
	rm -rf *.o hzip