serv:  serv.c list/list.c 
	gcc serv.c list/list.c -lpthread -Wformat -Wall -o server

cli:  cli.c
	gcc cli.c -lpthread -Wformat -Wall -o client
