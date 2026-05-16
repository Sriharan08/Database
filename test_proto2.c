#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Prototype2/flight_p2.h"

int main() {
    clear_all_data_p2();
    
    const char* test_files[] = {
        "TestData/small_test.txt",
        "TestData/medium_test.txt", 
        "TestData/large_test.txt",
        "TestData/xlarge_test.txt"
    };
    
    const char* names[] = {"SMALL", "MEDIUM", "LARGE", "XLARGE"};
    
    printf("\n========================================\n");
    printf("PROTOTYPE 2: Hash Table + AVL Tree\n");
    printf("========================================\n\n");
    
    for (int i = 0; i < 4; i++) {
        FILE* file = fopen(test_files[i], "r");
        if (!file) {
            printf("Cannot open %s\n", test_files[i]);
            continue;
        }
        
        clear_all_data_p2();
        
        char line[256];
        int queries = 0;
        clock_t total = 0;
        
        printf("%s test file:\n", names[i]);
        
        while (fgets(line, sizeof(line), file)) {
            char cmd[20], code[20];
            int pid;
            
            if (sscanf(line, "%s %s", cmd, code) == 2) {
                if (strcmp(cmd, "INSERT_FLIGHT") == 0) {
                    insert_flight_p2(code);
                }
                else if (strcmp(cmd, "QUERY") == 0) {
                    clock_t start = clock();
                    // Redirect output to null to avoid printing passengers
                    freopen("nul", "w", stdout);
                    list_passengers_for_flight_p2(code);
                    freopen("CON", "w", stdout);
                    clock_t end = clock();
                    total += (end - start);
                    queries++;
                }
            }
            else if (sscanf(line, "%s %s %d", cmd, code, &pid) == 3) {
                if (strcmp(cmd, "ENROLL") == 0) {
                    enroll_passenger_p2(code, pid);
                }
            }
        }
        
        fclose(file);
        
        double seconds = (double)total / CLOCKS_PER_SEC;
        double avg_ms = seconds * 1000 / queries;
        
        printf("  Queries run: %d\n", queries);
        printf("  Total time: %.3f seconds\n", seconds);
        printf("  Average time: %.3f ms per query\n\n", avg_ms);
    }
    
    printf("========================================\n");
    printf("Prototype 2 tests complete!\n");
    printf("========================================\n");
    
    return 0;
}