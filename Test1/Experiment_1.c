#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define q0 0
#define q1 1
#define qf 2
#define InputSize 100

int transitionTable[3][2] = {
    {q1, q1}, 
    {qf, qf},  
    {qf, qf} 
};

bool acceptedByDFA(char *userString) {
    if(strlen(userString)==0){
        return false;
    }
    int currentState = q0;
    for (int i = 0; userString[i] != '\0'; i++) {
        if (userString[i] != '0' && userString[i] != '1')
            return false; 

        int symbol = (userString[i] == '0') ? 0 : 1;
        currentState = transitionTable[currentState][symbol];
    }
    return (currentState == qf);
}

int main() {
    char testString[InputSize];
    while(true){
        puts("Enter a string (type end to exit the program) :");
        scanf("%s", testString);
        if (strcmp(testString, "end") == 0) {
            break;
        }
        if (acceptedByDFA(testString)) {
            puts("accepted");
        } else {
            puts("rejected");
        }
    }
    return 0;
}