# Находим GoogleTest
find_package(GTest REQUIRED)

# Указываем полный путь к исходному файлу теста
add_executable(vector_test 
    ${CMAKE_CURRENT_SOURCE_DIR}/vector_test.cpp 
    ${CMAKE_SOURCE_DIR}/src/vector.hpp
)

# Линкуем с GTest
target_link_libraries(vector_test 
    GTest::GTest 
    GTest::Main
)

# Указываем выходной каталог
set_target_properties(vector_test 
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Добавляем тест в CTest
add_test(
    NAME vector_test 
    COMMAND ${CMAKE_BINARY_DIR}/bin/vector_test
)
