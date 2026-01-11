#ifndef LEXER_H
#define LEXER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<ctype.h>

#define MAX_KEYWORDS 32
#define MAX_TOKEN_SIZE 100

typedef enum{
    FAILURE,
    KEYWORD, //int, if,  while
    IDENTIFIER, //variable and function  names
    CONSTANT, //numerics,"hello"
    OPERATOR, //+,-,%
    SPECIAL_CHARACTER,//punctuation like ;
    COMMENT, // /* */ or //
    UNKNOWN, // anything unrecognised
}Tokentype;

int is_keyword(char *token);
int is_identifier(char *token);
int is_special_character(char *token);
int is_operator(char *token);
int is_constant(char *token, int line);
int is_comment(char *token);
int is_unknown(char *token);
void tokenize(char *token, int line_num);
int check_two_char_operator(char first, char second, char *result);
#endif