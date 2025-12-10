#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SYMTAB InsertSymbol(SYMTAB table, char name[7], int addr, int srcline) {
    SYMBOL *new = malloc(sizeof(SYMBOL));
    if (new == NULL) {
        printf("Memory allocation failed for new symbol.\n");
        exit(1);
    }
    memset(new, 0, sizeof(SYMBOL));
    strcpy(new->name, name);
    new->address = addr;
    new->sourceline = srcline;
    new->next = table;
    table = new;
    return table;
}

int SymbolExists(SYMTAB table, char *name) {
    SYMTAB temp = table;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

void converttoO(char *opcode, char *operand, char *objectCode) {
    if (!opcode || !objectCode) return;
    objectCode[0] = '\0';
    if (!operand) return;

    if (strcmp(opcode, "WORD") == 0) {
        int value = atoi(operand);
        sprintf(objectCode, "%06X", value & 0xFFFFFF);
    }
    else if (strcmp(opcode, "BYTE") == 0) {
        if (operand[0] == 'C' && operand[1] == '\'') {
            char temp[256] = "";
            for (int i = 2; operand[i] && operand[i] != '\''; i++) {
                char hexval[3];
                sprintf(hexval, "%02X", (unsigned char)operand[i]);
                strcat(temp, hexval);
            }
            strcpy(objectCode, temp);
        }
        else if (operand[0] == 'X' && operand[1] == '\'') {
            int len = strlen(operand);
            if (len > 3) {
                int hexlen = len - 3;
                strncpy(objectCode, operand + 2, hexlen);
                objectCode[hexlen] = '\0';
            }
        }
    }
}
