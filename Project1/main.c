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
	
	int indentLevel = 0;
	int lineHasCharacters = 0;

	int newLine = 1;
	int newLineNeeded = 0;

	char nestings[200];
	int nestingInt = 0;

	int nestingMismatches = 0;

	for(i = 0; i < strlen(fileLines); i++) {
		char curChar = fileLines[i];
		
		if (curChar == '\n') {
			printf("\n");
			newLine = 1;
			//newLineNeeded = 0;
		}
		else if (curChar == ' ' || curChar == '\t') {
			if(!newLine)
				printf("%c", curChar);
		}
		else if (curChar == '(' || curChar == '{' || curChar == '[') {
			if (newLineNeeded) {
				printf("\n");
				newLine = 1;
				newLineNeeded = 0;
			}

			indentLevel++;

			// count
			switch (curChar) {
			case '(':
				bracketTallyO++;
				break;
			case '[':
				boxTallyO++;
				break;
			case '{':
				braceTallyO++;
				break;
			}

			nestings[nestingInt] = curChar;
			nestingInt++;

			if (!newLine) {
				printf("\n");
				newLine = 1;
			}

			int i;
			for (i = 0; i < indentLevel - 1; i++) {
				printf("  ");
			}

			printf("%c", curChar);

			newLineNeeded = 1;
		}
		else if (curChar == ')' || curChar == '}' || curChar == ']') {
			if (newLineNeeded) {
				printf("\n");
				newLine = 1;
				newLineNeeded = 0;
			}

			indentLevel--;

			int itsAMismatch = 0;

			// count
			switch (curChar) {
			case ')':
				bracketTallyC++;

				// nesting mismatch
				if (nestingInt == 0 || nestings[nestingInt - 1] != '(') {
					nestingMismatches++;
					itsAMismatch = 1;
				}

				break;
			case ']':
				boxTallyC++;

				// nesting mismatch
				if (nestingInt == 0 || nestings[nestingInt - 1] != '[') {
					nestingMismatches++;
					itsAMismatch = 1;
				}

				break;
			case '}':
				braceTallyC++;

				// nesting mismatch
				if (nestingInt == 0 || nestings[nestingInt - 1] != '{') {
					nestingMismatches++;
					itsAMismatch = 1;
				}

				break;
			}

			if(nestingInt > 0)
				nestingInt--;

			if (!newLine) {
				printf("\n");
				newLine = 1;
			}

			int i;
			for (i = 0; i < indentLevel; i++) {
				printf("  ");
			}

			printf("%c", curChar);

			if (itsAMismatch) {
				printf(" <-- !!");
			}

			newLineNeeded = 1;
		}
		else {
			if (newLineNeeded) {
				printf("\n");
				newLine = 1;
				newLineNeeded = 0;
			}

			if (newLine) {
				int i;
				for (i = 0; i < indentLevel; i++) {
					printf("  ");
				}

				newLine = 0;
			}

			printf("%c", curChar);
		}
	}
	
	printf("\n---------------\n");
	
	printf("Brackets closed: %i/%i (%s)\n", bracketTallyC, bracketTallyO, bracketTallyC == bracketTallyO ? "ok" : "mismatch!");
	printf("Braces closed: %i/%i (%s)\n", braceTallyC, braceTallyO, braceTallyC == braceTallyO ? "ok" : "mismatch!");
	printf("Boxes closed: %i/%i (%s)\n", boxTallyC, boxTallyO, boxTallyC == boxTallyO ? "ok" : "mismatch!");
	
	if (nestingMismatches) {
		printf("%i Nesting mismatches detected.\n", nestingMismatches);
	}

	free(fileLines);
}
