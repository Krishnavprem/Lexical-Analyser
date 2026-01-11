#include<stdio.h>

int main(){
    int a = 10;
    float b = 20.5;
    double c = 3.14e-10;
    unsigned long x = 0x1A2F;
    int octal = 0755;
    int binary = 0b1010;
    
    if(a < b){
        a = a + b;
    }
    else{
        a = a - b; 
    }
    
    // This is single line comment
    
    /* This is multi
       line
       comment */
    
    char ch = 'A';
    char escaped = '\n';
    char *str = "Hello World";
    
    for(int i = 0; i < 5; i++){
        a = a + i;
    }
    
    // Test operators
    a++;
    b += 2.5;
    x <<= 2;
    
    // Test keywords
    register int fast_var;
    static volatile const char *ptr;
    
    return a;
}
