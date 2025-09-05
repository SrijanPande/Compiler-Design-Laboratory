#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main() {
    FILE *file;
    char filename[200];
    int ch;
    int characters = 0, words = 0, lines = 0;
    int inWord = 0;

    printf("Enter the filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename.\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = 0;

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return 1;
    }

    while ((ch = fgetc(file)) != EOF) {
        characters++;

        if (ch == '\n') {
            lines++;
        }

        if (isspace(ch)) {
            if (inWord) {
                words++;
                inWord = 0;
            }
        } else {
            inWord = 1;
        }
    }

    if (inWord) {
        words++;
    }

    fclose(file);

    if (characters > 0) {
        lines++;  
    }
    printf("Characters: %d\nWords: %d\nLines: %d\n", characters, words, lines);

    return 0;
}
