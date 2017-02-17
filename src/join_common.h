//*****************************************************************************
// ECE 588 Project
// By: Sean Koppenhafer and Luis Santiago
// join_common.h
//*****************************************************************************

#ifndef JOIN_COMMON_H
#define JOIN_COMMON_H

#include "common.h"

typedef struct Node Node;

struct Node {
    trip_data *trip_info;
    Node *next;
};

typedef struct {
    employee_data *employee_info;
    Node* head;
} head_node;

// Functions
Node* allocate_node(trip_data*, Node*);
void add_trip_to_head(Node**, trip_data*);
void free_nodes(head_node*, uint32_t);

#endif

