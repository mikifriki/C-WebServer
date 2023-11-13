main: ./socket/handlesocket.c webserv.c ./systeminfo/systemstats.c
	cc -o log-webserer -fsanitize=undefined ./socket/handlesocket.c webserv.c ./systeminfo/systemstats.c
asm: ./socket/handlesocket.c webserv.c ./systeminfo/systemstats.c
	cc -S ./socket/handlesocket.c webserv.c ./systeminfo/systemstats.c  -fverbose-asm
clean:
	rm -f log-webserer
clean-asm:
	rm -f log-webserer handlesocket.s webserv.s systemstats.s