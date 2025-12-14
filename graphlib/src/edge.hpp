#ifndef EDGE_HPP
#define EDGE_HPP

#include <utility>
#include <iostream>

#include "vertex.hpp"

class BaseEdge {
public:
    // Конструктор принимает две вершины
    BaseEdge(const Vertex& v1, const Vertex& v2) : vertex1_(v1), vertex2_(v2) {}
    
    // Деструктор
    ~BaseEdge() = default;
    
    // Получить первую вершину (константная версия)
    const Vertex& first() const {
        return vertex1_;
    }
    
    // Получить вторую вершину (константная версия)
    const Vertex& second() const {
        return vertex2_;
    }
    
    // Не константные версии (если нужно изменять вершины)
    Vertex& first() {
        return vertex1_;
    }
    
    Vertex& second() {
        return vertex2_;
    }
    
    // Получить обе вершины в виде пары
    std::pair<Vertex, Vertex> vertices() const {
        return std::make_pair(vertex1_, vertex2_);
    }
    
    // Проверить, является ли вершина частью ребра
    bool contains(const Vertex& vertex) const {
        return (vertex1_ == vertex) || (vertex2_ == vertex);
    }
    
    // Проверить, является ли вершина частью ребра по номеру
    bool contains(unsigned int vertexNumber) const {
        return (vertex1_.number() == vertexNumber) || (vertex2_.number() == vertexNumber);
    }
    
    // Проверить, являются ли две вершины концами ребра
    bool connects(const Vertex& v1, const Vertex& v2) const {
        return (vertex1_ == v1 && vertex2_ == v2) || 
               (vertex1_ == v2 && vertex2_ == v1);
    }
    
    // Проверить, являются ли две вершины концами ребра по номерам
    bool connects(unsigned int v1Number, unsigned int v2Number) const {
        return (vertex1_.number() == v1Number && vertex2_.number() == v2Number) || 
               (vertex1_.number() == v2Number && vertex2_.number() == v1Number);
    }
    
    // Оператор сравнения на равенство
    bool operator==(const BaseEdge& other) const {
        // Ребро (A, B) равно ребру (B, A) - порядок вершин не важен
        return (vertex1_ == other.vertex1_ && vertex2_ == other.vertex2_) ||
               (vertex1_ == other.vertex2_ && vertex2_ == other.vertex1_);
    }
    
    // Оператор сравнения на неравенство
    bool operator!=(const BaseEdge& other) const {
        return !(*this == other);
    }
    
    // Получить обратное ребро (поменять вершины местами)
    BaseEdge reversed() const {
        return BaseEdge(vertex2_, vertex1_);
    }
    
    // Проверить, является ли ребро петлей (вершины совпадают)
    bool isLoop() const {
        return vertex1_ == vertex2_;
    }

    // Для вывода в поток
    friend std::ostream& operator<<(std::ostream& os, const BaseEdge& edge) {
        os << "Edge(" << edge.vertex1_.number() << "[" << edge.vertex1_.value() 
           << "] - " << edge.vertex2_.number() << "[" << edge.vertex2_.value() << "])";
        return os;
    }

private:
    Vertex vertex1_;
    Vertex vertex2_;
};
#endif // BASEEDGE_HPP