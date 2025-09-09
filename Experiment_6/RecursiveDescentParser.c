#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXIMUM_SIZE 1000

const char *input;
int pos = 0;

char peek() {
    return input[pos];
}

void consume(char c) {
    if (peek() == c) {
        pos++;
    } else {
        printf("Error: Expected '%c' but found '%c'\n", c, peek());
        exit(1);
    }
}

void E();
void Edash();
void T();
void Tdash();
void F();

void E() {
    T();
    Edash();
}

void Edash() {
    if (peek() == '+') {
        consume('+');
        T();
        Edash();
    }
}

void T() {
    F();
    Tdash();
}

void Tdash() {
    if (peek() == '*') {
        consume('*');
        F();
        Tdash();
    }
}

void F() {
    if (peek() == '(') {
        consume('(');
        E();
        consume(')');
    } else if (isalpha(peek())) {
        consume(peek());  
    } else {
        printf("Error: Unexpected character '%c' in factor\n", peek());
        exit(1);
    }
}

void parse() {
    E();
    if (input[pos] == '\0') {
        printf("Expression is valid!\n");
    } else {
        printf("Error: Unexpected characters after parsing\n");
        exit(1);
    }
}

int main() {
    char expr[MAXIMUM_SIZE];
    printf("Enter the expression: ");
    scanf("%s", expr);

    input = expr;
    parse();

    return 0;
}
