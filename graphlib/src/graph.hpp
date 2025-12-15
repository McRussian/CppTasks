#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>
#include <memory>

#include "exception.hpp"
#include "edge.hpp"

class Graph {
public:
    // Конструкторы
    Graph() = default;
    
    Graph(const std::vector<Vertex>& vertices, const std::vector<BaseEdge>& edges) {
        // Добавляем вершины
        for (const auto& vertex : vertices) {
            addVertex(vertex);
        }
        
        // Добавляем ребра
        for (const auto& edge : edges) {
            addEdge(edge);
        }
    }
    
    // Добавление вершины
    void addVertex(const Vertex& vertex) {
        // Проверяем, нет ли уже такой вершины
        if (containsVertex(vertex.number())) {
            throw GraphException(50, "Vertex already exists in graph");
        }
        vertices_.push_back(vertex);
    }
    
    // Добавление вершины по номеру и значению
    Vertex& addVertex(unsigned int number, const std::string& value = "") {
        Vertex vertex(number, value);
        addVertex(vertex);
        return vertices_.back(); // Возвращаем ссылку на добавленную вершину
    }
    
    // Добавление ребра
    void addEdge(const BaseEdge& edge) {
        // Проверяем, что обе вершины существуют в графе
        if (!containsVertex(edge.first()) || !containsVertex(edge.second())) {
            throw GraphException(50, "Cannot add edge: one or both vertices not in graph");
        }
        
        // Проверяем, нет ли уже такого ребра
        if (containsEdge(edge)) {
            throw GraphException(50, "Edge already exists in graph");
        }
        
        edges_.push_back(edge);
    }
    
    // Добавление ребра по двум вершинам
    void addEdge(const Vertex& v1, const Vertex& v2) {
        BaseEdge edge(v1, v2);
        addEdge(edge);
    }
    
    // Добавление ребра по номерам вершин
    void addEdge(unsigned int v1Number, unsigned int v2Number) {
        Vertex* v1 = findVertexByNumber(v1Number);
        Vertex* v2 = findVertexByNumber(v2Number);
        
        if (!v1 || !v2) {
            throw GraphException(50, "Cannot add edge: one or both vertices not in graph");
        }
        
        BaseEdge edge(*v1, *v2);
        addEdge(edge);
    }
    
    // Удаление вершины
    void removeVertex(const Vertex& vertex) {
        // Проверяем, существует ли вершина
        if (!containsVertex(vertex)) {
            throw GraphException(50, "Vertex not found in graph");
        }
        
        // Удаляем все ребра, инцидентные этой вершине
        removeEdgesIncidentToVertex(vertex);
        
        // Удаляем вершину из списка
        auto it = std::find(vertices_.begin(), vertices_.end(), vertex);
        if (it != vertices_.end()) {
            vertices_.erase(it);
        }
    }
    
    // Удаление вершины по номеру
    void removeVertex(unsigned int vertexNumber) {
        Vertex* vertex = findVertexByNumber(vertexNumber);
        if (!vertex) {
            throw GraphException(50, "Vertex not found in graph");
        }
        removeVertex(*vertex);
    }
    
    // Удаление ребра
    void removeEdge(const BaseEdge& edge) {
        // Проверяем, существует ли ребро
        if (!containsEdge(edge)) {
            throw GraphException(50, "Edge not found in graph");
        }
        
        // Ищем и удаляем ребро
        auto it = std::find(edges_.begin(), edges_.end(), edge);
        if (it != edges_.end()) {
            edges_.erase(it);
        }
    }
    
    // Удаление ребра по двум вершинам
    void removeEdge(const Vertex& v1, const Vertex& v2) {
        BaseEdge edge(v1, v2);
        removeEdge(edge);
    }
    
    // Удаление ребра по номерам вершин
    void removeEdge(unsigned int v1Number, unsigned int v2Number) {
        Vertex* v1 = findVertexByNumber(v1Number);
        Vertex* v2 = findVertexByNumber(v2Number);
        
        if (!v1 || !v2) {
            throw GraphException(50, "Cannot remove edge: one or both vertices not in graph");
        }
        
        BaseEdge edge(*v1, *v2);
        removeEdge(edge);
    }
    
    // Проверка наличия вершины
    bool containsVertex(const Vertex& vertex) const {
        return std::find(vertices_.begin(), vertices_.end(), vertex) != vertices_.end();
    }
    
    // Проверка наличия вершины по номеру
    bool containsVertex(unsigned int vertexNumber) const {
        return findVertexByNumber(vertexNumber) != nullptr;
    }
    
    // Проверка наличия ребра
    bool containsEdge(const BaseEdge& edge) const {
        return std::find(edges_.begin(), edges_.end(), edge) != edges_.end();
    }
    
    // Проверка наличия ребра по вершинам
    bool containsEdge(const Vertex& v1, const Vertex& v2) const {
        BaseEdge edge(v1, v2);
        return containsEdge(edge);
    }
    
    // Проверка наличия ребра по номерам вершин
    bool containsEdge(unsigned int v1Number, unsigned int v2Number) const {
        const Vertex* v1 = findVertexByNumber(v1Number);
        const Vertex* v2 = findVertexByNumber(v2Number);
        
        if (!v1 || !v2) {
            return false;
        }
        
        BaseEdge edge(*v1, *v2);
        return containsEdge(edge);
    }
    
    // Получение всех вершин
    const std::vector<Vertex>& getVertices() const {
        return vertices_;
    }
    
    // Получение всех ребер
    const std::vector<BaseEdge>& getEdges() const {
        return edges_;
    }
    
    // Поиск всех смежных вершин для данной вершины
    std::vector<Vertex> getAdjacentVertices(const Vertex& vertex) const {
        if (!containsVertex(vertex)) {
            throw GraphException(50, "Vertex not found in graph");
        }
        
        std::unordered_set<Vertex> uniqueVertices;
        
        for (const auto& edge : edges_) {
            if (edge.contains(vertex)) {
                // Добавляем другую вершину ребра
                uniqueVertices.insert(edge.otherVertex(vertex));
            }
        }
        
        // Конвертируем unordered_set в vector
        return std::vector<Vertex>(uniqueVertices.begin(), uniqueVertices.end());
    }
    
    // Поиск всех смежных вершин по номеру вершины
    std::vector<Vertex> getAdjacentVertices(unsigned int vertexNumber) const {
        const Vertex* vertex = findVertexByNumber(vertexNumber);
        if (!vertex) {
            throw GraphException(50, "Vertex not found in graph");
        }
        return getAdjacentVertices(*vertex);
    }
    
    // Поиск всех ребер, инцидентных данной вершине
    std::vector<BaseEdge> getIncidentEdges(const Vertex& vertex) const {
        if (!containsVertex(vertex)) {
            throw GraphException(50, "Vertex not found in graph");
        }
        
        std::vector<BaseEdge> incidentEdges;
        
        for (const auto& edge : edges_) {
            if (edge.contains(vertex)) {
                incidentEdges.push_back(edge);
            }
        }
        
        return incidentEdges;
    }
    
    // Поиск всех ребер, инцидентных вершине по номеру
    std::vector<BaseEdge> getIncidentEdges(unsigned int vertexNumber) const {
        const Vertex* vertex = findVertexByNumber(vertexNumber);
        if (!vertex) {
            throw GraphException(50, "Vertex not found in graph");
        }
        return getIncidentEdges(*vertex);
    }
    
    // Проверка смежности двух вершин
    bool areAdjacent(const Vertex& v1, const Vertex& v2) const {
        if (!containsVertex(v1) || !containsVertex(v2)) {
            return false;
        }
        
        return containsEdge(v1, v2);
    }
    
    // Проверка смежности двух вершин по номерам
    bool areAdjacent(unsigned int v1Number, unsigned int v2Number) const {
        return containsEdge(v1Number, v2Number);
    }
    
    // Получение степени вершины (количество инцидентных ребер)
    unsigned int getVertexDegree(const Vertex& vertex) const {
        if (!containsVertex(vertex)) {
            throw GraphException(50, "Vertex not found in graph");
        }
        
        unsigned int degree = 0;
        for (const auto& edge : edges_) {
            if (edge.contains(vertex)) {
                degree++;
            }
        }
        return degree;
    }
    
    // Получение степени вершины по номеру
    unsigned int getVertexDegree(unsigned int vertexNumber) const {
        const Vertex* vertex = findVertexByNumber(vertexNumber);
        if (!vertex) {
            throw GraphException(50, "Vertex not found in graph");
        }
        return getVertexDegree(*vertex);
    }
    
    // Получение количества вершин
    size_t vertexCount() const {
        return vertices_.size();
    }
    
    // Получение количества ребер
    size_t edgeCount() const {
        return edges_.size();
    }
    
    // Очистка графа
    void clear() {
        vertices_.clear();
        edges_.clear();
    }
    
    // Проверка на пустоту
    bool isEmpty() const {
        return vertices_.empty();
    }
    
    // Поиск вершины по номеру
    Vertex* findVertexByNumber(unsigned int number) {
        for (auto& vertex : vertices_) {
            if (vertex.number() == number) {
                return &vertex;
            }
        }
        return nullptr;
    }
    
    // Константная версия поиска вершины по номеру
    const Vertex* findVertexByNumber(unsigned int number) const {
        for (const auto& vertex : vertices_) {
            if (vertex.number() == number) {
                return &vertex;
            }
        }
        return nullptr;
    }

private:
    std::vector<Vertex> vertices_;
    std::vector<BaseEdge> edges_;
    
    // Вспомогательный метод для удаления всех ребер, инцидентных вершине
    void removeEdgesIncidentToVertex(const Vertex& vertex) {
        // Используем идиому remove-erase
        edges_.erase(
            std::remove_if(edges_.begin(), edges_.end(),
                [&vertex](const BaseEdge& edge) {
                    return edge.contains(vertex);
                }),
            edges_.end()
        );
    }
};

#endif // GRAPH_HPP