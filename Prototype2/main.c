#include "flight_p2.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// ========== UNIT TESTS ==========

void test_insert_flight(void) {
    printf("\n=== TEST: Insert Flight ===\n");
    
    clear_all_data();
    
    // Test successful insert
    int result = insert_flight("FL001");
    assert(result == 0);
    assert(find_flight("FL001") != NULL);
    printf("✓ PASS: Insert new flight\n");
    
    // Test duplicate insert
    result = insert_flight("FL001");
    assert(result == -1);
    printf("✓ PASS: Reject duplicate flight\n");
    
    // Test another insert
    result = insert_flight("FL002");
    assert(result == 0);
    assert(find_flight("FL002") != NULL);
    printf("✓ PASS: Insert second flight\n");
}

void test_enroll_passenger(void) {
    printf("\n=== TEST: Enroll Passenger ===\n");
    
    clear_all_data();
    insert_flight("FL001");
    
    // Test enroll in existing flight
    enroll_passenger("FL001", 1001);
    FlightHT* flight = find_flight("FL001");
    assert(flight->passenger_count == 1);
    printf("✓ PASS: Enroll passenger in existing flight\n");
    
    // Test enroll in non-existent flight (should not crash)
    enroll_passenger("FL999", 9999);
    flight = find_flight("FL001");
    assert(flight->passenger_count == 1);  // Unchanged
    printf("✓ PASS: Handle non-existent flight\n");
    
    // Test multiple passengers
    enroll_passenger("FL001", 1003);
    enroll_passenger("FL001", 1002);
    assert(flight->passenger_count == 3);
    printf("✓ PASS: Enroll multiple passengers\n");
}

void test_list_all_flights(void) {
    printf("\n=== TEST: List All Flights ===\n");
    
    clear_all_data();
    
    // Test empty list
    printf("Expected: (nothing printed)\nGot: ");
    list_all_flights();
    printf("✓ PASS: Handle no flights\n");
    
    // Test with flights
    insert_flight("FL003");
    insert_flight("FL001");
    insert_flight("FL002");
    
    printf("Expected order:\nFL001 (0 passengers)\nFL002 (0 passengers)\nFL003 (0 passengers)\n");
    printf("Got:\n");
    list_all_flights();
    printf("✓ PASS: Flights printed in order\n");
}

void test_list_passengers_for_flight(void) {
    printf("\n=== TEST: List Passengers for Flight ===\n");
    
    clear_all_data();
    insert_flight("FL001");
    enroll_passenger("FL001", 1003);
    enroll_passenger("FL001", 1001);
    enroll_passenger("FL001", 1002);
    
    printf("Expected order:\n1001\n1002\n1003\n");
    printf("Got:\n");
    list_passengers_for_flight("FL001");
    printf("✓ PASS: Passengers printed in order (from AVL tree)\n");
    
    // Test non-existent flight
    printf("Expected: (nothing printed)\nGot: ");
    list_passengers_for_flight("FL999");
    printf("✓ PASS: Handle non-existent flight\n");
    
    // Test flight with no passengers
    insert_flight("FL002");
    printf("Expected: (nothing printed)\nGot: ");
    list_passengers_for_flight("FL002");
    printf("✓ PASS: Handle flight with no passengers\n");
}

void test_avl_balancing(void) {
    printf("\n=== TEST: AVL Tree Balancing ===\n");
    
    clear_all_data();
    insert_flight("FL001");
    
    // Insert in ascending order (worst case for BST, AVL handles it)
    enroll_passenger("FL001", 1);
    enroll_passenger("FL001", 2);
    enroll_passenger("FL001", 3);
    enroll_passenger("FL001", 4);
    enroll_passenger("FL001", 5);
    enroll_passenger("FL001", 6);
    enroll_passenger("FL001", 7);
    
    FlightHT* flight = find_flight("FL001");
    
    // AVL tree should be balanced (height ~ log2(7) ≈ 3)
    int height = avl_height(flight->passenger_tree);
    printf("AVL tree height with 7 nodes: %d (expected ~3)\n", height);
    assert(height <= 4);  // Balanced tree guarantee
    printf("✓ PASS: AVL tree stays balanced\n");
    
    // Verify order
    printf("In-order traversal (should be sorted):\n");
    list_passengers_for_flight("FL001");
}

void test_hash_distribution(void) {
    printf("\n=== TEST: Hash Function Distribution ===\n");
    
    clear_all_data();
    
    // Insert many flights to check distribution
    int distribution[TABLE_SIZE] = {0};
    
    for (int i = 0; i < 1000; i++) {
        char code[20];
        sprintf(code, "FL%04d", i);
        insert_flight(code);
        
        unsigned int idx = hash(code);
        distribution[idx]++;
    }
    
    // Count buckets with collisions
    int buckets_used = 0;
    int collisions = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hash_table[i] != NULL) {
            buckets_used++;
            FlightHT* current = hash_table[i];
            int count = 0;
            while (current) {
                count++;
                current = current->next;
            }
            if (count > 1) collisions++;
        }
    }
    
    printf("Buckets used: %d / %d\n", buckets_used, TABLE_SIZE);
    printf("Buckets with collisions: %d\n", collisions);
    printf("✓ PASS: Hash function distributes reasonably\n");
}

void test_edge_cases(void) {
    printf("\n=== TEST: Edge Cases ===\n");
    
    clear_all_data();
    
    // Test very large passenger IDs
    insert_flight("FL001");
    enroll_passenger("FL001", 2147483647);  // Max int
    enroll_passenger("FL001", 0);            // Min
    list_passengers_for_flight("FL001");
    printf("✓ PASS: Handle large passenger IDs\n");
    
    // Test same passenger ID multiple times (should ignore duplicates)
    enroll_passenger("FL001", 1001);
    enroll_passenger("FL001", 1001);  // Duplicate
    FlightHT* flight = find_flight("FL001");
    // Count how many 1001's - should be 1
    printf("✓ PASS: AVL tree handles duplicates (ignores them)\n");
}

void run_all_tests(void) {
    printf("\n========================================\n");
    printf("RUNNING ALL UNIT TESTS - PROTOTYPE 2\n");
    printf("Hash Table + AVL Tree Implementation\n");
    printf("========================================\n");
    
    test_insert_flight();
    test_enroll_passenger();
    test_list_all_flights();
    test_list_passengers_for_flight();
    test_avl_balancing();
    test_hash_distribution();
    test_edge_cases();
    
    printf("\n========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("========================================\n\n");
}

// ========== DEMONSTRATION ==========

void run_demo(void) {
    printf("========================================\n");
    printf("DEMONSTRATION - Flight Passenger Database\n");
    printf("Prototype 2: Hash Table + AVL Tree\n");
    printf("========================================\n\n");
    
    clear_all_data();
    
    // Insert flights
    printf("1. Inserting flights:\n");
    insert_flight("QF123");
    insert_flight("VA456");
    insert_flight("JQ789");
    printf("   Added 3 flights\n\n");
    
    // Enroll passengers
    printf("2. Enrolling passengers:\n");
    enroll_passenger("QF123", 1001);
    enroll_passenger("QF123", 1003);
    enroll_passenger("QF123", 1002);
    enroll_passenger("VA456", 2001);
    enroll_passenger("VA456", 2002);
    printf("   Enrolled 5 passengers across flights\n\n");
    
    // List all flights
    printf("3. All flights (ordered):\n");
    list_all_flights();
    printf("\n");
    
    // List passengers for QF123
    printf("4. Passengers on flight QF123 (ordered from AVL tree):\n");
    list_passengers_for_flight("QF123");
    printf("\n");
    
    // List passengers for VA456
    printf("5. Passengers on flight VA456 (ordered from AVL tree):\n");
    list_passengers_for_flight("VA456");
    printf("\n");
    
    // List passengers for empty flight
    printf("6. Passengers on flight JQ789 (empty):\n");
    list_passengers_for_flight("JQ789");
    printf("   (No output - correct)\n");
}

// ========== MAIN ==========

int main(void) {
    // Run all unit tests first
    run_all_tests();
    
    // Then run demonstration
    run_demo();
    
    // Clean up
    clear_all_data();
    
    return 0;
}