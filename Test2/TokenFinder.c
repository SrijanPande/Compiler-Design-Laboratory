#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int isKeyword(const char *word);
int isOperator(char c);
int isMultiCharOperator(char c, char next, char *op);
int isPunctuator(char c);
int isSpecialSymbol(char c);
void addIdentifier(const char *name);
void printIdentifierTable();
void printSummary();
void handleStringLiteral(FILE *fp);
void handleCharLiteral(FILE *fp);
void handleConstant(FILE *fp, char first);
void handleIdentifierOrKeyword(const char *word);
void skipComment(FILE *fp, char style);
int fileExists(const char *filePath);
void getUserFilePath(char *filePath);

#define MAX_TOKEN_LEN 100
#define MAX_IDENTIFIERS 100
#define MAX_FILE_PATH 1000

const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};
int keywordCount = sizeof(keywords)/sizeof(keywords[0]);

typedef struct {
    char name[MAX_TOKEN_LEN];
    int count;
} Identifier;

int keywordTotal = 0, identifierTotal = 0, operatorTotal = 0, punctuatorTotal = 0, stringTotal = 0, specialTotal = 0, constantTotal = 0, intConstTotal = 0, floatConstTotal = 0, charConstTotal = 0;
Identifier identifiers[MAX_IDENTIFIERS];
int identifierCount = 0;

int main() {
    char filePath[MAX_FILE_PATH];
    FILE *fp;
    char c, buffer[MAX_TOKEN_LEN];
    
    getUserFilePath(filePath);
    
    fp = fopen(filePath, "r");
    if (!fp) {
        printf("Error: Cannot open file '%s'.\n", filePath);
        return EXIT_FAILURE;
    }

    printf("\nTokens found in %s\n", filePath);
    printf("%s : %s\n", "Token", "Type");
    printf("\n");
    
    while ((c = fgetc(fp)) != EOF) {
        if (isspace(c)) continue;

        if (c == '/') {
            char next = fgetc(fp);
            if (next == '/') { 
                skipComment(fp, '/');
                continue;
            } else if (next == '*') { 
                skipComment(fp, '*');
                continue;
            } else {
                char op[3] = {0};
                if (isMultiCharOperator(c, next, op)) {
                    printf("%s : %s\n", op, "operator");
                    operatorTotal++;
                    continue;
                } else {
                    printf("%c : %s\n", c, "operator");
                    operatorTotal++;
                    ungetc(next, fp);
                    continue;
                }
            }
        }

        if (c == '\"') {
            handleStringLiteral(fp);
            continue;
        }

        if (c == '\'') {
            handleCharLiteral(fp);
            continue;
        }

        if (isalpha(c) || c == '_') {
            int i = 0;
            buffer[i++] = c;
            while ((c = fgetc(fp)), (isalnum(c) || c == '_')) {
                if (i < MAX_TOKEN_LEN - 1)
                    buffer[i++] = c;
            }
            buffer[i] = '\0';
            handleIdentifierOrKeyword(buffer);
            ungetc(c, fp);
            continue;
        }

        if (isdigit(c)) {
            handleConstant(fp, c);
            continue;
        }

        char next = fgetc(fp);
        char op[3] = {0};
        if (isMultiCharOperator(c, next, op)) {
            printf("%s : %s\n", op, "operator");
            operatorTotal++;
            continue;
        }
        ungetc(next, fp);

        if (isOperator(c)) {
            printf("%c : %s\n", c, "operator");
            operatorTotal++;
            continue;
        }

        if (isPunctuator(c)) {
            printf("%c : %s\n", c, "punctuator");
            punctuatorTotal++;
            continue;
        }

        if (isSpecialSymbol(c)) {
            printf("%c : %s\n", c, "special symbol");
            specialTotal++;
            continue;
        }

        printf("%c : %s\n", c, "unknown");
    }
    
    fclose(fp);
    printIdentifierTable();
    printSummary();
    return 0;
}

void getUserFilePath(char *filePath) {
    printf("C LEXICAL ANALYZER\n");
    printf("Enter the complete file path of C source file: ");
    fgets(filePath, MAX_FILE_PATH, stdin);
    
    int len = strlen(filePath);
    if (len > 0 && filePath[len-1] == '\n') {
        filePath[len-1] = '\0';
    }
    
    if (!fileExists(filePath)) {
        printf("Warning: File '%s' may not exist or is not accessible.\n", filePath);
        printf("Continuing anyway...\n\n");
    }
}

int fileExists(const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

int isKeyword(const char *word) {
    for (int i = 0; i < keywordCount; ++i) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '%' || c == '=' ||
            c == '<' || c == '>' || c == '!' || c == '&' || c == '|' ||
            c == '^' || c == '~' || c == '/');
}

int isMultiCharOperator(char c, char next, char *op) {
    if ((c == '=' && next == '=') || (c == '!' && next == '=') ||
        (c == '<' && next == '=') || (c == '>' && next == '=') ||
        (c == '&' && next == '&') || (c == '|' && next == '|') ||
        (c == '+' && next == '+') || (c == '-' && next == '-') ||
        (c == '<' && next == '<') || (c == '>' && next == '>') ||
        (c == '/' && next == '=') || (c == '*' && next == '=') ||
        (c == '+' && next == '=') || (c == '-' && next == '=')) {
        op[0] = c; 
        op[1] = next; 
        op[2] = '\0';
        return 1;
    }
    return 0;
}

int isPunctuator(char c) {
    return (c == ';' || c == ',');
}

int isSpecialSymbol(char c) {
    return (c == '(' || c == ')' || c == '{' || c == '}' || 
            c == '[' || c == ']' || c == '#');
}

void addIdentifier(const char *name) {
    for (int i = 0; i < identifierCount; ++i) {
        if (strcmp(identifiers[i].name, name) == 0) {
            identifiers[i].count++;
            return;
        }
    }
    if (identifierCount < MAX_IDENTIFIERS) {
        strcpy(identifiers[identifierCount].name, name);
        identifiers[identifierCount].count = 1;
        identifierCount++;
    }
}

void printIdentifierTable() {
    printf("\n\nIDENTIFIER FREQUENCY TABLE\n");
    printf("%-20s | %-10s\n", "Identifier", "Count");
    printf("---------------------|----------\n");
    for (int i = 0; i < identifierCount; ++i) {
        printf("%-20s | %-10d\n", identifiers[i].name, identifiers[i].count);
    }
    if (identifierCount == 0) {
        printf("No identifiers found in the source file.\n");
    }
}

void printSummary() {
    int totalTokens = keywordTotal + identifierTotal + operatorTotal +
                      punctuatorTotal + stringTotal + specialTotal + constantTotal;
    
    printf("\n\nTOKEN SUMMARY\n");
    printf("Total Tokens         : %d\n", totalTokens);
    printf("Keywords             : %d\n", keywordTotal);
    printf("Identifiers          : %d\n", identifierTotal);
    printf("Operators            : %d\n", operatorTotal);
    printf("Punctuators          : %d\n", punctuatorTotal);
    printf("Strings              : %d\n", stringTotal);
    printf("Special Symbols      : %d\n", specialTotal);
    printf("Constants (Total)    : %d\n", constantTotal);
    
    printf("\nCONSTANT BREAKDOWN\n");
    printf("Integer constants    : %d\n", intConstTotal);
    printf("Float constants      : %d\n", floatConstTotal);
    printf("Character constants  : %d\n", charConstTotal);
    
    printf("\nANALYSIS COMPLETE\n");
}

void skipComment(FILE *fp, char style) {
    char c1, c2;
    if (style == '/') { 
        while ((c1 = fgetc(fp)) != '\n' && c1 != EOF);
    } else { 
        while ((c1 = fgetc(fp)) != EOF) {
            if (c1 == '*' && (c2 = fgetc(fp)) == '/') {
                break;
            } else {
                ungetc(c2, fp);
            }
        }
    }
}

void handleStringLiteral(FILE *fp) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0, c;
    buffer[i++] = '\"';
    while ((c = fgetc(fp)) != '\"' && c != EOF && c != '\n') {
        if (i < MAX_TOKEN_LEN - 2)
            buffer[i++] = c;
    }
    if (c == '\"')
        buffer[i++] = '\"';
    buffer[i] = '\0';
    printf("%s : %s\n", buffer, "string");
    stringTotal++;
}

void handleCharLiteral(FILE *fp) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0, c;
    buffer[i++] = '\'';
    while ((c = fgetc(fp)) != '\'' && c != EOF && c != '\n') {
        if (i < MAX_TOKEN_LEN - 2)
            buffer[i++] = c;
    }
    if (c == '\'')
        buffer[i++] = '\'';
    buffer[i] = '\0';
    printf("%s : %s\n", buffer, "char constant");
    constantTotal++;
    charConstTotal++;
}

void handleConstant(FILE *fp, char first) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0, c, isFloat = 0;
    buffer[i++] = first;
    while ((c = fgetc(fp)) != EOF && (isdigit(c) || c == '.')) {
        if (c == '.') {
            if (isFloat) break; 
            isFloat = 1;
        }
        if (i < MAX_TOKEN_LEN - 1)
            buffer[i++] = c;
    }
    buffer[i] = '\0';
    ungetc(c, fp);
    
    if (isFloat) {
        printf("%s : %s\n", buffer, "float constant");
        floatConstTotal++;
    } else {
        printf("%s : %s\n", buffer, "integer constant");
        intConstTotal++;
    }
    constantTotal++;
}

void handleIdentifierOrKeyword(const char *word) {
    if (isKeyword(word)) {
        printf("%s : %s\n", word, "keyword");
        keywordTotal++;
    } else {
        printf("%s : %s\n", word, "identifier");
        addIdentifier(word);
        identifierTotal++;
    }
}