#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>  // For high-precision timing

#include "Prototype2/flight_p2.h"

int main() {
    const char* test_files[] = {
        "TestData/small_test.txt",
        "TestData/medium_test.txt",
        "TestData/large_test.txt",
        "TestData/xlarge_test.txt"
    };
    
    const char* names[] = {"SMALL", "MEDIUM", "LARGE", "XLARGE"};
    
    // Get high-precision timer frequency
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    
    printf("\n========================================\n");
    printf("PROTOTYPE 2: Hash Table + AVL Tree\n");
    printf("========================================\n\n");
    
    for (int i = 0; i < 4; i++) {
        printf("%s test file:\n", names[i]);
        
        FILE* file = fopen(test_files[i], "r");
        if (!file) {
            printf("  ERROR: Cannot open %s\n\n", test_files[i]);
            continue;
        }
        
        clear_all_data();
        
        char line[512];
        int queries = 0;
        LARGE_INTEGER total_time;
        total_time.QuadPart = 0;
        
        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = 0;
            
            char cmd[20], code[20];
            int pid;
            
            if (sscanf(line, "%s %s", cmd, code) == 2) {
                if (strcmp(cmd, "INSERT_FLIGHT") == 0) {
                    insert_flight(code);
                }
                else if (strcmp(cmd, "QUERY") == 0) {
                    LARGE_INTEGER start, end;
                    
                    QueryPerformanceCounter(&start);
                    list_passengers_for_flight(code);
                    QueryPerformanceCounter(&end);
                    
                    total_time.QuadPart += (end.QuadPart - start.QuadPart);
                    queries++;
                }
            }
            else if (sscanf(line, "%s %s %d", cmd, code, &pid) == 3) {
                if (strcmp(cmd, "ENROLL") == 0) {
                    enroll_passenger(code, pid);
                }
            }
        }
        
        fclose(file);
        
        double seconds = (double)total_time.QuadPart / frequency.QuadPart;
        double avg_ms = (seconds * 1000) / queries;
        
        printf("  Queries run: %d\n", queries);
        printf("  Total time: %.6f seconds\n", seconds);
        printf("  Average time: %.6f ms per query\n\n", avg_ms);
    }
    
    printf("========================================\n");
    printf("Prototype 2 tests complete!\n");
    printf("========================================\n");
    
    return 0;
}