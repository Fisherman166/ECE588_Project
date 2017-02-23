//*****************************************************************************
// ECE 588 Project
// By: Sean Koppenhafer and Luis Santiago
// join.c
//*****************************************************************************

#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "join.h"


typedef struct Node Node;

struct Node {
    trip_data *trip_info;
    Node *next;
};

typedef struct {
    Node** thread_head_node_array;
    trip_data* trips_array;
    uint32_t starting_trip_num;
    uint32_t ending_trip_num;
} thread_args;


static pthread_t* threads = NULL;
static thread_args* args = NULL;
static Node*** thread_head_pointers;
static uint32_t number_of_threads = 0;


//*****************************************************************************
// Private Functions
//*****************************************************************************
static void* run_join_operation(void*);


static Node*** allocate_thread_head_array_pointers() {
    Node*** thread_head_node_pointers = malloc( sizeof(Node**) * number_of_threads);
    if(thread_head_node_pointers == NULL) {
        printf("ERROR: Failed to malloc head node pointers for each thread\n");
        exit(1);
    }
    return thread_head_node_pointers;
}


static Node** allocate_thread_head_node_pointers_array(uint32_t num_employees) {
    Node** thread_head_node_pointer_array = malloc( sizeof(Node*) * num_employees );
    if(thread_head_node_pointer_array == NULL) {
        printf("ERROR: Failed to malloc thread_head_node_pointer_array\n");
        exit(1);
    }
    return thread_head_node_pointer_array;
}


static pthread_t* allocate_threads() {
    pthread_t* threads = malloc( sizeof(pthread_t) * number_of_threads );
    if(threads == NULL) {
        printf("ERROR: Threads malloc returned NULL. Attempted to malloc %u threads\n", number_of_threads);
        exit(-1);
    }
    return threads;
}


static thread_args* create_threads(pthread_t* threads, trip_data* trips_array) {
    thread_args* args = malloc( sizeof(thread_args) * number_of_threads);
    if(args == NULL) {
        printf("ERROR: Thread args malloc failed. Tried to malloc %u\n", number_of_threads);
        exit(-1);
    }

    uint32_t trips_per_thread = NUM_TRIPS / number_of_threads;
    uint32_t last_thread = number_of_threads - 1;
    for(uint32_t i = 0; i < number_of_threads; i++) {
        args[i].thread_head_node_array = thread_head_pointers[i];
        args[i].trips_array = trips_array;
        args[i].starting_trip_num = trips_per_thread * i;
        if(i == last_thread) args[i].ending_trip_num = NUM_TRIPS;
        else args[i].ending_trip_num = trips_per_thread * (i + 1);
        pthread_create(&threads[i], NULL, &run_join_operation, (void*)&args[i]);
    }

    return args;
}


static uint32_t hash_function(uint32_t employee_ID) {
    uint32_t index = employee_ID % NUM_EMPLOYEES;
    return index;
}


static Node* allocate_node(trip_data* trip_info, Node* previous_head_node) {
    Node* new_node = malloc( sizeof(Node) );
    if(new_node == NULL) {
        printf("ERROR: Failed to malloc new node\n");
        exit(1);
    }
    new_node->trip_info = trip_info;
    new_node->next = previous_head_node;

    return new_node;
}


static void add_trip_to_head(Node** head_node, trip_data* trip) {
    *head_node = allocate_node(trip, *head_node);
}


static void free_nodes(Node** head_array, uint32_t num_employees) {
    Node* current_node;
    Node* next_node;

    for(uint32_t i = 0; i < num_employees; i++) {
        current_node = head_array[i];
        while(current_node != NULL) {
            next_node = current_node->next;
            free(current_node);
            current_node = next_node;
        }
    }
}


static void* run_join_operation(void* void_args) {
    thread_args* args = (thread_args*)void_args;
    uint32_t employee_ID;
    uint32_t hashed_ID;

    for(uint32_t i = args->starting_trip_num; i < args->ending_trip_num; i++) {
        employee_ID = args->trips_array[i].ID;
        hashed_ID = hash_function(employee_ID);
        add_trip_to_head( &(args->thread_head_node_array[hashed_ID]), &(args->trips_array[i]) );
    }

    return 0;
}


//*****************************************************************************
// Functions
//*****************************************************************************
void create_join_database(trip_data* trips_array, uint32_t num_employees, uint32_t num_threads) {
    number_of_threads = num_threads;
    thread_head_pointers = allocate_thread_head_array_pointers();

    // Make all head pointers NULL
    for(uint32_t i = 0; i < number_of_threads; i++) {
        thread_head_pointers[i] = allocate_thread_head_node_pointers_array(num_employees);
        for(uint32_t j = 0; j < num_employees; j++) {
            thread_head_pointers[i][j] = NULL;
        }
    }

    threads = allocate_threads(number_of_threads);
    args = create_threads(threads, trips_array);
}


void run_threads(uint32_t number_of_threads) {
    for(uint32_t i = 0; i < number_of_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}


void print_joined_database(employee_data* employee_data_array, uint32_t num_employees) {
    FILE* output_file;
    char* output_filename = "join_output.txt";
    Node* current_node = NULL;

    output_file = fopen(output_filename, "w");
    if(output_file == NULL) {
        printf("ERROR: Failed to open output file named %s\n", output_filename);
        exit(1);
    }

    uint32_t hashed_ID;
    uint32_t employee_ID;
    for(uint32_t i = 0; i < num_employees; i++) {
        fprintf(output_file, "Printing Trips for Employee ID: %u, %s\n", employee_data_array[i].ID, employee_data_array[i].name);

        employee_ID = employee_data_array[i].ID;
        hashed_ID = hash_function(employee_ID);
        for(uint32_t array = 0; array < number_of_threads; array++) {
            current_node = thread_head_pointers[array][hashed_ID];

            while(current_node != NULL) {
                if(current_node->trip_info->ID == employee_ID) {
                    fprintf(output_file, "\tTrip Date: %s, Trip Destination: %s\n", current_node->trip_info->timestamp, current_node->trip_info->destination);
                }
                current_node = current_node->next;
            }
        }
    }
    fclose(output_file);
}


void cleanup_joined_database(uint32_t num_employees) {
    free(threads);
    free(args);

    for(uint32_t array = 0; array < number_of_threads; array++) {
        free_nodes( thread_head_pointers[array], NUM_EMPLOYEES );
        free(thread_head_pointers[array]);
    }
    free(thread_head_pointers);
}

