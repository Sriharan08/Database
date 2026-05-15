#include "flight_p2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize hash table
FlightHT* hash_table[TABLE_SIZE];

// Hash function - djb2 (excellent for strings)
unsigned int hash(const char* str) {
    unsigned int hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    return hash % TABLE_SIZE;
}

// ========== AVL TREE FUNCTIONS ==========

// Create new AVL node
AVLNode* avl_create_node(int passenger_id) {
    AVLNode* new_node = (AVLNode*)malloc(sizeof(AVLNode));
    if (!new_node) return NULL;
    
    new_node->passenger_id = passenger_id;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->height = 1;  // New node is leaf
    return new_node;
}

// Get height of node
int avl_height(AVLNode* node) {
    return node ? node->height : 0;
}

// Calculate balance factor
int avl_balance_factor(AVLNode* node) {
    return node ? avl_height(node->left) - avl_height(node->right) : 0;
}

// Update height of node
void avl_update_height(AVLNode* node) {
    if (node) {
        int left_height = avl_height(node->left);
        int right_height = avl_height(node->right);
        node->height = (left_height > right_height ? left_height : right_height) + 1;
    }
}

// Right rotation
AVLNode* avl_rotate_right(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    
    // Perform rotation
    x->right = y;
    y->left = T2;
    
    // Update heights
    avl_update_height(y);
    avl_update_height(x);
    
    return x;  // New root
}

// Left rotation
AVLNode* avl_rotate_left(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    
    // Perform rotation
    y->left = x;
    x->right = T2;
    
    // Update heights
    avl_update_height(x);
    avl_update_height(y);
    
    return y;  // New root
}

// Insert into AVL tree (no duplicates allowed)
AVLNode* avl_insert(AVLNode* node, int passenger_id) {
    // Step 1: Normal BST insertion
    if (!node) {
        return avl_create_node(passenger_id);
    }
    
    // No duplicates - check if already exists
    if (passenger_id == node->passenger_id) {
        return node;  // Duplicate, ignore
    }
    
    if (passenger_id < node->passenger_id) {
        node->left = avl_insert(node->left, passenger_id);
    } else if (passenger_id > node->passenger_id) {
        node->right = avl_insert(node->right, passenger_id);
    } else {
        return node;  // Duplicate found
    }
    
    // Step 2: Update height
    avl_update_height(node);
    
    // Step 3: Get balance factor
    int balance = avl_balance_factor(node);
    
    // Step 4: Balance the tree (4 cases)
    
    // Left Left Case
    if (balance > 1 && passenger_id < node->left->passenger_id) {
        return avl_rotate_right(node);
    }
    
    // Right Right Case
    if (balance < -1 && passenger_id > node->right->passenger_id) {
        return avl_rotate_left(node);
    }
    
    // Left Right Case
    if (balance > 1 && passenger_id > node->left->passenger_id) {
        node->left = avl_rotate_left(node->left);
        return avl_rotate_right(node);
    }
    
    // Right Left Case
    if (balance < -1 && passenger_id < node->right->passenger_id) {
        node->right = avl_rotate_right(node->right);
        return avl_rotate_left(node);
    }
    
    return node;
}

// In-order traversal (prints sorted passenger IDs)
void avl_inorder(AVLNode* node) {
    if (node) {
        avl_inorder(node->left);
        printf("%d\n", node->passenger_id);
        avl_inorder(node->right);
    }
}

// Free AVL tree memory
void avl_free(AVLNode* node) {
    if (node) {
        avl_free(node->left);
        avl_free(node->right);
        free(node);
    }
}

// ========== FLIGHT HASH TABLE FUNCTIONS ==========

// Find flight by code
FlightHT* find_flight(const char* flight_code) {
    unsigned int index = hash(flight_code);
    FlightHT* current = hash_table[index];
    
    while (current) {
        if (strcmp(current->flight_code, flight_code) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;  // Not found
}

// Insert new flight
int insert_flight(const char* flight_code) {
    // Check if flight already exists
    if (find_flight(flight_code) != NULL) {
        return -1;  // Flight already exists
    }
    
    // Create new flight
    FlightHT* new_flight = (FlightHT*)malloc(sizeof(FlightHT));
    if (!new_flight) return -1;
    
    strcpy(new_flight->flight_code, flight_code);
    new_flight->passenger_tree = NULL;
    new_flight->passenger_count = 0;
    new_flight->next = NULL;
    
    // Insert into hash table (chaining)
    unsigned int index = hash(flight_code);
    new_flight->next = hash_table[index];
    hash_table[index] = new_flight;
    
    return 0;  // Success
}

// Enroll passenger in flight
void enroll_passenger(const char* flight_code, int passenger_id) {
    FlightHT* flight = find_flight(flight_code);
    if (!flight) {
        return;  // Flight not found
    }
    
    // Insert into AVL tree
    flight->passenger_tree = avl_insert(flight->passenger_tree, passenger_id);
    flight->passenger_count++;
}

// Comparison function for sorting flights by code
int compare_flights(const void* a, const void* b) {
    FlightHT* fa = *(FlightHT**)a;
    FlightHT* fb = *(FlightHT**)b;
    return strcmp(fa->flight_code, fb->flight_code);
}

// List all flights (sorted)
void list_all_flights(void) {
    // Collect all flights from hash table
    FlightHT* flights[TABLE_SIZE];
    int flight_count = 0;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        FlightHT* current = hash_table[i];
        while (current) {
            flights[flight_count++] = current;
            current = current->next;
        }
    }
    
    if (flight_count == 0) return;
    
    // Sort flights
    qsort(flights, flight_count, sizeof(FlightHT*), compare_flights);
    
    // Print flights
    for (int i = 0; i < flight_count; i++) {
        printf("%s (%d passengers)\n", 
               flights[i]->flight_code, 
               flights[i]->passenger_count);
    }
}

// List all passengers for a flight (already sorted from AVL tree!)
void list_passengers_for_flight(const char* flight_code) {
    FlightHT* flight = find_flight(flight_code);
    if (!flight || flight->passenger_count == 0) {
        return;  // Flight not found or no passengers
    }
    
    // AVL tree already maintains order - just traverse in-order
    avl_inorder(flight->passenger_tree);
}

// Clear all data and free memory
void clear_all_data(void) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        FlightHT* current = hash_table[i];
        while (current) {
            FlightHT* temp = current;
            current = current->next;
            
            // Free AVL tree
            avl_free(temp->passenger_tree);
            free(temp);
        }
        hash_table[i] = NULL;
    }
}