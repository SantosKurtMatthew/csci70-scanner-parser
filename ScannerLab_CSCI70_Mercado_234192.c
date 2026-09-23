/*
Token list and Implementation status
1. Identifier
2. Number
DONE*   3. String -- DOES NOT HANDLE NEWLINES YET
DONE    4. Assign: :=
DONE    5. Semicolon: ;
DONE    6. Colon: :
DONE    7. Comma: ,
DONE    8. LeftParen: (
DONE    9. RightParen: )
DONE    10. Plus: +
DONE    11. Minus: -
DONE    12. Multiply: *
DONE    13. Divide: /
DONE    14. Raise: **
DONE    15. LessThan: <
DONE    16. Equal: =
DONE    17. GreaterThan: >
DONE    18. LTEqual: <=
DONE    19. GTEqual: >=
DONE    20. NotEqual: !=
DONE    21. EndofFile
DONE    (Addtl) Commment
*/

#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>

void readFile(char *fileName) { // function to read each individual input text file
    FILE *input = fopen(fileName, "r"); 
    
    char outputName[256] = "";
    char* current = fileName;   
    char* found;

    while ((found = strstr(current, "input")) != NULL) { // replace all instances of input with output
        strncat(outputName, current, found - current);
        strcat(outputName, "output");
        current = found + strlen("input");
    }
    strcat(outputName, current);

    FILE *output = fopen(outputName, "w");

    char ch;
    char State = 'A';

    while ((ch = fgetc(input)) != EOF) { // read file character by character
        void end() // function to end a token, switch back to starting state (A)
        {
            fprintf(output, "\n");
            State = 'A';
        }

        if (isspace(ch) && State != 'J') {
            continue;
        }
        switch(State) {
            case 'A': // start state
                if (isdigit(ch)) { // go to state B if next character is a digit
                    State = 'B';
                    fprintf(output, "NUM     %c", ch);
                }

                // START OF TOKENS W/O PUSHBACK
                else if (ch == '+') { // create PLUS token if next character is +
                    fprintf(output, "PLUS    %c", ch);
                    end();
                }

                else if (ch == '-') { // create MINUS token if next character is -
                    fprintf(output, "MINUS   %c", ch);
                    end();
                }

                else if (ch == ';') { // create SEMICOLON token if next character is ;
                    fprintf(output, "Semicolon   %c", ch);
                    end();
                }

                else if (ch == ',') { // create COMMA token if next character is ,
                    fprintf(output, "Comma   %c", ch);
                    end();
                }

                else if (ch == '(') { // create LeftParen token if next character is (
                    fprintf(output, "LeftParen   %c", ch);
                    end();
                }

                else if (ch == ')') { // create RightParen token if next character is )
                    fprintf(output, "RightParen   %c", ch);
                    end();
                }

                else if (ch == '=') { // create Equal token if next character is =
                    fprintf(output, "Equal   %c", ch);
                    end();
                }


                // TOKENS W/ PUSHBACK
                else if (ch == '*') { // go to state C if next character is *
                    State = 'C';
                }

                else if (ch == '/') { // go to state D if next character is /
                    State = 'D';
                }

                else if (ch == ':') { // go to state E if next character is :
                    State = 'E';
                }
                
                else if (ch == '<') { // go to state F if next character is <
                    State = 'F';
                }

                else if (ch == '>') { // go to state G if next character is >
                    State = 'G';
                }

                else if (ch == '!') { // go to state H if next character is !
                    State = 'H';
                }

                // IDENTIFIER
                else if (isalpha(ch) != 0 || ch == '_') { // go to state I if next character is a letter or underscore (starting an identifier)
                    State = 'I';
                    fprintf(output, "Identifier     %c", ch);
                }

                // STRING
                else if (ch == '\"') { // go to state J if next character is " (starting a string)
                    State = 'J';
                    fprintf(output, "String     %c", ch);
                }

                // ERROR STATE
                else { // error if other characters
                    fprintf(output, "Lexical Error reading character \"%c\"\n", ch);
                    State = 'S';
                }

                break;

            // STATE B was moved down because there are many states needed for the NUM token

            case 'C': // state for creating Multiply/Raise token 
                if (ch == '*') { // if * again, thats a RAISE token
                    fprintf(output, "Raise   **");
                    end();
                }

                else { // anything else terminates as a MULTIPLY token
                    ungetc(ch, input);
                    fprintf(output, "Multiply   *");
                    end();
                }
                State = 'A';
                break;

            case 'D': // state for creating Multiply/Raise token 
                if (ch == '/') { // if * again, thats a COMMENT token
                    fprintf(output, "Comment   //");
                    end();
                }

                else { // anything else terminates as a DIVIDE token
                    ungetc(ch, input);
                    fprintf(output, "Divide   /");
                    end();
                }
                State = 'A';
                break;
            
            case 'E':
                if (ch == '=') { 
                    fprintf(output, "ASSIGN  :="); // end ASSIGN token if '=' is found
                    end();
                }
                else { // anything else terminates as a COLON token
                    ungetc(ch, input);
                    fprintf(output, "Colon   :");
                    end();
                }

                break;

            case 'F': // state for creating LTEqual/LessThan tokens
                if (ch == '=') { // if <= thats LTEqual token
                    fprintf(output, "LTEqual   <=");
                    end();
                }

                else { // anything else terminates as a LessThan token
                    ungetc(ch, input);
                    fprintf(output, "LessThan   <");
                    end();
                }
                State = 'A';
                break;

            case 'G': // state for creating GTEqual/GreaterThan tokens
                if (ch == '=') { // if <= thats LTEqual token
                    fprintf(output, "GTEqual   >=");
                    end();
                }

                else { // anything else terminates as a GreaterThan token
                    ungetc(ch, input);
                    fprintf(output, "GreaterThan   >");
                    end();
                }
                State = 'A';
                break;
            
            case 'H': // state for creating NotEqual token
                if (ch == '=') { // if != thats NotEqual token
                    fprintf(output, "NotEqual   !=");
                    end();
                }

                else {
                    fprintf(output, "Lexical Error reading character \"%c\"\n", ch); // go to state S if '=' not found
                    State = 'S';
                }
                break;

            case 'I': // state for creating Identifier token
                if (isalpha(ch) != 0 || isdigit(ch) != 0 || ch == '_'){ // checks if the character is a number, letter, or underscore
                    State = 'I';
                    fprintf(output, "%c", ch);
                }
                else{
                    ungetc(ch, input);
                    end();
                }
                break;

            case 'J': // state for creating String token
                if (ch == '\"'){ // checks if the string is closed, otherwise continue adding to it
                    fprintf(output, "%c", ch);
                    end();
                }
                else {
                    State = 'J';
                    fprintf(output, "%c", ch);
                }
                break;

            case 'B': // state for creating Whole Numbers (part of NUM token)
                if (isdigit(ch)) { // continue adding to NUM if next character is digit
                    State = 'B';
                    fprintf(output, "%c", ch);
                }
                else if (ch == '.'){
                    State = 'K';
                    fprintf(output, "%c", ch);
                }
                else if (ch == 'E' || ch == 'e'){
                    State = 'M';
                    fprintf(output, "%c", ch);
                }
                else { // end NUM otherwise
                    ungetc(ch, input);
                    end();
                }
                break;

            case 'K':
                if (isdigit(ch)) {
                    State = 'L';
                    fprintf(output, "%c", ch);
                }
                else{
                    fprintf(output, "\nLexical Error reading character \"%c\"\n", ch); // go to state S if number ends on a .
                    State = 'S';
                }
                break;

            case 'L': // Functionally state B but cannot take .
                if (isdigit(ch)) { // continue adding to NUM if next character is digit
                    State = 'L';
                    fprintf(output, "%c", ch);
                }
                else if (ch == 'E' || ch == 'e'){
                    State = 'M';
                    fprintf(output, "%c", ch);
                }
                else { // end NUM otherwise
                    ungetc(ch, input);
                    end();
                }
                break;
            
            case 'M': // state after E/e in the exponent
                if (ch == '+' || ch == '-'){
                    State = 'N';
                    fprintf(output, "%c", ch);
                }
                else if (isdigit(ch)){
                    State = 'O';
                    fprintf(output, "%c", ch);
                }
                else{
                    fprintf(output, "\nLexical Error reading character \"%c\"\n", ch); // go to state S if number ends on a .
                    State = 'S';
                }
                break;
            
            case 'N': // state to check that the char after the +/- in state M is a number
                if (isdigit(ch)){
                    State = 'O';
                    fprintf(output, "%c", ch);
                }
                else{
                    fprintf(output, "\nLexical Error reading character \"%c\"\n", ch); // go to state S if number ends on a .
                    State = 'S';
                }
                break;
            
            case 'O': // keep getting numbers to complete the exponent
                if (isdigit(ch)){
                    State = 'O';
                    fprintf(output, "%c", ch);
                }
                else { // end NUM otherwise
                    ungetc(ch, input);
                    end();
                }
                break;
                

            case 'S': // state that handles errors, stop output
                break;
        }
    }

    fclose(input);
}

int main()
{
    struct dirent *de;
    DIR *folder = opendir("."); // open source folder

    while ((de = readdir(folder)) != NULL) { // check all files for text files with input in the name
        char *suffix = strrchr(de->d_name, '.');

        if (strstr(de->d_name, "input") != NULL && suffix != NULL && strcmp(suffix, ".txt") == 0) {
            readFile(de->d_name); // scan each valid file
        }
    }

    closedir(folder);    
    return 0;
}
