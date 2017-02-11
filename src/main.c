#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define NUM_EMPLOYEES 20000

typedef struct {
    uint32_t ID;
    char name[30];
} employee_data;

static employee_data employees[NUM_EMPLOYEES];

uint8_t parse_cmdline(int, char**);
void create_employee_database();

int main(int argc, char* argv[]) {
    uint8_t number_of_threads = parse_cmdline(argc, argv);
    create_employee_database();
}


uint8_t parse_cmdline(int argc, char** argv) {
    uint8_t number_of_threads;

    if( (argc < 2) || (argc > 4)) {
        printf("ERROR: Argc = %d. Expected 2-4 arguments", argc);
        exit(-1);
    }
    if(argc >= 2) sscanf(argv[1], "%"SCNu8, &number_of_threads);

    return number_of_threads;
}


//*****************************************************************************
// Functions
//*****************************************************************************


void create_employee_database(employee_data* employee_database) {
    char* employee_info_filename = "employee_info.db";

    FILE* employee_info_FH = fopen(employee_info_filename, "r");
    if(employee_info_FH == NULL) {
        printf("ERROR: Failed to open file %s\n", employee_info_filename);
        exit(1);
    }

    char name[30];
    uint32_t test;
        fscanf(employee_info_FH, "%u,%s", &test, name);
    printf("woo: %u, %s\n", test, name);
    //for(uint32_t i = 0; i < NUM_EMPLOYEES; i++) {
    //    fscanf(employee_info_FH, "%u,%s", &(employee_database[i].ID), &(employee_database[i].name[0]));
    //    printf("At num %u\n", employee_database[i].ID);
    //}

    fclose(employee_info_FH);
}

