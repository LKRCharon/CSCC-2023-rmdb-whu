set(SOURCES lru_replacer.cpp)
add_library(lru_replacer STATIC ${SOURCES})

add_executable(lru_replacer_test lru_replacer_test.cpp)
target_link_libraries(lru_replacer_test lru_replacer gtest_main)  # add gtest