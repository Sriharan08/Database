import random
import os

def generate_test_file(filename, num_flights, max_passengers_per_flight, num_queries):
    """
    Generate a test file with flight insertions, passenger enrollments, and queries
    
    Format:
    INSERT_FLIGHT <flight_code>
    ENROLL <flight_code> <passenger_id>
    QUERY <flight_code>
    """
    
    print(f"Generating {filename}...")
    print(f"  - Flights: {num_flights}")
    print(f"  - Max passengers per flight: {max_passengers_per_flight}")
    print(f"  - Queries: {num_queries}")
    
    # Create flight codes
    flight_codes = [f"FL{i:05d}" for i in range(1, num_flights + 1)]
    
    with open(filename, 'w') as f:
        # Step 1: Insert all flights
        for code in flight_codes:
            f.write(f"INSERT_FLIGHT {code}\n")
        
        # Step 2: Enroll passengers (random distribution)
        total_passengers = 0
        for code in flight_codes:
            num_passengers = random.randint(1, max_passengers_per_flight)
            total_passengers += num_passengers
            
            for _ in range(num_passengers):
                passenger_id = random.randint(10000, 9999999)
                f.write(f"ENROLL {code} {passenger_id}\n")
        
        # Step 3: Add query commands (for performance testing)
        for _ in range(num_queries):
            code = random.choice(flight_codes)
            f.write(f"QUERY {code}\n")
    
    print(f"  - Total passengers enrolled: {total_passengers}")
    print(f"  - File created: {filename}\n")
    return total_passengers

def generate_all_test_files():
    """Generate test files of different sizes"""
    
    # Create TestData folder if it doesn't exist
    os.makedirs("../TestData", exist_ok=True)
    
    # Small test (for quick verification)
    generate_test_file(
        "../TestData/small_test.txt",
        num_flights=10,
        max_passengers_per_flight=100,
        num_queries=20
    )
    
    # Medium test
    generate_test_file(
        "../TestData/medium_test.txt",
        num_flights=100,
        max_passengers_per_flight=500,
        num_queries=100
    )
    
    # Large test
    generate_test_file(
        "../TestData/large_test.txt",
        num_flights=500,
        max_passengers_per_flight=1000,
        num_queries=200
    )
    
    # Extra large test (for serious benchmarking)
    generate_test_file(
        "../TestData/xlarge_test.txt",
        num_flights=1000,
        max_passengers_per_flight=5000,
        num_queries=500
    )
    
    print("\n✅ All test files generated successfully!")
    print("Files are in the TestData folder")

if __name__ == "__main__":
    generate_all_test_files()