#include "flight.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define global variables
Flight flights[MAX_FLIGHTS];
int flight_count = 0;

// Find flight by code, return index or -1 if not found
int find_flight_index(const char* flight_code) {
    for (int i = 0; i < flight_count; i++) {
        if (strcmp(flights[i].flight_code, flight_code) == 0) {
            return i;
        }
    }
    return -1;
}

// Insert a new flight
int insert_flight(const char* flight_code) {
    // Check if flight already exists
    if (find_flight_index(flight_code) != -1) {
        return -1;  // Flight already exists
    }
    
    // Check if we have space
    if (flight_count >= MAX_FLIGHTS) {
        return -1;  // Maximum flights reached
    }
    
    // Add new flight
    strcpy(flights[flight_count].flight_code, flight_code);
    flights[flight_count].passenger_list = NULL;
    flights[flight_count].passenger_count = 0;
    flight_count++;
    
    return 0;  // Success
}

// Enroll a passenger in a flight (add to linked list at front for O(1) insertion)
void enroll_passenger(const char* flight_code, int passenger_id) {
    int idx = find_flight_index(flight_code);
    if (idx == -1) {
        return;  // Flight not found
    }
    
    // Optional: Check for duplicate passenger (uncomment if needed)
    // PassengerNode* current = flights[idx].passenger_list;
    // while (current != NULL) {
    //     if (current->passenger_id == passenger_id) {
    //         return;  // Already enrolled
    //     }
    //     current = current->next;
    // }
    
    // Create new passenger node
    PassengerNode* new_node = (PassengerNode*)malloc(sizeof(PassengerNode));
    if (!new_node) {
        return;  // Memory allocation failed
    }
    
    new_node->passenger_id = passenger_id;
    new_node->next = flights[idx].passenger_list;
    flights[idx].passenger_list = new_node;
    flights[idx].passenger_count++;
}

// Comparison function for sorting flight codes
int compare_flight_codes(const void* a, const void* b) {
    Flight* flight_a = (Flight*)a;
    Flight* flight_b = (Flight*)b;
    return strcmp(flight_a->flight_code, flight_b->flight_code);
}

// List all flights (sorted by flight code)
void list_all_flights(void) {
    if (flight_count == 0) {
        return;
    }
    
    // Create a temporary copy of flights for sorting (to not modify original order)
    Flight* temp_flights = (Flight*)malloc(flight_count * sizeof(Flight));
    if (!temp_flights) {
        return;
    }
    
    for (int i = 0; i < flight_count; i++) {
        temp_flights[i] = flights[i];
    }
    
    // Sort the temporary array
    qsort(temp_flights, flight_count, sizeof(Flight), compare_flight_codes);
    
    // Print flights
    for (int i = 0; i < flight_count; i++) {
        printf("%s (%d passengers)\n", temp_flights[i].flight_code, temp_flights[i].passenger_count);
    }
    
    free(temp_flights);
}

// Comparison function for sorting passenger IDs
int compare_passenger_ids(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

// List all passengers for a given flight (sorted by passenger ID)
void list_passengers_for_flight(const char* flight_code) {
    int idx = find_flight_index(flight_code);
    if (idx == -1) {
        return;  // Flight not found
    }
    
    if (flights[idx].passenger_count == 0) {
        return;  // No passengers
    }
    
    // Collect passengers into array for sorting
    int* passengers = (int*)malloc(flights[idx].passenger_count * sizeof(int));
    if (!passengers) {
        return;
    }
    
    PassengerNode* current = flights[idx].passenger_list;
    int count = 0;
    while (current != NULL) {
        passengers[count++] = current->passenger_id;
        current = current->next;
    }
    
    // Sort passengers
    qsort(passengers, count, sizeof(int), compare_passenger_ids);
    
    // Print passengers
    for (int i = 0; i < count; i++) {
        printf("%d\n", passengers[i]);
    }
    
    free(passengers);
}

// Clear all data and free memory (for testing)
void clear_all_data(void) {
    for (int i = 0; i < flight_count; i++) {
        PassengerNode* current = flights[i].passenger_list;
        while (current != NULL) {
            PassengerNode* temp = current;
            current = current->next;
            free(temp);
        }
        flights[i].passenger_list = NULL;
        flights[i].passenger_count = 0;
    }
    flight_count = 0;
}