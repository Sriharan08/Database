#include "flight.h"
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
    assert(flight_count == 1);
    printf("✓ PASS: Insert new flight\n");
    
    // Test duplicate insert
    result = insert_flight("FL001");
    assert(result == -1);
    printf("✓ PASS: Reject duplicate flight\n");
    
    // Test another insert
    result = insert_flight("FL002");
    assert(result == 0);
    assert(flight_count == 2);
    printf("✓ PASS: Insert second flight\n");
}

void test_enroll_passenger(void) {
    printf("\n=== TEST: Enroll Passenger ===\n");
    
    clear_all_data();
    insert_flight("FL001");
    
    // Test enroll in existing flight
    enroll_passenger("FL001", 1001);
    assert(flights[0].passenger_count == 1);
    printf("✓ PASS: Enroll passenger in existing flight\n");
    
    // Test enroll in non-existent flight (should not crash)
    enroll_passenger("FL999", 9999);
    assert(flights[0].passenger_count == 1);  // Unchanged
    printf("✓ PASS: Handle non-existent flight\n");
    
    // Test multiple passengers
    enroll_passenger("FL001", 1003);
    enroll_passenger("FL001", 1002);
    assert(flights[0].passenger_count == 3);
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
    printf("✓ PASS: Passengers printed in order\n");
    
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

void test_edge_cases(void) {
    printf("\n=== TEST: Edge Cases ===\n");
    
    clear_all_data();
    
    // Test MAX_FLIGHTS limit
    for (int i = 0; i < MAX_FLIGHTS; i++) {
        char code[20];
        sprintf(code, "FL%04d", i);
        assert(insert_flight(code) == 0);
    }
    assert(insert_flight("OVERFLOW") == -1);
    printf("✓ PASS: Handle MAX_FLIGHTS limit\n");
    
    clear_all_data();
    
    // Test very large passenger IDs
    insert_flight("FL001");
    enroll_passenger("FL001", 2147483647);  // Max int
    enroll_passenger("FL001", 0);            // Min
    list_passengers_for_flight("FL001");
    printf("✓ PASS: Handle large passenger IDs\n");
}

void run_all_tests(void) {
    printf("\n========================================\n");
    printf("RUNNING ALL UNIT TESTS - PROTOTYPE 1\n");
    printf("Array + Linked List Implementation\n");
    printf("========================================\n");
    
    test_insert_flight();
    test_enroll_passenger();
    test_list_all_flights();
    test_list_passengers_for_flight();
    test_edge_cases();
    
    printf("\n========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("========================================\n\n");
}

// ========== DEMONSTRATION ==========

void run_demo(void) {
    printf("========================================\n");
    printf("DEMONSTRATION - Flight Passenger Database\n");
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
    printf("4. Passengers on flight QF123 (ordered):\n");
    list_passengers_for_flight("QF123");
    printf("\n");
    
    // List passengers for VA456
    printf("5. Passengers on flight VA456 (ordered):\n");
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