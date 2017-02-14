#ifndef SERIAL_JOIN_H
#define SERIAL_JOIN_H

void serial_create_join_database(employee_data*, uint32_t);
void serial_run_join_operation(trip_data*, uint32_t);
void serial_print_joined_database(uint32_t);
void serial_cleanup_joined_database(uint32_t);

#endif

