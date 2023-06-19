set(SOURCES 
        disk_manager.cpp 
        buffer_pool_manager.cpp 
        ../replacer/replacer.h 
        ../replacer/lru_replacer.cpp 
)
add_library(storage STATIC ${SOURCES})

# buffer_pool_manager_test
add_executable(buffer_pool_manager_test buffer_pool_manager_test.cpp)
target_link_libraries(buffer_pool_manager_test storage gtest_main)  # add gtest
