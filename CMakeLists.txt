cmake_minimum_required(VERSION 4.2)
project(vectorized)

add_subdirectory(utils)
add_subdirectory(collections)
add_subdirectory(distributions)

# -----------------------------------------------------
# Global compile options (centralized)
# -----------------------------------------------------
set(VECTOR_COMPILE_OPTS -mavx2)

# -----------------------------------------------------
# Helper: test target
# -----------------------------------------------------
function(add_vec_test name file)
    add_executable(${name} ${file})
    target_link_libraries(${name} PRIVATE collections distributions utils)
    target_compile_options(${name} PRIVATE ${VECTOR_COMPILE_OPTS})
endfunction()

# -----------------------------------------------------
# Helper: benchmark target
# -----------------------------------------------------
function(add_vec_bench name file)
    add_executable(${name} ${file})
    target_link_libraries(${name} PRIVATE collections distributions utils)
    target_compile_options(${name} PRIVATE ${VECTOR_COMPILE_OPTS})
endfunction()

# -----------------------------------------------------
# Main executable
# -----------------------------------------------------
add_executable(vectorized main.cpp)
target_link_libraries(vectorized PRIVATE collections distributions utils)
target_compile_options(vectorized PRIVATE ${VECTOR_COMPILE_OPTS})

# -----------------------------------------------------
# Collections
# -----------------------------------------------------
add_vec_test(VSetTest collections/VSetTest.cpp)
add_vec_bench(VSetBenchmark collections/VSetBench.cpp)

# -----------------------------------------------------
# Distributions
# -----------------------------------------------------
add_vec_test(UniformTest distributions/UniformTest.cpp)
add_vec_test(MarsagliaTest distributions/MarsagliaTest.cpp)

add_vec_bench(UniformBenchmark distributions/UniformBench.cpp)
add_vec_bench(MarsagliaBenchmark distributions/MarsagliaBench.cpp)