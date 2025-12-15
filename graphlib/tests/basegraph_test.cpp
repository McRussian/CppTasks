#include <gtest/gtest.h>
#include <vector>

#include "graph.hpp"
#include "exception.hpp"

TEST(GraphTest, DefaultConstructor) {
    Graph graph;
    
    EXPECT_EQ(graph.vertexCount(), 0);
    EXPECT_EQ(graph.edgeCount(), 0);
    EXPECT_TRUE(graph.isEmpty());
}

TEST(GraphTest, ConstructorWithVerticesAndEdges) {
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    Vertex v3(3, "C");
    
    std::vector<Vertex> vertices = {v1, v2, v3};
    std::vector<BaseEdge> edges = {BaseEdge(v1, v2), BaseEdge(v2, v3)};
    
    Graph graph(vertices, edges);
    
    EXPECT_EQ(graph.vertexCount(), 3);
    EXPECT_EQ(graph.edgeCount(), 2);
    EXPECT_TRUE(graph.containsVertex(v1));
    EXPECT_TRUE(graph.containsEdge(v1, v2));
}

TEST(GraphTest, AddVertex) {
    Graph graph;
    Vertex v1(1, "A");
    
    graph.addVertex(v1);
    
    EXPECT_EQ(graph.vertexCount(), 1);
    EXPECT_TRUE(graph.containsVertex(v1));
    EXPECT_TRUE(graph.containsVertex(1));
}

TEST(GraphTest, AddVertexDuplicateThrows) {
    Graph graph;
    Vertex v1(1, "A");
    
    graph.addVertex(v1);
    
    EXPECT_THROW(graph.addVertex(v1), GraphException);
    EXPECT_THROW(graph.addVertex(1, "B"), GraphException);
}

TEST(GraphTest, AddVertexByNumberAndValue) {
    Graph graph;
    
    Vertex& addedVertex = graph.addVertex(1, "VertexA");
    
    EXPECT_EQ(graph.vertexCount(), 1);
    EXPECT_EQ(addedVertex.number(), 1);
    EXPECT_EQ(addedVertex.value(), "VertexA");
    EXPECT_TRUE(graph.containsVertex(1));
}

TEST(GraphTest, AddEdge) {
    Graph graph;
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    
    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addEdge(v1, v2);
    
    EXPECT_EQ(graph.edgeCount(), 1);
    EXPECT_TRUE(graph.containsEdge(v1, v2));
    EXPECT_TRUE(graph.areAdjacent(v1, v2));
}

TEST(GraphTest, AddEdgeMissingVertexThrows) {
    Graph graph;
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    
    graph.addVertex(v1);
    // v2 не добавлена
    
    EXPECT_THROW(graph.addEdge(v1, v2), GraphException);
}

TEST(GraphTest, AddEdgeDuplicateThrows) {
    Graph graph;
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    
    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addEdge(v1, v2);
    
    EXPECT_THROW(graph.addEdge(v1, v2), GraphException);
    EXPECT_THROW(graph.addEdge(v2, v1), GraphException); // Обратное ребро тоже дубликат
}

TEST(GraphTest, AddEdgeByNumbers) {
    Graph graph;
    
    graph.addVertex(1, "A");
    graph.addVertex(2, "B");
    graph.addEdge(1, 2);
    
    EXPECT_EQ(graph.edgeCount(), 1);
    EXPECT_TRUE(graph.containsEdge(1, 2));
}

TEST(GraphTest, RemoveVertex) {
    Graph graph;
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    Vertex v3(3, "C");
    
    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addVertex(v3);
    graph.addEdge(v1, v2);
    graph.addEdge(v2, v3);
    
    EXPECT_EQ(graph.vertexCount(), 3);
    EXPECT_EQ(graph.edgeCount(), 2);
    
    graph.removeVertex(v2);
    
    EXPECT_EQ(graph.vertexCount(), 2);
    EXPECT_EQ(graph.edgeCount(), 0); // Оба ребра удалены, так как они были инцидентны v2
    EXPECT_FALSE(graph.containsVertex(v2));
    EXPECT_TRUE(graph.containsVertex(v1));
    EXPECT_TRUE(graph.containsVertex(v3));
}

TEST(GraphTest, RemoveVertexByNumber) {
    Graph graph;
    
    graph.addVertex(1, "A");
    graph.addVertex(2, "B");
    graph.addEdge(1, 2);
    
    graph.removeVertex(2);
    
    EXPECT_EQ(graph.vertexCount(), 1);
    EXPECT_EQ(graph.edgeCount(), 0);
    EXPECT_FALSE(graph.containsVertex(2));
}

TEST(GraphTest, RemoveNonExistentVertexThrows) {
    Graph graph;
    Vertex v1(1, "A");
    
    graph.addVertex(v1);
    
    Vertex v2(2, "B");
    EXPECT_THROW(graph.removeVertex(v2), GraphException);
    EXPECT_THROW(graph.removeVertex(2), GraphException);
}

TEST(GraphTest, RemoveEdge) {
    Graph graph;
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    Vertex v3(3, "C");
    
    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addVertex(v3);
    graph.addEdge(v1, v2);
    graph.addEdge(v2, v3);
    
    EXPECT_EQ(graph.edgeCount(), 2);
    
    graph.removeEdge(v1, v2);
    
    EXPECT_EQ(graph.edgeCount(), 1);
    EXPECT_FALSE(graph.containsEdge(v1, v2));
    EXPECT_TRUE(graph.containsEdge(v2, v3));
    EXPECT_EQ(graph.vertexCount(), 3); // Вершины остаются
}

TEST(GraphTest, RemoveEdgeByNumbers) {
    Graph graph;
    
    graph.addVertex(1, "A");
    graph.addVertex(2, "B");
    graph.addVertex(3, "C");
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    
    graph.removeEdge(1, 2);
    
    EXPECT_EQ(graph.edgeCount(), 1);
    EXPECT_FALSE(graph.containsEdge(1, 2));
}

TEST(GraphTest, RemoveNonExistentEdgeThrows) {
    Graph graph;
    Vertex v1(1, "A");
    Vertex v2(2, "B");
    
    graph.addVertex(v1);
    graph.addVertex(v2);
    
    EXPECT_THROW(graph.removeEdge(v1, v2), GraphException);
    EXPECT_THROW(graph.removeEdge(1, 2), GraphException);
}

TEST(GraphTest, GetAdjacentVertices) {
    Graph graph;
    
    graph.addVertex(1, "A");
    graph.addVertex(2, "B");
    graph.addVertex(3, "C");
    graph.addVertex(4, "D");
    
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    
    auto adjacentTo1 = graph.getAdjacentVertices(1);
    auto adjacentTo3 = graph.getAdjacentVertices(3);
    
    EXPECT_EQ(adjacentTo1.size(), 2);
    EXPECT_EQ(adjacentTo3.size(), 3);
    
    // Проверяем, что вершины 2 и 3 смежны с вершиной 1
    bool has2 = false, has3 = false;
    for (const auto& v : adjacentTo1) {
        if (v.number() == 2) has2 = true;
        if (v.number() == 3) has3 = true;
    }
    EXPECT_TRUE(has2);
    EXPECT_TRUE(has3);
}

TEST(GraphTest, GetIncidentEdges) {
    Graph graph;
    
    graph.addVertex(1, "A");
    graph.addVertex(2, "B");
    graph.addVertex(3, "C");
    
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);
    
    auto incidentTo1 = graph.getIncidentEdges(1);
    auto incidentTo3 = graph.getIncidentEdges(3);
    
    EXPECT_EQ(incidentTo1.size(), 2);
    EXPECT_EQ(incidentTo3.size(), 2);
    
    // Проверяем ребра
    bool hasEdge1_2 = false, hasEdge1_3 = false;
    for (const auto& edge : incidentTo1) {
        if (edge.connects(1, 2)) hasEdge1_2 = true;
        if (edge.connects(1, 3)) hasEdge1_3 = true;
    }
    EXPECT_TRUE(hasEdge1_2);
    EXPECT_TRUE(hasEdge1_3);
}

TEST(GraphTest, AreAdjacent) {
    Graph graph;
    
    graph.addVertex(1, "A");
    graph.addVertex(2, "B");
    graph.addVertex(3, "C");
    
    graph.addEdge(1, 2);
    
    EXPECT_TRUE(graph.areAdjacent(1, 2));
    EXPECT_TRUE(graph.areAdjacent(2, 1)); // Симметричность
    EXPECT_FALSE(graph.areAdjacent(1, 3));
    EXPECT_FALSE(graph.areAdjacent(2, 3));
}

TEST(GraphTest, GetVertexDegree) {
    Graph graph;
    
    graph.addVertex(1, "A");
    graph.addVertex(2, "B");
    graph.addVertex(3, "C");
    graph.addVertex(4, "D");
    
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    
    EXPECT_EQ(graph.getVertexDegree(1), 2); // Ребра: (1,2), (1,3)
    EXPECT_EQ(graph.getVertexDegree(2), 2); // Ребра: (1,2), (2,3)
    EXPECT_EQ(graph.getVertexDegree(3), 3); // Ребра: (1,3), (2,3), (3,4)
    EXPECT_EQ(graph.getVertexDegree(4), 1); // Ребро: (3,4)
}

TEST(GraphTest, ClearGraph) {
    Graph graph;
    
    graph.addVertex(1, "A");
    graph.addVertex(2, "B");
    graph.addEdge(1, 2);
    
    EXPECT_EQ(graph.vertexCount(), 2);
    EXPECT_EQ(graph.edgeCount(), 1);
    
    graph.clear();
    
    EXPECT_EQ(graph.vertexCount(), 0);
    EXPECT_EQ(graph.edgeCount(), 0);
    EXPECT_TRUE(graph.isEmpty());
}

TEST(GraphTest, GraphWithLoop) {
    Graph graph;
    
    graph.addVertex(1, "A");
    graph.addEdge(1, 1); // Петля
    
    EXPECT_EQ(graph.vertexCount(), 1);
    EXPECT_EQ(graph.edgeCount(), 1);
    EXPECT_TRUE(graph.containsEdge(1, 1));
    
    auto incident = graph.getIncidentEdges(1);
    EXPECT_EQ(incident.size(), 1);
    EXPECT_TRUE(incident[0].isLoop());
    
    EXPECT_EQ(graph.getVertexDegree(1), 1);
}

TEST(GraphTest, MultipleEdgesBetweenSameVerticesNotAllowed) {
    Graph graph;
    
    graph.addVertex(1, "A");
    graph.addVertex(2, "B");
    graph.addEdge(1, 2);
    
    // Нельзя добавить еще одно ребро между теми же вершинами
    EXPECT_THROW(graph.addEdge(2, 1), GraphException);
    
    // Но можно создать новый граф с теми же вершинами
    Graph graph2;
    graph2.addVertex(1, "A");
    graph2.addVertex(2, "B");
    graph2.addEdge(2, 1); // Это нормально в новом графе
    
    EXPECT_EQ(graph2.edgeCount(), 1);
}
