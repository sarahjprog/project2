#include "headers.h"
#define MAX 30
#define MAX_MRECS 1024

int pass2(FILE *objectc, FILE *file, SYMTAB MySymbolTable, int proglength)
{

        char line[1024], label[40], opcode[40], operand[1024];
        char object[1024][256], textRec[2048], temp[2048]="", StartLabel[30];
        int Location_Counter=0, textStart=0, textLength=0, StartAddr=0, executionAddr=-1, instsize=3,currentAddr=0, sourceline =1, Maddr=0;
        int MCount =0;
        textRec[0] = '\0';
        int formatcheck=0, hexlen=0;


        if(fgets(line, sizeof(line), file)) {
                label[0]=opcode[0]=operand[0]='\0';
                int num =sscanf(line, "%X %s %s %[^\n]", &Location_Counter, label, opcode, operand);
                convertToUpper(opcode);
                if (num <3 || strcmp(opcode, "START")!=0) {
                        printf("ERRor START directive missing \n");
                        return -1;
                }
                StartAddr=(int)strtol(operand, NULL, 16);
                executionAddr=StartAddr;
                strcpy(StartLabel, label);
                fprintf(objectc,"H%-6s%06X%06X\n", StartLabel, StartAddr, proglength);
                textStart= Location_Counter=StartAddr;
        }
        while (fgets(line, sizeof(line), file)) {
                label[0]=opcode[0]=operand[0]='\0';
                sscanf(line, "%X %s %s %[^\n]", &Location_Counter, label, opcode, operand);
                convertToUpper(opcode);
                currentAddr =Location_Counter;

                if (strcmp(opcode, "BYTE") == 0) {
                        temp[0] = '\0';
                        hexlen = 0;

                        if (operand[0]=='C' && operand[1] == '\'' ) {
                                int i=2;
                                while(operand[i] != '\0' && operand[i] != '\'') {
                                        unsigned char ascii = EBCDIC_to_ASCII[(unsigned char)operand[i]];
                                        sprintf(temp+hexlen, "%02X", ascii);
                                        hexlen+=2;
                                        i++;
                                }

                        } else if (operand[0]=='X' && operand[1] == '\'' ) {
                                int i=2;
                                while(operand[i] != '\0' && operand[i] != '\'') {
                                        temp[hexlen++]=operand[i];
                                        i++;
                                }
                                temp[hexlen]='\0';
                        }

                        appendObject(textRec, &textLength, &textStart, objectc, currentAddr, temp, hexlen/2);
                        continue;
                }

                if (strcmp(opcode, "WORD")==0) {
                        if (operand[0]== '\0') continue;
                        long conversion=strtol(operand, NULL,10);
                        char objtmp[16];
                        sprintf(objtmp, "%06X", (unsigned)(conversion & 0xFFFFFF));
                        appendObject(textRec, &textLength, &textStart, objectc, currentAddr, objtmp, 3);
                        continue;
                }
                else if (strcmp(opcode, "RESB")==0 || strcmp(opcode, "RESW")==0 ) {
                        flushTextRec(objectc, textStart, textRec, textLength);
                        textRec[0] = '\0';
                        textLength = 0;
                        continue;
                }

                else if (strcmp(opcode, "END") == 0) {

                        if (operand[0] != '\0') {
                                int addr = checkForOperand(MySymbolTable, operand);
                                if (addr == -1) {
                                        printf("ASSEMBLY ERROR: Undefined symbol '%s'\n", operand);
                                        return -1;
                                }
                                executionAddr = addr;
                        }

                        flushTextRec(objectc, textStart, textRec, textLength);

                        for (int i = 0; i < MCount; i++) {
                                fprintf(objectc, "%s", object[i]);
                        }

                        fprintf(objectc, "E%06X\n", executionAddr);
                        break;
                }

                char *opval = getOpcodeValue(label);
                if (opval != NULL && getOpcodeValue(opcode) == NULL) {
                        strcpy(operand, opcode);
                        strcpy(opcode, label);
                        label[0] = '\0';
                }

                char * opvalue=getOpcodeValue(opcode);
                Maddr=currentAddr+1;

                formatcheck=checkForFormat(opcode);
                int operandaddrwithcomma=stringwithacomma(operand);
                int offsetbits= offsetbit( Location_Counter);
                   int operandaddr= checkForOperand(MySymbolTable, operand);
                if (formatcheck!= -1) {
                        if(opvalue!= NULL) {
                                int opcode_val = (int)strtol(opvalue, NULL, 16);

                                if (operand[0] != '\0') {
                                        if (operandaddr == -1 && !operandaddrwithcomma) {
                                                printf("ASSEMBLY ERROR: Undefined symbol '%s'\n", operand);
                                                return -1;
                                        }
                                }

                                 if(operandaddrwithcomma == 1) {
                                     char operandAssign[256];
                                     strcpy(operandAssign, operand);
                                     char *comma=strchr(operandAssign, ',');
                                     if (comma) *comma ='\0';

                                     int baseAddr=checkForOperand(MySymbolTable, operandAssign);
                                     if (baseAddr == -1){
                                         printf("ASSEMBLY ERROR: Undefined symbol '%s'\n", operandAssign);
                                                return -1;
                                        }
                                         baseAddr |= 0x8000;
                                         unsigned int objcode= ((opcode_val & 0xFF) << 16) | (baseAddr& 0xFFFF);

                                         char objtmp[16];
                                         sprintf(objtmp, "%06X", objcode);
                                         appendObject(textRec, &textLength, &textStart, objectc, currentAddr, objtmp, 3);

                                }
                                else if(operandaddr!=-1) {
                                        char objtmp[16];
                                        sprintf(objtmp, "%02X%04X", opcode_val, operandaddr);
                                        appendObject(textRec, &textLength, &textStart, objectc, currentAddr, objtmp, 3);

                                }
                                else {
                                        char objtmp[16];
                                        sprintf(objtmp, "%02X0000", opcode_val);
                                        appendObject(textRec, &textLength, &textStart, objectc, currentAddr, objtmp, 3);
                                }

                        }
                     if (formatcheck == 3 && operand[0] != '\0') {
    char operandSymbol[256];
    strcpy(operandSymbol, operand);
    char *comma = strchr(operandSymbol, ',');
    if (comma) *comma = '\0';

    int relocAddr = currentAddr + 1;
    sprintf(object[MCount], "M%06X04+%s\n", relocAddr, StartLabel);
    MCount++;
}

                }
                if (!opvalue ) {

                        printf("ASSEMBLY ERROR: Invalid opcode '%s'\n", opcode);

                        return -1;

                }

        }
        return 0;
}
