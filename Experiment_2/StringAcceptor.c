#include <stdio.h>
#include <string.h>

int nstates, ninputs;
int dfa[10][10];
char input_symbols[10];
int final_states[10], nfinals;
int start_state;

int get_input_symbol_index(char c) {
    for (int i = 0; i < ninputs; i++) {
        if (input_symbols[i] == c)
            return i;
    }
    return -1;
}

int main() {
    printf("Enter number of states: ");
    scanf("%d", &nstates);

    printf("Enter number of input symbols: ");
    scanf("%d", &ninputs);

    printf("Enter input symbols: ");
    for (int i = 0; i < ninputs; i++) {
        scanf(" %c", &input_symbols[i]);
    }

    printf("Enter start state (0 to %d): ", nstates - 1);
    scanf("%d", &start_state);

    printf("Enter number of final states: ");
    scanf("%d", &nfinals);

    printf("Enter final states: ");
    for (int i = 0; i < nfinals; i++) {
        scanf("%d", &final_states[i]);
    }

    printf("Define transition table\n");
    for (int i = 0; i < nstates; i++) {
        for (int j = 0; j < ninputs; j++) {
            printf("(q%d, %c) = ", i, input_symbols[j]);
            scanf("%d", &dfa[i][j]);
        }
    }

    char test_string[100];
    printf("Enter string to test: ");
    scanf("%s", test_string);

    int current_state = start_state;
    int i, symbol_index;

    for (i = 0; i < strlen(test_string); i++) {
        symbol_index = get_input_symbol_index(test_string[i]);
        if (symbol_index == -1) {
            printf("Invalid input symbol '%c'.\n", test_string[i]);
            return 1;
        }
        current_state = dfa[current_state][symbol_index];
        if (current_state == -1) {
            // Invalid transition
            break;
        }
    }

    int accepted = 0;
    for (int k = 0; k < nfinals; k++) {
        if (current_state == final_states[k]) {
            accepted = 1;
            break;
        }
    }

    if (accepted) {
        printf("String is accepted by the DFA.\n");
    } else {
        printf("String is rejected by the DFA.\n");
    }

    return 0;
}
