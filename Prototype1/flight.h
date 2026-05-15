#ifndef FLIGHT_H
#define FLIGHT_H

#define MAX_FLIGHTS 10000
#define MAX_FLIGHT_CODE 20

// Linked list node for passengers
typedef struct PassengerNode {
    int passenger_id;
    struct PassengerNode* next;
} PassengerNode;

// Flight structure
typedef struct {
    char flight_code[MAX_FLIGHT_CODE];
    PassengerNode* passenger_list;
    int passenger_count;
} Flight;

// Global variables
extern Flight flights[MAX_FLIGHTS];
extern int flight_count;

// Function declarations
int find_flight_index(const char* flight_code);
int insert_flight(const char* flight_code);
void enroll_passenger(const char* flight_code, int passenger_id);
void list_all_flights(void);
void list_passengers_for_flight(const char* flight_code);
void clear_all_data(void);  // For testing - frees all memory

#endif