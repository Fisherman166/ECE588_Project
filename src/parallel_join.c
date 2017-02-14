#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "parallel_join.h"

typedef struct Node Node;

struct Node {
    trip_data *trip_info;
    Node *next;
};

typedef struct {
    employee_data *employee_info;
    Node* head;
} head_node;

typedef struct {
    trip_data* trips_array;
    uint32_t starting_trip_num;
    uint32_t ending_trip_num;
} thread_args;


static head_node employee_node[NUM_EMPLOYEES];
static pthread_t* threads = NULL;
static thread_args* args = NULL;

static pthread_mutex_t head_lock;


//*****************************************************************************
// Private Functions
//*****************************************************************************
static void* parallel_run_join_operation(void*);

static void init_mutex() {
    int retval = pthread_mutex_init(&head_lock, NULL);
    if(retval) {
        printf("ERROR: Could not init mutex\n");
        exit(2);
    }
}


static pthread_t* allocate_threads(uint32_t number_of_threads) {
    pthread_t* threads = malloc( sizeof(pthread_t) * number_of_threads );
    if(threads == NULL) {
        printf("ERROR: Threads malloc returned NULL. Attempted to malloc %u threads\n", number_of_threads);
        exit(-1);
    }
    return threads;
}


static thread_args* create_threads(pthread_t* threads, trip_data* trips_array, uint32_t number_of_threads) {
    thread_args* args = malloc( sizeof(thread_args) * number_of_threads);
    if(args == NULL) {
        printf("ERROR: Thread args malloc failed. Tried to malloc %u\n", number_of_threads);
        exit(-1);
    }

    uint32_t trips_per_thread = NUM_TRIPS / number_of_threads;
    uint32_t last_thread = number_of_threads - 1;
    for(uint32_t i = 0; i < number_of_threads; i++) {
        args[i].trips_array = trips_array;
        args[i].starting_trip_num = trips_per_thread * i;
        if(i == last_thread) args[i].ending_trip_num = NUM_TRIPS;
        else args[i].ending_trip_num = trips_per_thread * (i + 1);
        pthread_create(&threads[i], NULL, &parallel_run_join_operation, (void*)&args[i]);
    }

    return args;
}


static Node* allocate_node(trip_data* trip_info) {
    Node* new_node = malloc( sizeof(Node) );
    if(new_node == NULL) {
        printf("ERROR: Failed to malloc new node\n");
        exit(1);
    }
    new_node->trip_info = trip_info;

    return new_node;
}


static void add_trip_to_head(Node** head_node, trip_data* trip) {
    Node* new_node = allocate_node(trip);

    pthread_mutex_lock(&head_lock);
        if(*head_node == NULL) new_node->next = NULL;
        else new_node->next = *head_node;
        *head_node = new_node;
    pthread_mutex_unlock(&head_lock);
}


static void* parallel_run_join_operation(void* void_args) {
    thread_args* args = (thread_args*)void_args;
    uint32_t employee_ID;

    for(uint32_t i = args->starting_trip_num; i < args->ending_trip_num; i++) {
        employee_ID = args->trips_array[i].ID;
        add_trip_to_head( &(employee_node[employee_ID].head), &(args->trips_array[i]) );
    }

    return 0;
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


static void destroy_mutex() {
    int retval = pthread_mutex_destroy(&head_lock);
    if(retval) {
        printf("ERROR: Could not destroy mutex\n");
        exit(2);
    }
}


//*****************************************************************************
// Functions
//*****************************************************************************
void parallel_create_join_database(employee_data* employee_data_array, trip_data* trips_array,
                                   uint32_t num_employees, uint32_t num_threads) {
    init_mutex();
    threads = allocate_threads(num_threads);
    args = create_threads(threads, trips_array, num_threads);

    for(uint32_t i = 0; i < num_employees; i++) {
        employee_node[i].employee_info = &(employee_data_array[i]);
        employee_node[i].head = NULL;
    }
}


void run_threads(uint32_t number_of_threads) {
    for(uint32_t i = 0; i < number_of_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}


void parallel_print_joined_database(uint32_t num_employees) {
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


void parallel_cleanup_joined_database(uint32_t num_employees) {
    destroy_mutex();
    free_nodes( &(employee_node[0]), num_employees);
    free(threads);
    free(args);
}

