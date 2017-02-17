//*****************************************************************************
// ECE 588 Project
// By: Sean Koppenhafer and Luis Santiago
// serial_join.c
//*****************************************************************************

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include "common.h"
#include "join_common.h"
#include "serial_join.h"


static head_node employee_node[NUM_EMPLOYEES];


//*****************************************************************************
// Functions
//*****************************************************************************
void serial_create_join_database(employee_data* employee_data_array, uint32_t num_employees) {
    // The head should store in the employee information
    for(uint32_t i = 0; i < num_employees; i++) {
        employee_node[i].employee_info = &(employee_data_array[i]);
        employee_node[i].head = NULL;
    }
}


void serial_run_join_operation(trip_data* trip_array, uint32_t num_trips) {
    uint32_t employee_ID;

    for(uint32_t i = 0; i < num_trips; i++) {
        employee_ID = trip_array[i].ID;
        add_trip_to_head( &(employee_node[employee_ID].head), &(trip_array[i]) );
    }
}


void serial_print_joined_database(uint32_t num_employees) {
    Node* current_node = NULL;

    for(uint32_t i = 0; i < num_employees; i++) {
        current_node = employee_node[i].head;
        if(current_node == NULL) continue;

        printf("Printing Trips for Employee ID: %u, %s\n", employee_node[i].employee_info->ID, employee_node[i].employee_info->name);
        while(current_node != NULL) {
            printf("\tTrip Date: %s, Trip Destination: %s\n", current_node->trip_info->timestamp, current_node->trip_info->destination);
            current_node = current_node->next;
        }
    }
}


void serial_cleanup_joined_database(uint32_t num_employees) {
    free_nodes( &(employee_node[0]), num_employees );
}

