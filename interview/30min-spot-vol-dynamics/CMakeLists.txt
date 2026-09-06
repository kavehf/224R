cmake_minimum_required(VERSION 3.16)
project(spot_vol_dynamics CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Build the interviewer reference implementation instead of the candidate stub.
option(USE_REFERENCE_SOLUTION "Compile solution/bumped_vol_surface.cpp" OFF)

add_library(hidden_svi
    hidden/svi_vol_surface.cpp
    hidden/surface_factory.cpp)
target_include_directories(hidden_svi
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/hidden)

if(USE_REFERENCE_SOLUTION)
    set(BUMPED_SRC solution/bumped_vol_surface.cpp)
else()
    set(BUMPED_SRC src/bumped_vol_surface.cpp)
endif()

add_library(bumped_surface ${BUMPED_SRC})
target_include_directories(bumped_surface PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(test_properties tests/test_properties.cpp)
target_include_directories(test_properties PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
    ${CMAKE_CURRENT_SOURCE_DIR}/hidden)
target_link_libraries(test_properties PRIVATE bumped_surface hidden_svi)

add_executable(test_ssr_formula hidden/test_ssr_formula.cpp)
target_include_directories(test_ssr_formula PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
    ${CMAKE_CURRENT_SOURCE_DIR}/hidden)
target_link_libraries(test_ssr_formula PRIVATE bumped_surface hidden_svi)

add_executable(print_smile demo/print_smile.cpp)
target_include_directories(print_smile PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/hidden)
target_link_libraries(print_smile PRIVATE bumped_surface hidden_svi)

enable_testing()
add_test(NAME properties COMMAND test_properties)
add_test(NAME ssr_formula COMMAND test_ssr_formula)
