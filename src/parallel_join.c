//*****************************************************************************
// ECE 588 Project
// By: Sean Koppenhafer and Luis Santiago
// parallel_join.c
//*****************************************************************************

#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "parallel_join.h"
#include "join_common.h"

typedef struct {
    head_node* thread_array;
    trip_data* trips_array;
    uint32_t starting_trip_num;
    uint32_t ending_trip_num;
} thread_args;


static pthread_t* threads = NULL;
static thread_args* args = NULL;
static head_node** thread_head_nodes;
static uint32_t number_of_threads = 0;


//*****************************************************************************
// Private Functions
//*****************************************************************************
static void* parallel_run_join_operation(void*);


static head_node** allocate_thread_array_pointers() {
    head_node** thread_array_ptr_array = malloc( sizeof(head_node*) * number_of_threads);
    if(thread_array_ptr_array == NULL) {
        printf("ERROR: Failed to malloc head node pointers for each thread\n");
        exit(1);
    }
    return thread_array_ptr_array;
}


static head_node* allocate_thread_arrays(uint32_t num_employees) {
    head_node* thread_array = malloc( sizeof(head_node) * num_employees );
    if(thread_array == NULL) {
        printf("ERROR: Failed to malloc thread_array\n");
        exit(1);
    }
    return thread_array;
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
        args[i].thread_array = thread_head_nodes[i];
        args[i].trips_array = trips_array;
        args[i].starting_trip_num = trips_per_thread * i;
        if(i == last_thread) args[i].ending_trip_num = NUM_TRIPS;
        else args[i].ending_trip_num = trips_per_thread * (i + 1);
        pthread_create(&threads[i], NULL, &parallel_run_join_operation, (void*)&args[i]);
    }

    return args;
}


static void* parallel_run_join_operation(void* void_args) {
    thread_args* args = (thread_args*)void_args;
    uint32_t employee_ID;

    for(uint32_t i = args->starting_trip_num; i < args->ending_trip_num; i++) {
        employee_ID = args->trips_array[i].ID;
        add_trip_to_head( &(args->thread_array[employee_ID].head), &(args->trips_array[i]) );
    }

    return 0;
}


//*****************************************************************************
// Functions
//*****************************************************************************
void parallel_create_join_database(employee_data* employee_data_array, trip_data* trips_array,
                                   uint32_t num_employees, uint32_t num_threads) {
    number_of_threads = num_threads;
    thread_head_nodes = allocate_thread_array_pointers();

    for(uint32_t i = 0; i < number_of_threads; i++) {
        thread_head_nodes[i] = allocate_thread_arrays(num_employees);
        for(uint32_t j = 0; j < num_employees; j++) {
            (thread_head_nodes[i])[j].employee_info = &(employee_data_array[j]);
            (thread_head_nodes[i])[j].head = NULL;
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


void parallel_print_joined_database(uint32_t num_employees) {
    bool employee_went_on_trip;
    Node* current_node = NULL;

    for(uint32_t i = 0; i < num_employees; i++) {
        employee_went_on_trip = false;

        for(uint32_t array = 0; array < number_of_threads; array++) {
            if( (thread_head_nodes[array])[i].head != NULL) employee_went_on_trip = true;
        }
        if(!employee_went_on_trip) continue;


        printf("Printing Trips for Employee ID: %u, %s\n", (thread_head_nodes[0])[i].employee_info->ID, (thread_head_nodes[0])[i].employee_info->name);

        for(uint32_t array = 0; array < number_of_threads; array++) {
            current_node = (thread_head_nodes[array])[i].head;

            while(current_node != NULL) {
                printf("\tTrip Date: %s, Trip Destination: %s\n", current_node->trip_info->timestamp, current_node->trip_info->destination);
                current_node = current_node->next;
            }
        }
    }
}


void parallel_cleanup_joined_database(uint32_t num_employees) {
    free(threads);
    free(args);

    for(uint32_t array = 0; array < number_of_threads; array++) {
        free_nodes( thread_head_nodes[array], NUM_EMPLOYEES );
        free(thread_head_nodes[array]);
    }
    free(thread_head_nodes);
}

