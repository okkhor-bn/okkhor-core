cmake_minimum_required(VERSION 3.16)

project(okkhor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra)
endif()

# ============================================================
# Core library
# ============================================================

add_library(okkhor_core

    # Core
    src/core/mapping.cpp
    src/core/orthography.cpp
    src/core/okkhor.cpp

    # JSON
    src/util/json.cpp

    # Latin -> Bangla
    src/latin_to_bangla/tokenizer.cpp
    src/latin_to_bangla/parser.cpp
    src/latin_to_bangla/renderer.cpp
    src/latin_to_bangla/rules.cpp

    # Bangla -> Latin
    # src/bangla_to_latin/tokenizer.cpp
    # src/bangla_to_latin/parser.cpp
    # src/bangla_to_latin/renderer.cpp
)

target_include_directories(okkhor_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(okkhor_core PUBLIC
    OKKHOR_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
)

# ============================================================
# Command-line frontend
# ============================================================

add_executable(okkhor
    src/main.cpp
)

target_link_libraries(okkhor PRIVATE
    okkhor_core
)

# Keep a copy of the mapping data next to the binary.
add_custom_command(TARGET okkhor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    $<TARGET_FILE_DIR:okkhor>/data
)

# ============================================================
# Tests
# ============================================================

enable_testing()

add_executable(algebra_tests
    tests/algebra_tests.cpp
)

target_link_libraries(algebra_tests PRIVATE
    okkhor_core
)

target_include_directories(algebra_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

add_test(
    NAME algebra
    COMMAND algebra_tests
)

add_executable(phonetic_tests
    tests/phonetic_tests.cpp
)

target_link_libraries(phonetic_tests PRIVATE
    okkhor_core
)

target_include_directories(phonetic_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

add_test(
    NAME phonetic
    COMMAND phonetic_tests
)

add_executable(rules_tests
    tests/rules_tests.cpp
)

target_link_libraries(rules_tests PRIVATE
    okkhor_core
)

target_include_directories(rules_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

add_test(
    NAME rules
    COMMAND rules_tests
)