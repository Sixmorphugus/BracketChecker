#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// helpers
char* readFileToEnd(FILE* fl) {
	char* fileString = (char*) malloc(sizeof(char)); // allocate 1 char to start with
	
	int numChars = 0;
	
	do {
		char fileChar = fgetc(fl);
		numChars++;
		
		fileString = (char*) realloc(fileString, sizeof(char) * numChars);
		
		fileString[numChars-1] = fileChar; 
	}
	while(!feof(fl));
	
	printf("%i characters\n", numChars);
	
	fileString = (char*) realloc(fileString, sizeof(char) * numChars+1);
	fileString[numChars] = '\0';
	
	return fileString;
}

// program
int main(int argc, char** argv) {
	if(argc < 2) {
		printf("Please provide a file for me to scan.\n\ti.e. \"SyntaxCheck ./script.c\"\n");
		return 0;
	}
	
	printf("Scanning %s\n", argv[1]);
	
	FILE* file = fopen(argv[1], "r");
	
	if(!file) {
		printf("I couldn't open that file. Are you sure that's right?\n");
		return 0;
	}
	
	int bracketTallyO = 0;
	int bracketTallyC = 0;
	
	int braceTallyO = 0;
	int braceTallyC = 0;
	
	int boxTallyO = 0;
	int boxTallyC = 0;
	
	char curChar = 0;
	
	char* fileLines = readFileToEnd(file);
	fclose(file);
	
	printf("Formatted file:\n---------------\n");
	
	// go through the string and print it out formatted
	int i;
	
	int indentLevel = 1;
	int lineHasCharacters = 0;
	
	for(i = 0; i < strlen(fileLines); i++) {
		char curChar = fileLines[i];
		
		int curCharIsWhitespace = 0;
		if(curChar == ' ' || curChar == '\t') {
			curCharIsWhitespace = 1;
		}
		
		int curCharMustBeAlone = 0;
		
		int curCharMeansOneLessIndent = 0;
		
		if(!curCharIsWhitespace || lineHasCharacters) {
			if(!lineHasCharacters) {
				lineHasCharacters = 1;
				
				// insert our indents now
				int j;
				for(j = 0; j < indentLevel; j++) {
					printf("  ");
				}
			}
			
			printf("%c", curChar);
			
			if(curChar == '\n') {
				// reset
				lineHasCharacters = 0;
			}
		}
	}
	
	printf("\n---------------\n");
	
	printf("Brackets closed: %i/%i (%s)\n", bracketTallyC, bracketTallyO, bracketTallyC == bracketTallyO ? "ok" : "mismatch!");
	printf("Braces closed: %i/%i (%s)\n", braceTallyC, braceTallyO, braceTallyC == braceTallyO ? "ok" : "mismatch!");
	printf("Boxes closed: %i/%i (%s)\n", boxTallyC, boxTallyO, boxTallyC == boxTallyO ? "ok" : "mismatch!");
	
	free(fileLines);
}
