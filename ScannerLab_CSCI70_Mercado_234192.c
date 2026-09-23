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

        if (isspace(ch)) {
            continue;
        }

        switch(State) {
            case 'A': // start state
                if (isdigit(ch)) { // go to state B if next character is a digit
                    State = 'B';
                    fprintf(output, "NUM     %c", ch);
                }

                else if (ch == '+') { // create PLUS token if next character is +
                    fprintf(output, "PLUS    %c", ch);
                    end();
                }

                else if (ch == '-') { // create MINUS token if next character is -
                    fprintf(output, "MINUS   %c", ch);
                    end();
                }

                else if (ch == '=') { // go to state E if next charcater is =
                    State = 'E';
                }

                else { // error if other characters
                    fprintf(output, "Lexical Error reading character \"%c\"\n", ch);
                    State = 'S';
                }

                break;

            case 'B': // state for creating NUM token 
                if (isdigit(ch)) { // continue adding to NUM if next character is digit
                    State = 'B';
                    fprintf(output, "%c", ch);
                }

                else { // end NUM otherwise
                    State = 'A';
                    ungetc(ch, input);
                    end();
                }

                break;
            
            case 'E':
                if (ch == '=') { 
                    fprintf(output, "ASSIGN  =="); // end ASSIGN token if second '=' is found
                    end();
                }

                else {
                    fprintf(output, "Lexical Error reading character \"%c\"\n", ch); // go to state S if second '=' not found
                    State = 'S';
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
