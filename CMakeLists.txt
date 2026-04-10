cmake_minimum_required(VERSION 4.2)
project(vectorized)

# Add subdirectories
add_subdirectory(utils)
add_subdirectory(collections)
add_subdirectory(distributions)

# Main executable (optional)
add_executable(vectorized main.cpp)
target_link_libraries(vectorized PRIVATE collections)
target_compile_options(vectorized PRIVATE -mavx2)

# Collections tests
add_executable(VSetTest collections/VSetTest.cpp)
target_link_libraries(VSetTest PRIVATE collections)
target_compile_options(VSetTest PRIVATE -mavx2)

# Collections benchmark
add_executable(VSetBenchmark collections/VSetBench.cpp)
target_link_libraries(VSetBenchmark PRIVATE collections)
target_compile_options(VSetBenchmark PRIVATE -mavx2)

# Distributions tests
add_executable(DistributionsTest distributions/UniformTest.cpp)
target_link_libraries(DistributionsTest PRIVATE distributions)
target_compile_options(DistributionsTest PRIVATE -mavx2)

# Collections benchmark
#add_executable(VSetBenchmark collections/VSetBench.cpp)
#target_link_libraries(VSetBenchmark PRIVATE collections)
#target_compile_options(vectorized PRIVATE -mavx2)
