#include <stdio.h>
#include <string.h>
#include <regex.h> 
#include <stdlib.h>
#include <ctype.h>


int isTypeReference(char* token);
int matchesRegEx(char* token);
int isIdentifier(char* token);
int isNumber(char* token);

int validateInput(char* lexicalInput){
    /* 
    go through the input string and split into tokens
    seperated by ascii characters listed below:
    HORIZONTAL TABULATION (9) 
    LINE FEED (10) 
    VERTICAL TABULATION (11) 
    FORM FEED (12) 
    CARRIAGE RETURN (13) 
    SPACE (32) 
    */

   int j, word, started = 0, startedSequence = 0, identifierSeen = 0;
   char tokens[60][30];

   size_t length = strlen(lexicalInput);
   size_t i = 0;

   j = 0, word = 0;

    for (; i < length; i++){
        if(lexicalInput[i]==9 || lexicalInput[i]==10 || 
        lexicalInput[i]==11 || lexicalInput[i]==12 ||
        lexicalInput[i]==13 || lexicalInput[i]==32 ||
        lexicalInput[i]==' '|| lexicalInput[i] == '        '
        || lexicalInput[i]=='  '
        ){
            tokens[word][j]='\0';
            word++;
            j = 0;
        }else{
            tokens[word][j]=lexicalInput[i];
            j++;
        }
    }

    //check if first token is type reference
    if (isTypeReference(tokens[0]) != 1) {
        printf("%s is not a valid type reference\n", tokens[0]);
        return 0;
    }

    printf("type reference: %s\n", tokens[0]);
    
    // iterate through all of the tokens
    for(i = 0; i < word; i++){
        // printf("token: %s\n", tokens[i]);
        if(strcmp(tokens[i],"BEGIN") == 0 || (strcmp(tokens[i], "SEQUENCE")== 0)){
            if(strcmp(tokens[i-1],"::=") != 0){
                printf("assignment lexical item not found before begin or sequence\n");
                return 0;
            }
            if((strcmp(tokens[i], "BEGIN")== 0)){
                if (isTypeReference(tokens[i-3]) != 1) {
                printf("token %s is not a type reference\n", tokens[i-1]);
                return 0;
            }
             started = 1; 
            printf("sequence begin reached\n");
            }
            if((strcmp(tokens[i], "SEQUENCE")== 0)){
                if (isTypeReference(tokens[i-2]) != 1) {
                printf("token %s is not a type reference\n", tokens[i-1]);
                return 0;
            }
            
                printf("type reference: %s\n", tokens[i-2]);
                
                
                startedSequence = 1;
            }
        }


        // if begin has been seen
        if (started = 1) {
            if (startedSequence == 1) {
                if (strcmp(tokens[i], "SEQUENCE") == 0 && strcmp(tokens[i+1], "{") != 0){
                    printf("missing opening bracket\n");
                    return 0;
                }

                if (strcmp(tokens[i], "{") == 0) {
                    if (isIdentifier(tokens[i+1]) != 1) {
                        printf("identifier %s does not meet rules\n", tokens[i+1]);
                        return 0;
                    }
                    
                    printf("-identifier: %s-\n", tokens[i+1]);
                }

                if (strcmp(tokens[i], ",") == 0) {
                    if (isIdentifier(tokens[i+1]) != 1) {
                        printf("identifier %s does not meet rules\n", tokens[i+1]);
                        return 0;
                    }

                     printf("-identifier: %s-\n", tokens[i+1]);
                    printf("--identifier type: %s--\n", tokens[i+2]);
                }

                if (strcmp(tokens[i], "INTEGER") == 0){
                    if (isNumber(tokens[i+1]) != 1){
                        printf("%s is not a valid number\n", tokens[i+1]);
                        return 0;
                    }

                    printf("---integer value: %s---\n", tokens[i+1]);
                }
            }
        }

        if (strcmp(tokens[i], "END") == 0 && startedSequence == 0 && i == word -1) {
            printf("end reached\n");
            return 0;
        }
    }
   
    return 1;
}

// A "number" shall consist of one or more digits. 
// The first digit shall not be zero unless the "number" is a single digit. 
int isNumber(char* token){
    regex_t regexNum;
    int *returnVal, status;

    returnVal = regcomp(&regexNum, "^(\([1-9]*[0-9]*[..]*[1-9][0-9]*[ | ]*[1-9]*[0-9]*[..]*[1-9][0-9]*\)[,]*)$", 0);
    if (returnVal != 0){
        printf("failure creating regular expression\n");
        exit(1);
    }

    status = regexec(&regexNum, token, (size_t) 0, NULL, 0);
    if (status != REG_NOMATCH) {
        printf("token %s did not pass number regex\n", token);
        return 0;
    }

    return 1;
}

int matchesRegEx(char* token){
    regex_t regex;
    int *returnVal, status;

    returnVal = regcomp(&regex, "^[a-zA-Z0-9-]*$", 0);
    if (returnVal != 0){
        printf("failure creating regular expression\n");
        exit(1);
    }

    status = regexec(&regex, token, (size_t) 0, NULL, 0);
    if (status == REG_NOMATCH) {
        printf("\ntoken %s did not pass type ref regex\n", token);
        return 0;
    }

    return 1;
}

// A "typereference" shall consist of an arbitrary number (one or more) of letters, digits, and hyphens. 
// The initial  character shall be an upper-case letter.
// A hyphen shall not be the last character. A hyphen shall not be immediately followed by another hyphen. 
int isTypeReference(char* token){
    if(isupper(token[0]) == 0){
        return 0;
    }

    size_t length = strlen(token);
    size_t i = 0;
    for(; i < length; i++){
        if (token[i] == '-' && i != length - 1){
            if (token[i+1] == '-' ){
                return 0;
            }
        }

        if (i == length - 1 && token[i] == '-') {
            return 0;
        }
    }

    // see if token matches regular expression
    if (matchesRegEx(token) != 1){
        return 0;
    }

    return 1;
}

/* 
  An "identifier" shall consist of an arbitrary number (one or more) of letters, digits, and hyphens. 
  The initial character shall be a lower-case letter.
  A hyphen shall not be the last character. 
 A hyphen shall not be immediately followed by another hyphen. 
*/
int isIdentifier(char* token){
    if(islower(token[0]) == 0){
        return 0;
    }

    size_t length = strlen(token);
    size_t i = 0;
    for(; i < length; i++){
        if (token[i] == '-' && i != length - 1){
            if (token[i+1] == '-' ){
                return 0;
            }
        }

        if (i == length - 1 && token[i] == '-') {
            return 0;
        }
    }

    // see if token matches regular expression
    if (matchesRegEx(token) != 1){
        return 0;
    }

    return 1;
}

int main(void) {
    char lexicalInput[500] = 
    "MyShopPurchaseOrders TAGS ::= BEGIN\n"
    "PurchaseOrder ::= SEQUENCE {\n"
    "dateOfOrder DATE ,\n"
    "customer CustomerInfo ,\n"
    "items ListOfItems }\n"
    "Item ::= SEQUENCE {\n"
    "itemCode INTEGER (1..20) ,\n"
    "power INTEGER (110|220) ,\n"
    "deliveryTime INTEGER (8..12|14..19) ,\n"
    "quantity INTEGER (1..1000) ,\n"
    "unitPrice INTEGER (1..99) ,} END\n";
    
    if (validateInput(lexicalInput) != 0){
        printf("Successful");
    }else{
        printf("Unsuccessful");
    }

    return(0);
}