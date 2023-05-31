main: handlesocket.c webserv.c
	cc -o main handlesocket.c webserv.c systemstats.c
asm: handlesocket.c webserv.c
	cc -S handlesocket.c webserv.c  -fverbose-asm
clean:
	rm -f main
clean-asm:
	rm -f main handlesocket.s webserv.s