#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// global variables
struct mac {
    char mname[8];      // macro name
    char param[10][4];  // max. 10 parameters, each parameter max. 3 characters
    char macro[256];    // macro body
};
struct mac buffer[10];  // memory buffer for 10 macro definitions
int m_count = 0;        // counts the number of macro definitions

char field[10][7];      // max 10 fields in a line, each field max 6 characters

struct pt {
    char mname[8];      // macro name
    int nparams;        // number of parameters
    char dummy[10][4];  // max. 10 parameters, each parameter max. 3 characters
    char actual[10][4];
};
struct pt PT;           // parameter table for the current macro call

int main_argc;          // same as the argc in the main function
char **main_argv;       // same as the argv[] in the main function
char *filename;         // input file name
char *asmfilename;      // output file name


int read(char *filename);
void parse(char *file);
void is_macro();
void expand();
void createPT();


int main(int argc, char *argv[]) {
    if (argc < 1) {
        printf("Not enough arguments!\n");
        exit(EXIT_FAILURE);
    }

    // set the global pointers to correct values
    main_argc = argc;
    main_argv = argv;
    filename = argv[1];

    // find the output file name (which is "($filename).asm") and set the global pointer to it
    char outputfilename[strlen(filename) + strlen(".asm") + 1];
    strcpy(outputfilename, filename);
    strcat(outputfilename, ".asm");
    asmfilename = outputfilename;

    // call read() and set m_count
    m_count = read(filename);
    
    // main loop variables
    FILE *file = fopen(filename, "r"); 
    char line[500];
    int is_prog = 0;    // it is 0 until "PROG" line comes up, and it is 1 afterward

    if (!file) {
        printf("Unable to open: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        // remove the trailing new line char in the line string
        line[strcspn(line, "\r\n")] = 0;

        // ignore until "PROG" comes
        if (!is_prog) {
            const char delim[4] = " \t\n";
            char *first_field = strtok(line, delim);    // "PROG" is the first and only thing in its line

            if (strcmp(first_field, "PROG") == 0) is_prog = 1;
            else continue;
        }

        // main loop calls
        parse(line);
        is_macro();
    }

    fclose(file);
    exit(EXIT_SUCCESS);
}


int read(char* filename) {
    const char* delim = ":, \t\n";
    FILE* file = fopen(filename, "r");
    char line[256];
    
    if (file == NULL) {
        printf("Failed to open file %s\n", filename);
        return -1;
    }
    
    while (fgets(line, sizeof(line), file)) {
        // remove the trailing new line char in the line string
        line[strcspn(line, "\r\n")] = 0;

        char* token = strtok(line, delim);

        // read until the "PROG" line comes up
        if (strcmp(token, "PROG") == 0){
            break;
        }
        
        if (token != NULL && token[0] == '#' && strcmp(token, "#ENDM") != 0) { // This line contains a MACRO name
          
            sscanf(token + 1, "%[^:]s", buffer[m_count].mname);
            
            int param_count = 0;
            token = strtok(NULL, delim); // skip "MACRO"
            token = strtok(NULL, delim); // get first parameter 
            
            while (token != NULL && strcmp(token, "#ENDM") != 0) {
                if (param_count < 10) {
                    strncpy(buffer[m_count].param[param_count++], token, sizeof(buffer[m_count].param[0]) - 1);
                    buffer[m_count].param[param_count][sizeof(buffer[m_count].param[0]) - 1] = '\0'; // add null-termination
                }

                token = strtok(NULL, delim);
            }
           
            buffer[m_count].macro[0] = '\0'; // initialize macro body to empty string
            
            while (fgets(line, sizeof(line), file) && !strstr(line, "#ENDM")) {
                // remove the trailing new line char in the line string
                line[strcspn(line, "\r\n")] = 0;

                strcat(buffer[m_count].macro, line);
                strcat(buffer[m_count].macro, "\n");
            }

            m_count++; // increment the number of macros read
        }

    }

    fclose(file);
    return m_count; // return the total number of macros read
}


void parse(char *line) {
    const char* delim = " ,'’()=\t\n";  // define delimiter characters
    int fieldCount = 0;                 // initialize field count to 0

    // reset the fields
    for (int i = 0; i < 10; i++) {
        strcpy(field[i], "");
    }

    char* token = strtok(line, delim);
    while (token != NULL && fieldCount < 10) {
        if (strlen(token) > 6) {
            printf("Error: field '%s' exceeds maximum length of 6 characters\n", token);
            return;
        }

        strcpy(field[fieldCount++], token); // copy token to current field
        token = strtok(NULL, delim);        // get next token
    }
}


void is_macro() {
    
    // check whether if there is a condition (it starts with "#if" field)
    if (strcmp(field[0], "#if") == 0) { //string comparison. It is equal to zero when both strings are found to be identical.
        
        char * indexString = field[1] + 1;  // get the string after the leading "$" sign in condition parameter (eg: $2)
        int index = atoi(indexString);      // find the int index value to use in main_argv[]

        if (strcmp(main_argv[index], field[2]) == 0) {  //evaluate the condition and check if it is true
            for (int i = 0; i < 7; i++) {               //field[3] becomes field[0] an so on for expand() and createPT() functions usage
                strcpy(field[i], field[i + 3]);
            }

            for (int i = 7; i < 10; i++) {  //fill the last 3 fields with \0
                strcpy(field[i], "\0");
            }

            expand();
            return;
        }  
    } 

    // look for a macro call (starting with #)
    else if (strchr(field[0], '#') != NULL){    //check if field[0] has # char
        expand();
        return;
    }
    
    // no macro call found, writing line to .asm file
    else {
        FILE *fp = fopen(asmfilename, "a");     //file opened in append mode (does not overwrite, any data written to the file will be added to the end)
        
        // write the fields to the output file
        for (int i = 0; i < 10; i++) {
            if (strlen(field[i]) == 0) {
                break;
            }
            fprintf(fp, "%s ", field[i]);
        }
        fprintf(fp, "\n");

        fclose(fp);
    }
}


void expand() {
    // create the PT
    createPT();

    // copy the correct macro body and add new line char at the end
    char macro_cpy[257];
    for (int i = 0; i < m_count; i++) {
        if (strcmp(buffer[i].mname, PT.mname) == 0) {
            strcpy(macro_cpy, buffer[i].macro);
            
            // if last character is not a next line, add next line at the end
            char last_char = buffer[i].macro[strlen(buffer[i].macro) - 1];
            if (last_char != '\n') {
                strcat(macro_cpy, "\n");
            }

            break;
        }
    }

    char word_chunk[50] = "";
    char write_line[500] = "";
    char current_char;
    FILE *asmfile = fopen(asmfilename, "a");

    // iterate over the macro body char by char
    for (int i = 0; i < strlen(macro_cpy); i++) {
        current_char = macro_cpy[i];

        // if current_char is not an escape sequence, add it to the word
        // continue with the remaining chars
        if (!strchr(" \t\n\r", current_char)) {
            strncat(word_chunk, &current_char, 1);
            continue;
        }

        // if the word is in PT.dummy list, replace it with the correct actual param
        if (strlen(word_chunk)) {
            for (int j = 0; j < PT.nparams; j++) {
                if (strcmp(word_chunk, PT.dummy[j]) == 0) {
                    strcpy(word_chunk, PT.actual[j]);
                    break;
                }
            }
        }

        // add the word to the line and reset the word
        if (strlen(word_chunk)) {
            strcat(write_line, word_chunk);
            strcat(write_line, " ");
            strcpy(word_chunk, "");
        }

        // write the line to the file and reset the line
        if (current_char == '\n') {
            fprintf(asmfile, "%s\n", write_line);
            strcpy(write_line, "");
        }
    }

    fclose(asmfile);
}


void createPT() {
    // remove the leading '#' in first field if there is
    if (field[0][0] == '#') {
        memmove(field[0], field[0]+1, strlen(field[0]));
    }

    // find the correct buffer index
    int bufferIndex = -1;
    for (int i = 0; i < m_count; i++) {
        if (strcmp(field[0], buffer[i].mname) == 0) {
            bufferIndex = i;
            break;
        }
    }

    if (bufferIndex < 0 || bufferIndex >= m_count) {
        printf("buffer index: %d\n", bufferIndex);
        exit(EXIT_FAILURE);
    }

    // set the mname
    strcpy(PT.mname, buffer[bufferIndex].mname);

    // set the dummy parameters and find number of parameters
    int numOfParams = 0;
    for (int i = 0; i < 10; i++) {
        if (strlen(buffer[bufferIndex].param[i])) {
            numOfParams++;
        }

        strcpy(PT.dummy[i], buffer[bufferIndex].param[i]);
    }

    // set the number of parameters
    PT.nparams = numOfParams;

    // set the actual parameters
    for (int i = 1; i < 10; i++) {
        // truncate the field and find the actual parameter
        char actualParam[4];
        strncpy(actualParam, field[i], 3);
        actualParam[3] = '\0';

        // set the actual parameter
        strcpy(PT.actual[i - 1], actualParam);
    }
}
