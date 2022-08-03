all: src/*
	gcc -o main src/*

client: src/main.c src/main.h src/tcp_client.c src/tcp_client.h
	gcc -o client src/main.c src/tcp_client.c

server: src/main.c src/main.h src/tcp_server.c src/tcp_server.h
	gcc -o server src/main.c src/tcp_server.c
