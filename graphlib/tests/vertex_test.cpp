// vertex_tests.cpp
#include <gtest/gtest.h>
#include "../src/vertex.hpp"

// Тест на создание вершины с двумя параметрами
TEST(VertexTest, ConstructorWithTwoParameters) {
    Vertex v1(1, "A");
    EXPECT_EQ(v1.number(), 1);
    EXPECT_EQ(v1.value(), "A");
}

// Тест на создание вершины с одним параметром (значение по умолчанию)
TEST(VertexTest, ConstructorWithOneParameter) {
    Vertex v1(2);
    EXPECT_EQ(v1.number(), 2);
    EXPECT_EQ(v1.value(), "");
}

// Тест на создание вершины с пустым значением
TEST(VertexTest, ConstructorWithEmptyValue) {
    Vertex v1(3, "");
    EXPECT_EQ(v1.number(), 3);
    EXPECT_EQ(v1.value(), "");
}

// Тест на создание вершины с непустым значением
TEST(VertexTest, ConstructorWithNonEmptyValue) {
    Vertex v1(4, "TestValue");
    EXPECT_EQ(v1.number(), 4);
    EXPECT_EQ(v1.value(), "TestValue");
}

// Тест оператора равенства для одинаковых вершин
TEST(VertexTest, EqualityOperatorSameVertices) {
    Vertex v1(1, "A");
    Vertex v2(1, "A");
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
}

// Тест оператора равенства для вершин с разными номерами
TEST(VertexTest, EqualityOperatorDifferentNumbers) {
    Vertex v1(1, "A");
    Vertex v2(2, "A");
    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}

// Тест оператора равенства для вершин с разными значениями
TEST(VertexTest, EqualityOperatorDifferentValues) {
    Vertex v1(1, "A");
    Vertex v2(1, "B");
    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}

// Тест оператора равенства для вершин с разными номерами и значениями
TEST(VertexTest, EqualityOperatorDifferentNumbersAndValues) {
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}

// Тест оператора равенства для вершин с одинаковыми номерами, но одно значение пустое
TEST(VertexTest, EqualityOperatorSameNumbersOneEmptyValue) {
    Vertex v1(1, "");
    Vertex v2(1, "A");
    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}

// Тест оператора равенства при сравнении вершины с самой собой
TEST(VertexTest, EqualityOperatorSelfComparison) {
    Vertex v1(1, "A");
    EXPECT_TRUE(v1 == v1);
    EXPECT_FALSE(v1 != v1);
}

// Тест оператора неравенства для различных случаев
TEST(VertexTest, InequalityOperator) {
    Vertex v1(1, "A");
    Vertex v2(2, "A");
    Vertex v3(1, "B");
    Vertex v4(1, "A");
    
    EXPECT_TRUE(v1 != v2);
    EXPECT_TRUE(v1 != v3);
    EXPECT_FALSE(v1 != v4);
}

// Тест метода number()
TEST(VertexTest, NumberMethod) {
    Vertex v1(42, "Test");
    EXPECT_EQ(v1.number(), 42);
    
    Vertex v2(0, "Zero");
    EXPECT_EQ(v2.number(), 0);
    
    Vertex v3(1000, "Large");
    EXPECT_EQ(v3.number(), 1000);
}

// Тест метода value()
TEST(VertexTest, ValueMethod) {
    Vertex v1(1, "Simple");
    EXPECT_EQ(v1.value(), "Simple");
    
    Vertex v2(2, "");
    EXPECT_EQ(v2.value(), "");
    
    Vertex v3(3, "Long value with spaces");
    EXPECT_EQ(v3.value(), "Long value with spaces");
    
    Vertex v4(4, "Special@#$%");
    EXPECT_EQ(v4.value(), "Special@#$%");
}

// Тест на создание нескольких вершин
TEST(VertexTest, MultipleVertices) {
    Vertex v1(1, "First");
    Vertex v2(2, "Second");
    Vertex v3(3, "Third");
    
    EXPECT_EQ(v1.number(), 1);
    EXPECT_EQ(v1.value(), "First");
    
    EXPECT_EQ(v2.number(), 2);
    EXPECT_EQ(v2.value(), "Second");
    
    EXPECT_EQ(v3.number(), 3);
    EXPECT_EQ(v3.value(), "Third");
    
    EXPECT_TRUE(v1 != v2);
    EXPECT_TRUE(v2 != v3);
    EXPECT_TRUE(v1 != v3);
}

// Тест на граничные значения для number
TEST(VertexTest, BoundaryValuesForNumber) {
    Vertex v1(0, "Zero");
    Vertex v2(1, "One");
    Vertex v3(4294967295, "Max unsigned"); // максимальное значение unsigned int
    
    EXPECT_EQ(v1.number(), 0);
    EXPECT_EQ(v2.number(), 1);
    EXPECT_EQ(v3.number(), 4294967295u);
}
