main: ./socket/handlesocket.c webserv.c ./systeminfo/systemstats.c
	cc -o main -fsanitize=undefined ./socket/handlesocket.c webserv.c ./systeminfo/systemstats.c
asm: ./socket/handlesocket.c webserv.c ./systeminfo/systemstats.c
	cc -S ./socket/handlesocket.c webserv.c ./systeminfo/systemstats.c  -fverbose-asm
clean:
	rm -f main
clean-asm:
	rm -f main handlesocket.s webserv.s systemstats.s