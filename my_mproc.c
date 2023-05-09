#include <stdio.h>
#include <stdlib.h>

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


int read(char *filename);
void parse(char *file);
void is_macro(int argc, char *argv[]);
void expand();
void createPT();


int main(int argc, char *argv[]) {
    if (argc == 0) {
        printf("No argument is given!\n");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];

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
            strcpy(line_temp, line);
            char *first_field = strtok(line_temp, " ");

            if (!strcmp(first_field, "PROG")) is_prog = 1;
            else continue;
        }

        parse(line);
        is_macro(argc, argv);
    }

    fclose(file);
    exit(EXIT_SUCCESS);
}

int read(char *filename) {
    // update buffer[10]

    // code...
}

void parse(char *line) {
    // update field[10][7]

    // code...
}

void is_macro(int argc, char *argv[]) {
    // code...
}

void expand() {
    // code...
}

void createPT() {
    // update PT

    // code...
}