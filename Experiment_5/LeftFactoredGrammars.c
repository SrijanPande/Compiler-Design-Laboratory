#include <stdio.h>
#include <string.h>

#define MAX_PROD 20
#define MAX_LEN 100
#define MAX_NEW_NT 26

typedef struct {
    char nt;
    char productions[MAX_PROD][MAX_LEN];
    int prodCount;
} NewNonTerminal;

int newNTIndex = 0;
NewNonTerminal newNTs[MAX_NEW_NT];

void commonPrefixTwo(char *str1, char *str2, char *prefix) {
    int i = 0;
    while (str1[i] && str2[i] && str1[i] == str2[i]) {
        i++;
    }
    strncpy(prefix, str1, i);
    prefix[i] = '\0';
}

void longestCommonPrefixGroup(char arr[][MAX_LEN], int count, char *prefix) {
    if (count == 0) {
        prefix[0] = '\0';
        return;
    }
    strcpy(prefix, arr[0]);
    for (int i = 1; i < count; i++) {
        char temp[MAX_LEN];
        commonPrefixTwo(prefix, arr[i], temp);
        strcpy(prefix, temp);
        if (strlen(prefix) == 0) break;
    }
}

void addNewNT(NewNonTerminal nt) {
    if (newNTIndex < MAX_NEW_NT) {
        newNTs[newNTIndex++] = nt;
    }
}

void leftFactor(char nt, char productions[][MAX_LEN], int n) {
    int processed[MAX_PROD] = {0};
    int firstAlternative = 1;

    printf("%c -> ", nt);

    for (int i = 0; i < n; i++) {
        if (processed[i]) continue;

        char group[MAX_PROD][MAX_LEN];
        int groupCount = 0;

        strcpy(group[groupCount++], productions[i]);
        processed[i] = 1;

        for (int j = i + 1; j < n; j++) {
            if (!processed[j]) {
                char prefix[MAX_LEN];
                commonPrefixTwo(productions[i], productions[j], prefix);
                if (strlen(prefix) > 0) {
                    strcpy(group[groupCount++], productions[j]);
                    processed[j] = 1;
                }
            }
        }

        if (!firstAlternative) printf(" | ");

        if (groupCount == 1) {
            printf("%s", group[0]);
        } else {
            char prefix[MAX_LEN];
            longestCommonPrefixGroup(group, groupCount, prefix);
            int lenPrefix = strlen(prefix);

            printf("%s", prefix);
            char newNT = 'A' + newNTIndex;
            printf("%c", newNT);

            NewNonTerminal newNTobj;
            newNTobj.nt = newNT;
            newNTobj.prodCount = 0;

            for (int k = 0; k < groupCount; k++) {
                if ((int)strlen(group[k]) == lenPrefix) {
                    strcpy(newNTobj.productions[newNTobj.prodCount++], "#");
                } else {
                    strcpy(newNTobj.productions[newNTobj.prodCount++], group[k] + lenPrefix);
                }
            }

            addNewNT(newNTobj);
        }

        firstAlternative = 0;
    }

    printf("\n");
}

int main() {
    char nonTerminal;
    int n;
    newNTIndex = 0;

    printf("Enter the non-terminal symbol: ");
    scanf(" %c", &nonTerminal);

    printf("Enter the number of productions: ");
    scanf("%d", &n);

    char productions[MAX_PROD][MAX_LEN];

    printf("Enter the productions (one per line): \n");
    for (int i = 0; i < n; i++) {
        scanf("%s", productions[i]);
    }

    printf("\nLeft Factored Grammar:\n");
    leftFactor(nonTerminal, productions, n);

    for (int i = 0; i < newNTIndex; i++) {
        leftFactor(newNTs[i].nt, newNTs[i].productions, newNTs[i].prodCount);
    }

    return 0;
}
