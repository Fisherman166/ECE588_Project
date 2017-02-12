//*****************************************************************************
// ECE 588 Project
// By: Sean Koppenhafer and Luis Santiago
// Common.h
//*****************************************************************************

#ifndef COMMON_H
#define COMMON_H

#define NUM_EMPLOYEES 20000
#define NUM_TRIPS 100000

typedef struct {
    uint32_t ID;
    char name[30];
} employee_data;

typedef struct {
    uint32_t ID;
    char timestamp[15];
    char destination[30];
} trip_data;

#endif

