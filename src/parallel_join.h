#ifndef PARALLEL_JOIN_H
#define PARALLEL_JOIN_H

void parallel_create_join_database(employee_data*, trip_data*, uint32_t, uint32_t);
void run_threads(uint32_t);
void parallel_print_joined_database(uint32_t);
void parallel_cleanup_joined_database(uint32_t);

#endif

