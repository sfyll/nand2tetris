#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 10000

char ***loadFile(const char *filePath, int *lineCount) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char ***lines = NULL; // List of list of words
    char buffer[MAX_LINE_SIZE];
    *lineCount = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Resize lines to accommodate the new lne
        lines = realloc(lines, (*lineCount + 1) * sizeof(char **));
        lines[*lineCount] = NULL;  // Initialize to NULL for later use with realloc

        char *token;
        int wordCount = 0;

        token = strtok(buffer, " \n");
        while (token != NULL) {
            // Resize the current line to accommodate the new word
            lines[*lineCount] = realloc(lines[*lineCount], (wordCount + 1) * sizeof(char *));
            lines[*lineCount][wordCount] = strdup(token);
            wordCount++;
            token = strtok(NULL, " \n");
        }

        (*lineCount)++;
    }

    fclose(file);
    return lines;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    int lineCount = 0;
    char ***lines = loadFile(argv[1], &lineCount);

    return 0;
}
