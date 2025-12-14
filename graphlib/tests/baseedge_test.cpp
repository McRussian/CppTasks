#include <gtest/gtest.h>
#include "../src/vertex.hpp"
#include "../src/edge.hpp"

TEST(BaseEdgeTest, ConstructorAndBasicMethods) {
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    BaseEdge edge(v1, v2);
    
    EXPECT_EQ(edge.first().number(), 1);
    EXPECT_EQ(edge.first().value(), "A");
    EXPECT_EQ(edge.second().number(), 2);
    EXPECT_EQ(edge.second().value(), "B");
}

TEST(BaseEdgeTest, VerticesMethod) {
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    BaseEdge edge(v1, v2);
    
    auto vertices = edge.vertices();
    EXPECT_EQ(vertices.first.number(), 1);
    EXPECT_EQ(vertices.second.number(), 2);
}

TEST(BaseEdgeTest, ContainsVertex) {
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    Vertex v3(3, "C");
    BaseEdge edge(v1, v2);
    
    EXPECT_TRUE(edge.contains(v1));
    EXPECT_TRUE(edge.contains(v2));
    EXPECT_FALSE(edge.contains(v3));
}

TEST(BaseEdgeTest, ContainsVertexByNumber) {
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    BaseEdge edge(v1, v2);
    
    EXPECT_TRUE(edge.contains(1));
    EXPECT_TRUE(edge.contains(2));
    EXPECT_FALSE(edge.contains(3));
}

TEST(BaseEdgeTest, ConnectsVertices) {
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    Vertex v3(3, "C");
    BaseEdge edge(v1, v2);
    
    EXPECT_TRUE(edge.connects(v1, v2));
    EXPECT_TRUE(edge.connects(v2, v1)); // Порядок не важен
    EXPECT_FALSE(edge.connects(v1, v3));
    EXPECT_FALSE(edge.connects(v3, v2));
}

TEST(BaseEdgeTest, ConnectsVerticesByNumber) {
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    BaseEdge edge(v1, v2);
    
    EXPECT_TRUE(edge.connects(1, 2));
    EXPECT_TRUE(edge.connects(2, 1)); // Порядок не важен
    EXPECT_FALSE(edge.connects(1, 3));
    EXPECT_FALSE(edge.connects(3, 2));
}

TEST(BaseEdgeTest, EqualityOperatorOrderIndependent) {
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    Vertex v3(3, "C");
    
    BaseEdge edge1(v1, v2);
    BaseEdge edge2(v1, v2);
    BaseEdge edge3(v2, v1); // Обратный порядок
    BaseEdge edge4(v1, v3);
    
    EXPECT_TRUE(edge1 == edge2);
    EXPECT_TRUE(edge1 == edge3); // Порядок вершин не важен
    EXPECT_TRUE(edge3 == edge1); // Симметричность
    EXPECT_FALSE(edge1 == edge4);
}

TEST(BaseEdgeTest, InequalityOperator) {
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    Vertex v3(3, "C");
    
    BaseEdge edge1(v1, v2);
    BaseEdge edge2(v1, v3);
    
    EXPECT_TRUE(edge1 != edge2);
    EXPECT_FALSE(edge1 != edge1); // Рефлексивность
}

TEST(BaseEdgeTest, ReversedEdge) {
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    BaseEdge edge(v1, v2);
    BaseEdge reversed = edge.reversed();
    
    EXPECT_EQ(reversed.first().number(), 2);
    EXPECT_EQ(reversed.first().value(), "B");
    EXPECT_EQ(reversed.second().number(), 1);
    EXPECT_EQ(reversed.second().value(), "A");
    EXPECT_TRUE(edge == reversed); // Ребро равно своему обратному
}

TEST(BaseEdgeTest, IsLoop) {
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    
    BaseEdge loop(v1, v1);
    BaseEdge notLoop(v1, v2);
    
    EXPECT_TRUE(loop.isLoop());
    EXPECT_FALSE(notLoop.isLoop());
}

TEST(BaseEdgeTest, OutputOperator) {
    Vertex v1(1, "VertexA");
    Vertex v2(2, "VertexB");
    BaseEdge edge(v1, v2);
    
    std::stringstream ss;
    ss << edge;
    
    // Проверяем формат вывода
    std::string output = ss.str();
    EXPECT_TRUE(output.find("Edge(") != std::string::npos);
    EXPECT_TRUE(output.find("1[VertexA]") != std::string::npos);
    EXPECT_TRUE(output.find("2[VertexB]") != std::string::npos);
}

TEST(BaseEdgeTest, WithEmptyVertexValues) {
    Vertex v1(1);
    Vertex v2(2);
    BaseEdge edge(v1, v2);
    
    EXPECT_EQ(edge.first().value(), "");
    EXPECT_EQ(edge.second().value(), "");
    EXPECT_TRUE(edge.contains(v1));
    EXPECT_TRUE(edge.contains(v2));
}

TEST(BaseEdgeTest, SelfEdgeEquality) {
    Vertex v1(1, "A");
    BaseEdge edge(v1, v1);
    
    EXPECT_TRUE(edge == edge); // Рефлексивность
    EXPECT_FALSE(edge != edge);
    EXPECT_TRUE(edge.isLoop());
}
