all: src/*
	gcc -o main src/*

server: src/main.c src/http_server.c src/http_server.h
	gcc -o server src/main.c src/http_server.c
