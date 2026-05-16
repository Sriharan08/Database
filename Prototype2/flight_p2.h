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

// Function declarations with _p2 suffix
unsigned int hash_p2(const char* str);
FlightHT* find_flight_p2(const char* flight_code);
int insert_flight_p2(const char* flight_code);
void enroll_passenger_p2(const char* flight_code, int passenger_id);
void list_all_flights_p2(void);
void list_passengers_for_flight_p2(const char* flight_code);
void clear_all_data_p2(void);

// AVL functions with _p2 suffix
AVLNode* avl_create_node_p2(int passenger_id);
int avl_height_p2(AVLNode* node);
int avl_balance_factor_p2(AVLNode* node);
void avl_update_height_p2(AVLNode* node);
AVLNode* avl_rotate_right_p2(AVLNode* y);
AVLNode* avl_rotate_left_p2(AVLNode* x);
AVLNode* avl_insert_p2(AVLNode* node, int passenger_id);
void avl_inorder_p2(AVLNode* node);
void avl_free_p2(AVLNode* node);

#endif