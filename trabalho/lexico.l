%{  
#include <stdio.h>
#include "sintaxe.tab.h"

int yyerror(const char *s);
%}

%option yylineno

%%

[ \t\r\n] {}
"//".*\n {}

"!0111000001110010011010010110111001110100" {return TOK_PRINT;}
"!0110100101100110" {return TOK_IF;}
"!01100101011011000111001101100101" {return TOK_ELSE;}
"!01101100011011110110111101110000" {return TOK_LOOP;}
"!110001001110010011001010110000101101011" {return TOK_BREAK;}
"!01110011011000110110000101101110" {return TOK_SCAN;}
"!01100010011011110110111101101100" {return TIPO_BOOL;}
"!011010010110111001110100" {return TIPO_INT;}
"!0110011001101100011011110110000101110100" {return TIPO_FLOAT;}
"!01100011011010000110000101110010" {return TIPO_CHAR;}
"!011100110111010001110010011010010110111001100111" {return TIPO_STRING;}
"!0110111101110010" {return TOK_OR;}
"!0110000101101110 01100100" {return TOK_AND;}
"!00101011" {return '+';}
"!00101101" {return '-';}
"!00101010" {return '*';}
"!00101111" {return '/';}
"(" {return '(';}
")" {return ')';}
"!00111011" {return ';';}
"!00100001" {return TOK_DIFERENTE;}
"!00111101" {return '='; }
"!0011110000111101" {return TOK_MEOI;}
"!0011111000111101" {return TOK_MAOI;}
"!0011110100111101" {return TOK_IGUAL;}
"!00111110" {return '>';}
"!00111100" {return '<';}
"{" {return '{';}
"}" {return '}';}
"!01110100011100100111010101100101" {return TOK_TRUE;}
"!0110011001100001011011000111001101100101" {return TOK_FALSE;}

\"[a-zA-Z_' '0-9]+\" {return TOK_STRING;}
[a-zA-Z_][a-zA-Z0-9_]* {return TOK_IDENT;}
\'[a-zA-Z]\' {return TOK_CHAR;}
[0-9]+\.[0-9]* {return TOK_FLOAT;}
[0-9]+ {return TOK_INT;}

. {
    printf("Simbolo nao reconhecido%c\n", yytext[0]);
    }

%%

int yywrap() {
    return 1;
}

int yyerror(const char *s){
    printf("Erro de sintaxe na linha %d: %s\n", yylineno, s);
    return 1;
}

int main(int argc, char *argv[]){

    if(argc <=1){
        printf("Sintaxe: %s nome_do_programa\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL){
        printf("Não foi possível abrir o arquivo %s.\n", argv[1]);
        return 1;
    }

    yyparse();

    if (yyin)
        fclose(yyin);

    return 0;
}