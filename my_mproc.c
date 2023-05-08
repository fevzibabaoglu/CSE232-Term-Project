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
void is_macro(char *argv[]);
void expand();
void createPT();


int main(int argc, char *argv[]) {
    /*
    Your macro-processor must take the input file name and condition parameters as arguments, using argc, argv.
    Ex: ./my_mproc filename, 5,6
    argv[1] is the filename, argv[2], argv[3]... are condition parameters (5 and 6 in this example).
    */

    if (argc == 0) exit(EXIT_FAILURE);

    char *filename = argv[1];
    char line[500];
    FILE *file = fopen(filename, "r"); 
    m_count = read(filename);
    
    if (!file) {
        printf("\n Unable to open : %s ", filename);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        parse(line);
        is_macro(argv);
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

void is_macro(char *argv[]) {
    // code...
}

void expand() {
    // code...
}

void createPT() {
    // update PT

    // code...
}