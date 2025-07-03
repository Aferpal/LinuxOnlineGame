

server: 
	gcc src/server.c src/serverMain.c -o out/mainserver -lpthread

client:
	gcc src/clientMain.c -o out/clientserver

run_server:
	./out/mainserver

run_client:
	./out/mainclient

