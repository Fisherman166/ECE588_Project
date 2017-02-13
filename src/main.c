//*****************************************************************************
// ECE 588 Project
// By: Sean Koppenhafer and Luis Santiago
// Common.h
//*****************************************************************************

#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "common.h"
#include "join.h"

static employee_data employees[NUM_EMPLOYEES];
static trip_data trips[NUM_TRIPS];

uint8_t parse_cmdline(int, char**);
void create_employee_database(employee_data*);
void create_trip_database(trip_data*);

//*****************************************************************************
// Functions
//*****************************************************************************
int main(int argc, char* argv[]) {
    //uint8_t number_of_threads = parse_cmdline(argc, argv);
    create_employee_database(&(employees[0]));
    create_trip_database(&(trips[0]));

    create_join_database(&(employees[0]), NUM_EMPLOYEES);
    run_join_operation( &(trips[0]), NUM_TRIPS);
    print_joined_database(NUM_EMPLOYEES);
    cleanup_joined_database(NUM_EMPLOYEES);
}


uint8_t parse_cmdline(int argc, char** argv) {
    uint8_t number_of_threads;

    if( (argc < 2) || (argc > 4)) {
        printf("ERROR: Argc = %d. Expected 2-4 arguments\n", argc);
        exit(-1);
    }
    if(argc >= 2) sscanf(argv[1], "%"SCNu8, &number_of_threads);

    return number_of_threads;
}


void create_employee_database(employee_data* employee_database) {
    char* employee_info_filename = "employee_info.db";

    FILE* employee_info_FH = fopen(employee_info_filename, "r");
    if(employee_info_FH == NULL) {
        printf("ERROR: Failed to open file %s\n", employee_info_filename);
        exit(1);
    }

    for(uint32_t i = 0; i < NUM_EMPLOYEES; i++) {
        // Ugly format to be able to read in the space between the first and last name
        fscanf(employee_info_FH, "%u,%30[^\n]%*c", &(employee_database[i].ID), &(employee_database[i].name[0]));
    }

    fclose(employee_info_FH);
}

void create_trip_database(trip_data* trip_database) {
    char* trip_info_filename = "trips.db";

    FILE* trip_info_FH = fopen(trip_info_filename, "r");
    if(trip_info_FH == NULL) {
        printf("ERROR: Failed to open file %s\n", trip_info_filename);
        exit(1);
    }
    
    for(uint32_t i = 0; i < NUM_TRIPS; i++) {
        // Ugly format to be able to read in spaces
        fscanf(trip_info_FH, "%15[^,],%u,%30[^\n]%*c", &(trip_database[i].timestamp[0]), &(trip_database[i].ID), &(trip_database[i].destination[0]));
    }

    fclose(trip_info_FH);
}

