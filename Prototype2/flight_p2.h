#ifndef FLIGHT_P2_H
#define FLIGHT_P2_H

#define TABLE_SIZE 10007
#define MAX_FLIGHT_CODE 20

typedef struct AVLNode {
    int passenger_id;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

typedef struct FlightHT {
    char flight_code[MAX_FLIGHT_CODE];
    AVLNode* passenger_tree;
    int passenger_count;
    struct FlightHT* next;
} FlightHT;

extern FlightHT* hash_table[TABLE_SIZE];

// Hash function
unsigned int hash(const char* str);

// Flight operations (NO _p2 suffix - matching your flight_p2.c)
FlightHT* find_flight(const char* flight_code);
int insert_flight(const char* flight_code);
void enroll_passenger(const char* flight_code, int passenger_id);
void list_all_flights(void);
void list_passengers_for_flight(const char* flight_code);
void clear_all_data(void);

// AVL Tree functions
AVLNode* avl_create_node(int passenger_id);
int avl_height(AVLNode* node);
int avl_balance_factor(AVLNode* node);
void avl_update_height(AVLNode* node);
AVLNode* avl_rotate_right(AVLNode* y);
AVLNode* avl_rotate_left(AVLNode* x);
AVLNode* avl_insert(AVLNode* node, int passenger_id);
void avl_inorder(AVLNode* node);
void avl_free(AVLNode* node);

#endif