

server: 
	gcc src/server.c src/serverMain.c -o out/mainserver -lpthread

client:
	gcc src/client.c src/clientMain.c -o out/mainclient

run_server:
	./out/mainserver

run_client:
	./out/mainclient

