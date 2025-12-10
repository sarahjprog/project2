struct opcode directive[]= {{"ADD",3}, {"ADDF",3}, {"ADDR",2}, {"AND", 3},
        {"CLEAR", 2}, {"COMP",3}, {"COMPF", 3}, {"COMPR", 2},
        {"DIV",3}, {"DIVF",3}, {"FIX", 1}, {"FLOAT", 1}, {"HIO",1},
        {"J",3},{"JET",3},{"JEQ",3}, {"JGT",3}, {"JLT",3}, {"JSUB", 3},
        {"LDA", 3}, {"LDB",3}, {"LDCH", 3}, {"LDF", 3}, {"LDL", 3},
        {"LDS", 3}, {"LDT", 3}, {"LDX", 3}, {"LPS", 3},
        {"MUL", 3}, {"MULF", 3}, {"MULR", 2}, {"OR", 3}, {"RD", 3},
        {"RMO", 2}, {"RSUB", 3}, {"SHIFTL", 2},
        {"SHIFTR", 2}, {"SSK",3}, {"STA",3}, {"STB",3},
        {"STCH",3}, {"STF",3}, {"STI",3}, {"STL",3},
        {"STS",3}, {"STSW",3}, {"STT",3}, {"STX",3},
        {"SUB", 3}, {"SUBF", 3}, {"SUBFR",2}, {"SVC",2},
        {"TD",3}, {"TIO",1}, {"TIX",3}, {"TIXR",2},{"WD",3}
};
struct opcodeTable OPTAB[]= {{"ADD", "18"}, {"AND", "40"}, {"COMP", "28"}, {"DIV", "24"},
        {"J", "3C"}, {"JEQ", "30"}, {"JGT", "34"}, {"JLT", "38"},
        {"JSUB", "48"}, {"LDA", "00"}, {"LDCH", "50"}, {"LDL", "08"},
        {"LDX", "04"}, {"MUL", "20"}, {"OR", "44"}, {"RD", "D8"},
        {"RSUB", "4C"}, {"STA", "0C"}, {"STCH", "54"}, {"STL", "14"},
        {"STX", "10"}, {"SUB", "1C"}, {"TD", "E0"}, {"TIX", "2C"},
        {"WD", "DC"}
};
unsigned char EBCDIC_to_ASCII[256] = {
    0x00,0x01,0x02,0x03,0x9C,0x09,0x86,0x7F,
    0x97,0x8D,0x8E,0x0B,0x0C,0x0D,0x0E,0x0F,
    0x10,0x11,0x12,0x13,0x9D,0x0A,0x08,0x87,
    0x18,0x19,0x92,0x8F,0x1C,0x1D,0x1E,0x1F,
    0x80,0x81,0x82,0x83,0x84,0x0A,0x17,0x1B,
    0x88,0x89,0x8A,0x8B,0x8C,0x05,0x06,0x07,
    0x90,0x91,0x16,0x93,0x94,0x95,0x96,0x04,
    0x98,0x99,0x9A,0x9B,0x14,0x15,0x9E,0x1A,
    0x20,0xA0,0xE2,0xE4,0xE0,0xE1,0xE3,0xE5,
    0xE7,0xF1,0xA2,0x2E,0x3C,0x28,0x2B,0x7C,
    0x26,0xE9,0xEA,0xEB,0xE8,0xED,0xEE,0xEF,
    0xEC,0xDF,0x21,0x24,0x2A,0x29,0x3B,0xAC,
    0x2D,0x2F,0xC2,0xC4,0xC0,0xC1,0xC3,0xC5,
    0xC7,0xD1,0xA6,0x2C,0x25,0x5F,0x3E,0x3F,
    0xF8,0xC9,0xCA,0xCB,0xC8,0xCD,0xCE,0xCF,
    0xCC,0x60,0x3A,0x23,0x40,0x27,0x3D,0x22,
    0xD8,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
    0x68,0x69,0xAB,0xBB,0xF0,0xFD,0xFE,0xB1,
    0xB0,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,
    0x71,0x72,0xAA,0xBA,0xE6,0xB8,0xC6,0xA4,
    0xB5,0x7E,0x73,0x74,0x75,0x76,0x77,0x78,
    0x79,0x7A,0xA1,0xBF,0xD0,0x5B,0xDE,0xAE,
    0xAC,0xA3,0xA5,0xB7,0xA9,0xA7,0xB6,0xBC,
    0xBD,0xBE,0xDD,0xA8,0xAF,0x5D,0xB4,0xD7,
    0x7B,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
    0x48,0x49,0xAD,0xF4,0xF6,0xF2,0xF3,0xF5,
    0x7D,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,
    0x51,0x52,0xB9,0xFB,0xFC,0xF9,0xFA,0xFF,
    0x5C,0xF7,0x53,0x54,0x55,0x56,0x57,0x58,
    0x59,0x5A,0xB2,0xD4,0xD6,0xD2,0xD3,0xD5,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
    0x38,0x39,0xB3,0xDB,0xDC,0xD9,0xDA,0x9F
};


int OptabLength= sizeof(OPTAB)/sizeof(OPTAB[0]);
char *getOpcodeValue(char *opcode) {

        for (int i = 0; i < OptabLength; i++) {
                if (strcmp(opcode, OPTAB[i].mnemonic) == 0) {
                        return OPTAB[i].code;
                }
        }
        return NULL; // not found

}
int DirectiveLength = sizeof(directive)/ sizeof(directive[0]);

int isDirective(const char *opcode){
const char *directives[]={"START", "BYTE", "RESB", "END", "RESW", "WORD"};
int length= sizeof(directives)/sizeof(directives[0]);
for (int i=0; i<length; i++){
 if (strcmp(directives[i], opcode)==0) return 1;
}
return 0;
}


int checkForFormat(char *opcode) {


        for (int j=0; j<DirectiveLength; j++) {
                if (strcmp(opcode, directive[j].mnemonic) ==0) {
                        return directive[j].Format;
                }
        }
        return -1;
}

void convertToUpper(char *word) {
        for (int i=0; word[i] != '\0'; i++) {
                word[i]= toupper((unsigned char)word[i]);
        }
}

long Convert(const char *operand,int base, int lineNumber, const char*opcode) {
        char *ptr;
        long convert_base=strtol(operand,&ptr,base);
        if(*ptr != '\0') {
                printf("ASSEMBLY ERROR: Line %d: Invalid  constant '%s'\n", lineNumber, operand);
                exit(-1);
        }
        return convert_base;
}
int checkForOperand(SYMTAB MysymbolTable, char *operand) {

        SYMBOL *current= MysymbolTable;

        while (current !=NULL) {
                if (strcmp (current->name, operand) ==0) {
                        return current->address;
                }
                current = current->next;

        }
        return -1;
}

void flushTextRec(FILE *objectc, int textStart, char *textRec, int textLength) {
    if (textLength > 0) {
        fprintf(objectc, "T%06X%02X%s\n", textStart, textLength, textRec);
    }
}

void appendObject(char *textRec, int *textLength, int *textStart,
                  FILE *objectc, int currentAddr,
                  const char *obj, int objLength)
{
    for (int i = 0; i < objLength; i++) {

        char byteHex[3];
        byteHex[0] = obj[i*2];
        byteHex[1] = obj[i*2 + 1];
        byteHex[2] = '\0';

        if (*textLength == 0) {
            *textStart = currentAddr;
        }

        if (*textLength >= 30) {
            fprintf(objectc, "T%06X%02X%s\n",
                    *textStart, *textLength, textRec);
            textRec[0] = '\0';
            *textLength = 0;
            *textStart = currentAddr;
        }

        strcat(textRec, byteHex);
        (*textLength)++;
        currentAddr++;
    }
}
