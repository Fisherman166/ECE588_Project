//*****************************************************************************
// ECE 588 Project
// By: Sean Koppenhafer and Luis Santiago
// join.h
//*****************************************************************************

#ifndef JOIN_H
#define JOIN_H

void create_join_database(trip_data*, uint32_t, uint32_t);
void run_threads(uint32_t);
void print_joined_database(employee_data*, uint32_t);
void cleanup_joined_database(uint32_t);

#endif

