#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// global variables
struct mac {
    char mname[8];      //macro name
    char param[10][4];  //max. 10 parameters, each parameter max. 3 characters
    char macro[256];    //macro body
};
struct mac buffer[10];  // memory buffer for 10 macro definitions
int m_count;            // counts the number of macro definitions

char field[10][7];      //max 10 fields in a line, each field max 6 characters

struct pt {
    char mname[8];      //macro name
    int nparams;        //number of parameters
    char dummy[10][4];  //max. 10 parameters, each parameter max. 3 characters
    char actual[10][4];
};
struct pt PT;

int main_argc;
char **main_argv;
char *filename;         //input file name
char *asmfilename;      //output file name


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

    main_argc = argc;
    main_argv = argv;
    filename = argv[1];

    char outputfilename[strlen(filename) + strlen(".asm") + 1];
    strcpy(outputfilename, filename);
    strcat(outputfilename, ".asm");
    asmfilename = outputfilename;

    m_count = read(filename);
    
    FILE *file = fopen(filename, "r"); 
    char line[500];

    if (!file) {
        printf("Unable to open : %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int is_prog = 0;
    while (fgets(line, sizeof(line), file)) {
        // ignore until "PROG" comes
        if (!is_prog) {
            char line_temp[500];
            const char delim[4] = " \t\n";
            strcpy(line_temp, line);
            char *first_field = strtok(line_temp, delim);

            if (!strcmp(first_field, "PROG")) is_prog = 1;
            else continue;
        }

        parse(line);
        is_macro();
    }

    fclose(file);
    exit(EXIT_SUCCESS);
}

int read(char *filename) {
    // update buffer[10]

    // code...
}

void parse(char *line) {
    const char* delim = " \t\n";  // define delimiter characters
    int fieldCount = 0;           // initialize field count to 0
    char* token = strtok(line, delim);
    
    while (token != NULL && fieldCount < 10) {
        if (strlen(token) > 6) {
            printf("Error: field '%s' exceeds maximum length of 6 characters\n", token);
            return;
        }
        strcpy(field[fieldCount++], token);  // copy token to current field
        token = strtok(NULL, delim);       // get next token
    }
    
    // print the fields
    printf("(TEST PRINT) Fields are:");
    for (int i = 0; i < fieldCount; i++) {
        printf("field[%d]->%s ", i, field[i]);
    }
    printf("\n");
    printf("\n");   
}

void is_macro() {
    // code...
}

void expand() {
    createPT();

    char macro_cpy[257];
    for (int i = 0; i < m_count; i++) {
        if (strcmp(buffer[i].mname, PT.mname) == 0) {
            strcpy(macro_cpy, buffer[i].macro);
            
            // if last character is not a next line, add space at the end
            char last_char = buffer[i].macro[strlen(buffer[i].macro) - 1];
            if (last_char != '\n') {
                strcat(macro_cpy, "\n");
            }

            break;
        }
    }

    char non_space_chunk[50] = "";
    char write_chunk[50] = "";
    char current_char;
    FILE *asmfile = fopen(asmfilename, "a");

    for (int i = 0; i < strlen(macro_cpy); i++) {
        current_char = macro_cpy[i];

        if (current_char == ' ' || current_char == '\n' || current_char == '\t') {
            strcpy(write_chunk, non_space_chunk);

            // check whether the word is non-empty string or not
            if (strlen(non_space_chunk) != 0) {

                // check word, if word is in pt.dummy replace it with pt.actual
                for (int j = 0; j < PT.nparams; j++) {
                    if (strcmp(non_space_chunk, PT.dummy[j]) == 0) {
                        strcpy(write_chunk, PT.actual[j]);
                        break;
                    }
                }

                strcpy(non_space_chunk, "");
            }

            // write to file
            fprintf(asmfile, "%s%c", write_chunk, current_char);
        }
        else {
            strncat(non_space_chunk, &current_char, 1);
        }
    }

    fclose(asmfile);
}

void createPT() {
    // update PT

    // code...
}
