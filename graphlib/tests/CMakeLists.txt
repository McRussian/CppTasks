# Находим GoogleTest
find_package(GTest REQUIRED)

# Если vertex.hpp - это заголовочный файл, его не нужно добавлять в add_executable
# Вместо этого нужно только файлы реализации (.cpp)
add_executable(graphlib_test 
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/baseedge_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/basegraph_test.cpp
)

# Добавляем include директории
target_include_directories(graphlib_test
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

# Линкуем с GTest
target_link_libraries(graphlib_test 
    GTest::gtest
    GTest::gtest_main
)

# Указываем выходной каталог
set_target_properties(graphlib_test 
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Добавляем тест в CTest
add_test(
    NAME graphlib_test 
    COMMAND ${CMAKE_BINARY_DIR}/bin/graphlib_test
)