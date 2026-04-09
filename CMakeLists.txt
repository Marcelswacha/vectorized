cmake_minimum_required(VERSION 4.2)
project(vectorized)

# Add subdirectories
add_subdirectory(utils)
add_subdirectory(collections)

# Main executable (optional)
add_executable(vectorized main.cpp)
target_link_libraries(vectorized PRIVATE collections)
target_compile_options(vectorized PRIVATE -mavx2)

# Minimal test executable
add_executable(VSetTest collections/VSetTest.cpp)
target_link_libraries(VSetTest PRIVATE collections)
target_compile_options(vectorized PRIVATE -mavx2)

# Benchmark executable
add_executable(VSetBenchmark collections/VSetBench.cpp)
target_link_libraries(VSetBenchmark PRIVATE collections)
target_compile_options(vectorized PRIVATE -mavx2)
