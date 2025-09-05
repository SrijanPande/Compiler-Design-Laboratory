#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 100

const char *keywords[] = {
    "auto","break","case","char","const","continue","default","do","double",
    "else","enum","extern","float","for","goto","if","int","long","register",
    "return","short","signed","sizeof","static","struct","switch","typedef",
    "union","unsigned","void","volatile","while"
};
int is_keyword(const char *str) {
    for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++)
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    return 0;
}
int is_operator(char ch) {
    return strchr("+-*/%=<>&|^!~", ch) != NULL;
}
int is_special_symbol(char ch) {
    return strchr(";,(){}[]#", ch) != NULL;
}
void print_token(const char *token) {
    if (strlen(token) > 0)
        printf("Token: %s\n", token);
}
int main() {
    char input[256];
    printf("Enter a C/C++ statement:\n");
    fgets(input, sizeof(input), stdin);

    int i = 0, len = strlen(input);
    char token[MAX_TOKEN_LEN];
    int t = 0;

    printf("Tokens in the program:\n");
    while (i < len) {
        if (isspace(input[i])) {
            token[t] = '\0';
            print_token(token);
            t = 0;
            i++;
        } else if (isalpha(input[i]) || input[i] == '_') {
            token[t++] = input[i++];
            while (isalnum(input[i]) || input[i] == '_')
                token[t++] = input[i++];
            token[t] = '\0';
            print_token(token);
            t = 0;
        } else if (isdigit(input[i])) {
            token[t++] = input[i++];
            while (isdigit(input[i]))
                token[t++] = input[i++];
            token[t] = '\0';
            print_token(token);
            t = 0;
        } else if (is_operator(input[i])) {
            token[0] = input[i++];
            token[1] = '\0';
            print_token(token);
            t = 0;
        } else if (is_special_symbol(input[i])) {
            token[0] = input[i++];
            token[1] = '\0';
            print_token(token);
            t = 0;
        } else {
            i++;
        }
    }
    return 0;
}