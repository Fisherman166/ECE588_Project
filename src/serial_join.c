#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include "common.h"
#include "serial_join.h"

typedef struct Node Node;

struct Node {
    trip_data *trip_info;
    Node *next;
};

typedef struct {
    employee_data *employee_info;
    Node* head;
} head_node;


static head_node employee_node[NUM_EMPLOYEES];

static Node* allocate_node(trip_data* trip_info, Node* previous_next_node) {
    Node* new_node = malloc( sizeof(Node) );
    if(new_node == NULL) {
        printf("ERROR: Failed to malloc new node\n");
        exit(1);
    }
    new_node->trip_info = trip_info;
    new_node->next = previous_next_node;

    return new_node;
}

static void free_nodes(head_node* head_array, uint32_t num_employees) {
    Node* current_node;
    Node* next_node;

    for(uint32_t i = 0; i < num_employees; i++) {
        current_node = employee_node[i].head;
        while(current_node != NULL) {
            next_node = current_node->next;
            free(current_node);
            current_node = next_node;
        }
    }
}


static void add_trip_to_head(Node** head_node, trip_data* trip) {

    if(*head_node == NULL) {
        *head_node = allocate_node(trip, NULL);
    }
    else {
        *head_node = allocate_node(trip, *head_node);
    }
}


//*****************************************************************************
// Functions
//*****************************************************************************
void serial_create_join_database(employee_data* employee_data_array, uint32_t num_employees) {
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
    free_nodes( &(employee_node[0]), num_employees);
}

