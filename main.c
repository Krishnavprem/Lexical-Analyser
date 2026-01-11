/*
Name : Krishna V Prem
Date : 28 - 12 -2025
Description : The lexical analyzer reads a C source file line by line and scans it character by character. 
              It groups characters into meaningful units called tokens such as keywords, identifiers, operators, literals, and symbols.
              During scanning, it ignores whitespace and comments (both single-line and multi-line ) and identifies valid tokens 
              based on predefined rules. Each recognized token is then classified and displayed, forming the first phase of a compiler.
*/

#include "lexer.h"

int main(int argc, char *argv[]){
    // Validate command line arguments
    if(argc < 2){
        fprintf(stderr, "Error: No input file specified\n");
        fprintf(stderr, "Usage: %s <filename.c>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // Validate file extension
    char *extension = strrchr(argv[1], '.');
    if(!extension || strcmp(extension, ".c") != 0){
        fprintf(stderr, "Warning: Input file should have .c extension\n");
    }
    
    // Attempt to open file
    FILE *fp = fopen(argv[1], "r");
    if(fp == NULL){
        fprintf(stderr, "Error: Unable to open file '%s'\n", argv[1]);
        perror("Details");
        return EXIT_FAILURE;
    }
    
    // Print header
    printf("================================================================================\n");
    printf("                    LEXICAL ANALYSIS REPORT                    \n");
    printf("================================================================================\n");
    printf("Source File: %s\n", argv[1]);
    printf("================================================================================\n\n");
    printf("%-8s | %-25s | %-20s\n", "LINE", "TOKEN", "TYPE");
    printf("---------|---------------------------|---------------------\n");
    
    char line[512];
    char token[MAX_TOKEN_SIZE];
    int in_comment = 0;
    int comment_start_line = 0;
    int line_number = 0;
    int error_count = 0;

    // Main lexical analysis loop
    while(fgets(line, sizeof(line), fp)){
        line_number++;
        
        int i = 0;
        while(line[i] != '\0'){
            
            // Handle continuation of multi-line comments
            if(in_comment){
                if(line[i] == '*' && line[i + 1] == '/'){
                    in_comment = 0;
                    comment_start_line = 0;
                    i = i + 2;
                }
                else{
                    i++;
                }
                continue;
            }
            
            // Skip preprocessor directives (lines starting with #)
            if(line[i] == '#'){
                break; // Skip rest of line
            }
            
            // Detect single-line comments
            if(line[i] == '/' && line[i + 1] == '/'){
                break; // Skip rest of line
            }
            
            // Detect multi-line comment start
            if(line[i] == '/' && line[i + 1] == '*'){
                in_comment = 1;
                comment_start_line = line_number;
                i = i + 2;
                continue;
            }
            
            // Skip all whitespace characters
            if(isspace(line[i])){
                i++;
                continue;
            }
            
            // Process string literals
            if(line[i] == '"'){
                int j = 0;
                token[j++] = line[i++];
                
                // Read until closing quote or end of line
                while(line[i] != '\0' && line[i] != '"'){
                    if(j < MAX_TOKEN_SIZE - 1){
                        token[j++] = line[i];
                    }
                    i++;
                }
                
                // Check if string was properly terminated
                if(line[i] == '"'){
                    token[j++] = line[i++];
                    token[j] = '\0';
                    tokenize(token, line_number);
                }
                else{
                    token[j] = '\0';
                    printf("%-8d | ERROR: Unterminated string '%s'\n", line_number, token);
                    error_count++;
                }
                continue;
            }
            
            // Process character literals
            if(line[i] == '\''){
                int j = 0;
                token[j++] = line[i++];
                
                int escaped = 0;
                int closed = 0;
                
                // Read character literal content
                while(line[i] != '\0' && j < MAX_TOKEN_SIZE - 1){
                    token[j++] = line[i];
                    
                    if(!escaped && line[i] == '\\'){
                        escaped = 1;
                        i++;
                        continue;
                    }
                    
                    if(!escaped && line[i] == '\''){
                        closed = 1;
                        i++;
                        break;
                    }
                    
                    escaped = 0;
                    i++;
                }
                
                token[j] = '\0';
                
                if(closed){
                    tokenize(token, line_number);
                }
                else{
                    printf("%-8d | ERROR: Unterminated character literal '%s'\n", line_number, token);
                    error_count++;
                }
                continue;
            }
            
            // Process numeric literals (integers, floats, hex, octal, binary)
            if(isdigit(line[i])){
                int j = 0;
                
                // Hexadecimal: 0x or 0X
                if(line[i] == '0' && (line[i + 1] == 'x' || line[i + 1] == 'X')){
                    token[j++] = line[i++];
                    token[j++] = line[i++];
                    while(isxdigit(line[i]) && j < MAX_TOKEN_SIZE - 1){
                        token[j++] = line[i++];
                    }
                }
                // Binary: 0b or 0B
                else if(line[i] == '0' && (line[i + 1] == 'b' || line[i + 1] == 'B')){
                    token[j++] = line[i++];
                    token[j++] = line[i++];
                    while((line[i] == '0' || line[i] == '1') && j < MAX_TOKEN_SIZE - 1){
                        token[j++] = line[i++];
                    }
                }
                // Octal: starts with 0 followed by digits
                else if(line[i] == '0' && isdigit(line[i + 1])){
                    while((line[i] >= '0' && line[i] <= '7') && j < MAX_TOKEN_SIZE - 1){
                        token[j++] = line[i++];
                    }
                }
                // Decimal integer or floating point
                else{
                    while((isdigit(line[i]) || line[i] == '.') && j < MAX_TOKEN_SIZE - 1){
                        token[j++] = line[i++];
                    }
                    
                    // Handle scientific notation (e or E)
                    if((line[i] == 'e' || line[i] == 'E') && j < MAX_TOKEN_SIZE - 1){
                        token[j++] = line[i++];
                        
                        // Optional sign after exponent
                        if((line[i] == '+' || line[i] == '-') && j < MAX_TOKEN_SIZE - 1){
                            token[j++] = line[i++];
                        }
                        
                        // Exponent digits
                        while(isdigit(line[i]) && j < MAX_TOKEN_SIZE - 1){
                            token[j++] = line[i++];
                        }
                    }
                }
                
                token[j] = '\0';
                tokenize(token, line_number);
                continue;
            }
            
            // Process operators and punctuation
            if(ispunct(line[i])){
                token[0] = line[i];
                token[1] = '\0';
                
                // Check for three-character operators (<<= and >>=)
                if((line[i] == '<' && line[i + 1] == '<' && line[i + 2] == '=') ||
                   (line[i] == '>' && line[i + 1] == '>' && line[i + 2] == '=')){
                    token[0] = line[i];
                    token[1] = line[i + 1];
                    token[2] = line[i + 2];
                    token[3] = '\0';
                    i = i + 3;
                    tokenize(token, line_number);
                    continue;
                }
                
                // Check for two-character operators
                if(check_two_char_operator(line[i], line[i + 1], token)){
                    i = i + 2;
                }
                else{
                    i++;
                }
                
                tokenize(token, line_number);
                continue;
            }
            
            // Process identifiers and keywords
            if(isalpha(line[i]) || line[i] == '_'){
                int j = 0;
                while((isalnum(line[i]) || line[i] == '_') && j < MAX_TOKEN_SIZE - 1){
                    token[j++] = line[i++];
                }
                token[j] = '\0';
                tokenize(token, line_number);
                continue;
            }
            
            // Handle unrecognized characters
            token[0] = line[i];
            token[1] = '\0';
            printf("%-8d | ERROR: Unrecognized character '%s'\n", line_number, token);
            error_count++;
            i++;
        }
    }
    
    // Check for unterminated multi-line comment at end of file
    if(in_comment){
        printf("\n%-8d | ERROR: Unterminated block comment (started at line %d)\n", 
               line_number, comment_start_line);
        error_count++;
    }
    
    fclose(fp);
    
    // Print footer
    printf("\n================================================================================\n");
    printf("Analysis Complete - ");
    if(error_count == 0){
        printf("No errors detected\n");
    }
    else{
        printf("%d error(s) found\n", error_count);
    }
    printf("================================================================================\n");
    
    return (error_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}