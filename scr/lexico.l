%{
#include <stdio.h>
#include <stdlib.h>

bool force_print_tree = false;
char * build_file_name;
class Node;
#include "sintaxe.tab.h"
int yyerror(const char *s);
%}

%option yylineno

%%
[ \t\r\n_]  {   /*ignora*/  }
"//".*\n { /*Comentário*/ }

"!0111000001110010011010010110111001110100" {return TOK_PRINT;}
"todo1" {return TOK_ENQUANTO;}
"todo2" {return TOK_LOOP;}
"!0110100101100110" {return TOK_SE;}
"!01100101011011000111001101100101" {return TOK_SENAO;}
"todo3" {return TOK_NAO;}
"todo4" {return TOK_E;}
"!0110111101110010" {return TOK_OU;}
"todo5" {return TOK_IGUAL;}
"todo6" {return TOK_DIFERENTE;}
"todo8" {return '+' ; }
"todo7" {return '-' ; }
"todo9" {return '*' ; }
"todo01" {return '/' ; }
"todo11" {return '(' ; }
"todo12" {return ')' ; }
"todo13" {return ';' ; }
"todo14" {return '=' ; }
"todo15" {return '<' ; }
"todo16" {return '>' ; }
"todo17" {return '{' ; }
"todo18" {return '}' ; }
"todo19" {return '^' ;}
"todo20" {return '!' ;}
"todo21" {return '[' ; }
"todo22" {return ']' ; }


"verdadeiro" {return TOK_VERDADEIRO;}
"falso" {return TOK_FALSO;}

[a-zA-Z_][a-zA-Z0-9_]* {yylval.str = strndup(yytext, yyleng);
                        return TOK_ID ; }

[0-9]+\.[0-9]* {yylval.flt = atof(yytext);
                return TOK_PF ; }

[0-9]+  {
            yylval.itg = atoi(yytext);
            return TOK_INTEIRO ; }

\"([^\\\"]|\\.)*\"  {yylval.str = strndup(yytext, yyleng);
                    return TOK_PALAVRA ; }

.   {printf("Simbolo não reconhecido! %c\n",yytext[0]);}

%%
int yywrap(){
    return 1;
}

extern int error_count;
int yyerror(const char *s){
    printf("%s:%d:0: %s\n",build_file_name, yylineno, s);
    error_count++;
    return 1;
}

int main(int argc, char *argv[]){
    if(argc <= 1){
        printf("Sintaxe: %s nome_do_programa\n",argv[0]);
        return 1;
    }
        
    //Parametros adicionais
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"-d")==0){
            yydebug=1;
        }
        
        if (strcmp(argv[i],"-f")==0){
            force_print_tree = true;
        }
    }

    build_file_name = argv[1];
    yyin = fopen(argv[1],"r");
    if(yyin == NULL){
        printf("Não foi possivel abrir o arquivo %s.\n",build_file_name);
        return 1;
    }
    
    yyparse();

    if(yyin)
        fclose(yyin);

    return 0;
}
