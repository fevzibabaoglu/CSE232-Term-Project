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
char *filename = "example_input_file";         //input file name
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
    //filename = argv[1];

    //char outputfilename[strlen(filename) + strlen(".asm") + 1];
    //strcpy(outputfilename, filename);
    //strcat(outputfilename, ".asm");
    //asmfilename = outputfilename;

    //m_count = read(filename);

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
            strcpy(line_temp, line);
            char *first_field = strtok(line_temp, " ");
            parse(line);
            if (!strcmp(first_field, "PROG")) is_prog = 1;
            else continue;  
        }
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
    const char* delim = " ,'()=\t\n";  // define delimiter characters
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
    // code...
}

void createPT() {
    // update PT

    // code...
}
