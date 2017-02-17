//*****************************************************************************
// ECE 588 Project
// By: Sean Koppenhafer and Luis Santiago
// join_common.c
//*****************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include "join_common.h"


Node* allocate_node(trip_data* trip_info, Node* previous_head_node) {
    Node* new_node = malloc( sizeof(Node) );
    if(new_node == NULL) {
        printf("ERROR: Failed to malloc new node\n");
        exit(1);
    }
    new_node->trip_info = trip_info;
    new_node->next = previous_head_node;

    return new_node;
}


void add_trip_to_head(Node** head_node, trip_data* trip) {
    *head_node = allocate_node(trip, *head_node);
}


void free_nodes(head_node* head_array, uint32_t num_employees) {
    Node* current_node;
    Node* next_node;

    for(uint32_t i = 0; i < num_employees; i++) {
        current_node = head_array[i].head;
        while(current_node != NULL) {
            next_node = current_node->next;
            free(current_node);
            current_node = next_node;
        }
    }
}

