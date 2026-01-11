#include "lexer.h"

void tokenize(char *token, int line_num){
    if(token[0] == '\0'){
        return;
    }
    if(is_keyword(token) == KEYWORD){
        printf("%-8d | %-25s | %-20s\n", line_num, token, "KEYWORD");
    }
    else if(is_identifier(token) == IDENTIFIER){
        printf("%-8d | %-25s | %-20s\n", line_num, token, "IDENTIFIER");
    }
    else if(is_special_character(token) == SPECIAL_CHARACTER){
        printf("%-8d | %-25s | %-20s\n", line_num, token, "SPECIAL CHARACTER");
    }
    else if(is_operator(token) == OPERATOR){
        printf("%-8d | %-25s | %-20s\n", line_num, token, "OPERATOR");
    }
    else if(is_constant(token, line_num) == CONSTANT){
        printf("%-8d | %-25s | %-20s\n", line_num, token, "CONSTANT");
    }
    else{
        printf("%-8d | %-25s | %-20s\n", line_num, token, "UNKNOWN");
    }
}

int is_keyword(char *token){
    char *keyword[MAX_KEYWORDS] = {"int","float","char","double","if","else","for",
        "while","do","break","void","switch","case","default","return",
        "continue","struct","typedef","const","sizeof","auto", "register", "extern", "static", 
        "volatile", "unsigned", "signed", "long", "short", "enum", "union", "goto"};
    int i;
    int size = sizeof(keyword)/sizeof(keyword[0]);
    for(i=0;i<size;i++){
        if(strcmp(token,keyword[i])==0){
            return KEYWORD;
        }
    }
    return FAILURE;
}

int is_identifier(char *token){
    if(is_keyword(token)==KEYWORD){
        return FAILURE;
    }
    if(!(isalpha(token[0]) || token[0] == '_')){
        return FAILURE;
    }
    int i;
    for(i=1;token[i] != '\0';i++){
        if(!(isalnum(token[i]) || token[i] =='_')){
            return FAILURE;
        }
    }
    return IDENTIFIER;
}
int is_special_character(char *token){
    char *special_character[] = {"(", ")", "{", "}", "[", "]", ",", ";", "."};
    int i; 
    int size = sizeof(special_character)/sizeof(special_character[0]);                     
    for(i=0;i<size;i++){
        if(strcmp(token, special_character[i])==0){
            return SPECIAL_CHARACTER;
        }
    }
    return FAILURE;
}
int is_operator(char *token){
    char *operator[] ={"+", "-", "*", "/", "%", "=", "<", ">", "!", "&", "|", 
                    "^", "~", "?", ":", "++", "--", "==", "!=", "<=", ">=", 
                    "&&", "||", "+=", "-=", "*=", "/=", "%=", "->"};
    int i;
    int size = sizeof(operator)/sizeof(operator[0]);
    for(i=0;i<size;i++){
        if(strcmp(token, operator[i])==0){
            return OPERATOR;
        }
    }
    return FAILURE;
}
int is_constant(char *token, int line_num){
    int len = strlen(token);
    
    if(len == 0){
        return FAILURE;
    }
    
    // String literal: "text"
    if(len >= 2 && token[0] == '"' && token[len - 1] == '"'){
        return CONSTANT;
    }
    
    // Character literal: 'c' or '\n'
    if(len >= 3 && len <= 4 && token[0] == '\'' && token[len - 1] == '\''){
        return CONSTANT;
    }
    
    // Hexadecimal constant: 0x1A2F or 0X1a2f
    if(len > 2 && token[0] == '0' && (token[1] == 'x' || token[1] == 'X')){
        if(len == 2){
            printf("%-8d | ERROR: Incomplete hexadecimal constant '%s'\n", line_num, token);
            return FAILURE;
        }
        
        int i;
        for(i = 2; i < len; i++){
            if(!isxdigit(token[i])){
                printf("%-8d | ERROR: Invalid hexadecimal constant '%s' (invalid digit '%c')\n", 
                       line_num, token, token[i]);
                return FAILURE;
            }
        }
        return CONSTANT;
    }
    
    // Binary constant: 0b1010 or 0B1010
    if(len > 2 && token[0] == '0' && (token[1] == 'b' || token[1] == 'B')){
        if(len == 2){
            printf("%-8d | ERROR: Incomplete binary constant '%s'\n", line_num, token);
            return FAILURE;
        }
        
        int i;
        for(i = 2; i < len; i++){
            if(token[i] != '0' && token[i] != '1'){
                printf("%-8d | ERROR: Invalid binary constant '%s' (invalid digit '%c')\n", 
                       line_num, token, token[i]);
                return FAILURE;
            }
        }
        return CONSTANT;
    }
    
    // Octal constant: 0755 (leading zero followed by digits 0-7)
    if(len > 1 && token[0] == '0' && isdigit(token[1])){
        int i;
        for(i = 1; i < len; i++){
            if(token[i] < '0' || token[i] > '7'){
                printf("%-8d | ERROR: Invalid octal constant '%s' (digit '%c' out of range)\n", 
                       line_num, token, token[i]);
                return FAILURE;
            }
        }
        return CONSTANT;
    }
    
    // Decimal integer or floating-point constant
    int count = 0;
    int has_exp = 0;
    int has_digit = 0;
    int i;
    
    for(i = 0; i < len; i++){
        char current = token[i];
        
        // Check for digits
        if(isdigit(current)){
            has_digit = 1;
            continue;
        }
        
        // Check for decimal point
        if(current == '.'){
            count++;
            if(count > 1){
                printf("%-8d | ERROR: Multiple decimal points in '%s'\n", line_num, token);
                return FAILURE;
            }
            continue;
        }
        
        // Check for exponent (e or E)
        if(current == 'e' || current == 'E'){
            if(has_exp){
                printf("%-8d | ERROR: Multiple exponents in '%s'\n", line_num, token);
                return FAILURE;
            }
            if(!has_digit){
                printf("%-8d | ERROR: Exponent without preceding digits in '%s'\n", line_num, token);
                return FAILURE;
            }
            
            has_exp = 1;
            i++;
            
            // Check for optional sign after exponent
            if(i < len && (token[i] == '+' || token[i] == '-')){
                i++;
            }
            
            // Must have at least one digit after exponent
            if(i >= len || !isdigit(token[i])){
                printf("%-8d | ERROR: Invalid exponent format in '%s'\n", line_num, token);
                return FAILURE;
            }
            continue;
        }
        
        // Invalid character in numeric constant
        printf("%-8d | ERROR: Invalid character '%c' in numeric constant '%s'\n", 
               line_num, current, token);
        return FAILURE;
    }
    
    // Valid numeric constant must have at least one digit
    if(has_digit){
        return CONSTANT;
    }
    
    return FAILURE;
}

// Helper function to check for two-character operators
int check_two_char_operator(char first, char second, char *result){
    char pair[3] = {first, second, '\0'};
    
    // List of all two-character operators
    char *two_char_ops[] = {
        "++", "--", "==", "!=", "<=", ">=", 
        "&&", "||", "+=", "-=", "*=", "/=", 
        "%=", "->", "<<", ">>", "&=", "|=", 
        "^=", NULL
    };
    
    int i;
    for(i = 0; two_char_ops[i] != NULL; i++){
        if(strcmp(pair, two_char_ops[i]) == 0){
            strcpy(result, pair);
            return 1; // Found valid two-character operator
        }
    }
    
    return 0; // Not a two-character operator
}