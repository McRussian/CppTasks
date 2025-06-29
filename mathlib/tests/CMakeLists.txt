# Находим GoogleTest
find_package(GTest REQUIRED)

# Указываем полный путь к исходному файлу теста
add_executable(mathlib_test 
    ${CMAKE_CURRENT_SOURCE_DIR}/vector_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix_test.cpp  
    ${CMAKE_SOURCE_DIR}/src/vector.hpp
    ${CMAKE_SOURCE_DIR}/src/matrix.hpp
    ${CMAKE_SOURCE_DIR}/src/exception.hpp
)

# Линкуем с GTest
target_link_libraries(mathlib_test 
    GTest::GTest 
    GTest::Main
)

# Указываем выходной каталог
set_target_properties(mathlib_test 
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Добавляем тест в CTest
add_test(
    NAME mathlib_test 
    COMMAND ${CMAKE_BINARY_DIR}/bin/mathlib_test
)
