#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char *getBoard(char *path) {
	/**
	 * Gets the nth line of the boards file.
	 */
	char *board;
	size_t len;
	FILE *f = fopen(path, "r");
	
	if(f == NULL) {
		printf("File not found: %s", path);
		exit(1);
	}
	
	getline(&board, &len, f);
	return board;
}

int main() {
	char board[9];
	char file[2] = "00";
	int go = 1;
	char path1[256];
	//char *path1 = (char *) malloc(256);
	char *path2;
	char foo[2] = "\0";
	
	printf("\n========== SUDOKU ==========\n");
	printf("Enter a difficulty level S, E, M, or H (simple, easy, medium, or hard): ");
	while(go) {
		file[0] = getchar();
		switch(file[0]) {
			case 'S': case 's':
			case 'E': case 'e':
			case 'M': case 'm':
			case 'H': case 'h':
				go = 0;
		}
	}
	file[0] = toupper(file[0]);
	printf("Choose a board from 1 to 4: ");
	while(1) {
		file[1] = getchar();
		if(file[1] != '\n' && file[1]-'0' >= 1 && file[1]-'0' <= 4) break;
	}
	
	switch(file[0]) {
		case 'S':
			strcat(path1, "simple");
		case 'E':
			strcat(path1, "easy");
		case 'M':
			strcat(path1, "medium");
		case 'H':
			strcat(path1, "hard");
	}
	
	foo[0] = file[1];
	strcat(path1, foo);
	strcat(path1, ".txt");
	
	path2 = strcat(file, ".txt");
	
	strcpy(board, getBoard(path1));
	
	printf("board is %zd long\n", strlen(board));
	printf("board is %s\n", board);
	/*
	FILE *fp = fopen("subig20.txt", "r");
	char *board = NULL;
	size_t len = 0;
	
	if(fp == NULL) {
		fprintf(stderr, "File not found: subig20.txt\n");
		exit(1);
	}
	
	getline(&board, &len, fp);
	printf("%s\n", board);*/
	
	return 0;
}
