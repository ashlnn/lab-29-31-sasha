#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Лексичний аналізатор інтерпретатора мови SPL

enum {BEGINL = 257, ENDL, IFL, THENL, WHILEL, DOL, RETRL, READL, PRITL, INTL,
      CONSTL, IDEN, NUMB};

int nst = 0;
int lval, lex;
static char nch = '\n';

FILE *PF, *padres;

void get(void);
void number(void);
void word(void);
char *add(char *nm);

char TNM[400];
char *ptn = TNM;

int main() {
    PF = fopen("var2.s", "r");
    padres = fopen("getrez.dan", "w");

    if (!PF) {
        puts("File var2.s isn't open");
    } else {
        get();
    }

    fclose(PF);
    fclose(padres);

    return 0;
}

void get() {
    while (nch != EOF) {
        while (isspace(nch)) {
            if (nch == '\n')
                nst++;
            nch = getc(PF);
        }

        if (isdigit(nch)) {
            number();
        } else if (isalpha(nch)) {
            word();
        } else if (nch == '{' || nch == '}') {
            if (nch == '{')
                lex = BEGINL;
            else
                lex = ENDL;
            nch = getc(PF);
        } else if (nch == '(' || nch == ')' || nch == ',' || nch == ';' || nch == '=' ||
                   nch == '+' || nch == '-' || nch == '*' || nch == '/' || nch == '%') {
            lex = nch;
            nch = getc(PF);
        } else {
            if (nch != EOF)
                puts("Invalid symbol");
            nch = getc(PF);
        }
    }

    lex = EOF;
}

void number() {
    for (lval = 0; isdigit(nch); nch = getc(PF))
        lval = lval * 10 + nch - '0';
    lex = NUMB;
}

void word() {
    static int cdl[] = {IFL, THENL, WHILEL, DOL, RETRL, READL, PRITL, INTL, CONSTL, IDEN, NUMB};
    static char *serv[] = {"if", "then", "while", "do", "return", "read",
                           "print", "int", "const"};

    int i;
    char tx[40];
    char *p;

    for (p = tx; isdigit(nch) || isalpha(nch); nch = getc(PF))
        *(p++) = nch;
    *p = '\0';

    for (i = 0; i < 9; i++)
        if (strcmp(tx, serv[i]) == 0) {
            lex = cdl[i];
            return;
        }

    lex = IDEN;
    lval = (int)add(tx);
    printf("Address for %s = %p\n", tx, (void *)lval);
    fprintf(padres, "Address for %s = %p\n", tx, (void *)lval);
}

char *add(char *nm) {
    char *p;
    for (p = TNM; p < ptn; p += strlen(p) + 1)
        if (strcmp(p, nm) == 0)
            return p;
    if ((ptn + strlen(nm) + 1) > TNM + 400) {
        puts("Overload of TNM");
        exit(0);
    }
    strcpy(ptn, nm);
    p = ptn;
    ptn += strlen(nm) + 1;
    return p;
}
