main: ./socket/handlesocket.c webserv.c ./systeminfo/systemstats.c
	cc -o log-webserv -fsanitize=undefined ./socket/handlesocket.c webserv.c ./systeminfo/systemstats.c
asm: ./socket/handlesocket.c webserv.c ./systeminfo/systemstats.c
	cc -S ./socket/handlesocket.c webserv.c ./systeminfo/systemstats.c  -fverbose-asm
clean:
	rm -f log-webserv
clean-asm:
	rm -f log-webserv handlesocket.s webserv.s systemstats.s