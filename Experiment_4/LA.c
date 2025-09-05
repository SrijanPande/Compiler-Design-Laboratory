#include <stdio.h>
#include <ctype.h>
#include <string.h>

char keywords[32][10] = {"auto","break","case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while"};

int isKeyword(char *str) {
    for(int i = 0; i < 32; i++) {
        if(strcmp(keywords[i], str) == 0)
            return 1;
    }
    return 0;
}

void lexicalAnalyzer(char* code) {
    int i = 0, len = strlen(code);
    while (i < len) {
        if (isspace(code[i])) {
            i++;
        }
        else if (code[i] == '/' && code[i+1] == '/') {
            break; 
        }
        else if (code[i] == '/' && code[i+1] == '*') {
            int found = 0;
            i += 2;
            while (code[i] != '\0') {
                if (code[i] == '*' && code[i+1] == '/') {
                    i += 2;
                    found = 1;
                    break;
                }
                i++;
            }
            if (!found) {
                printf("Error: Comment not closed\n");
                return;
            }
        }
        else if (isalpha(code[i]) || code[i] == '_') {
            char buf[100];
            int k = 0;
            while (isalnum(code[i]) || code[i] == '_')
                buf[k++] = code[i++];
            buf[k] = '\0';
            if (isKeyword(buf))
                printf("Keyword: %s\n", buf);
            else
                printf("Identifier: %s\n", buf);
        }
        else if (isdigit(code[i])) {
            char buf[100];
            int k = 0;
            while (isdigit(code[i]))
                buf[k++] = code[i++];
            buf[k] = '\0';
            printf("Constant: %s\n", buf);
        }
        else if (strchr("+-*/=<>!&|%", code[i])) {
            printf("Operator: %c\n", code[i]);
            i++;
        }
        else if (strchr(";,(){}[]", code[i])) {
            printf("Special Symbol: %c\n", code[i]);
            i++;
        }
        else {
            printf("Unknown Character: %c\n", code[i]);
            i++;
        }
    }
}

int main() {
    char code[256];
    printf("Enter code:\n");
    fgets(code, sizeof(code), stdin);
    lexicalAnalyzer(code);
    return 0;
}

