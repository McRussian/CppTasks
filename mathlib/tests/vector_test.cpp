#include <gtest/gtest.h>
#include "../src/vector.hpp"
#include "../src/exception.hpp"

TEST(VectorTest, ConstructorOnlySize) {
    Vector<int> vec(5);
    EXPECT_EQ(vec.size(), 5);

    for(unsigned int i = 0; i < vec.size(); i++) {
        EXPECT_EQ(vec.at(i), 0);
        EXPECT_EQ(vec[i], 0);
    }
}

TEST(VectorTest, ConstructorWithValue) {
    Vector<int> vec(5, 10);
    EXPECT_EQ(vec.size(), 5);

    for(unsigned int i = 0; i < vec.size(); i++) {
        EXPECT_EQ(vec.at(i), 10);
        EXPECT_EQ(vec[i], 10);
    }
}


TEST(VectorTest, ConstructorListValues) {
    Vector<int> vec = {0, 1, 2, 3, 4};
    EXPECT_EQ(vec.size(), 5);

    for(unsigned int i = 0; i < vec.size(); i++) {
        EXPECT_EQ(vec.at(i), i);
        EXPECT_EQ(vec[i], i);
    }
}

TEST(VectorTest, ConstructorFromVector) {
    std::vector<int> vect = {1, 2, 3, 4, 5};

    Vector<int> v(vect);
    EXPECT_EQ(v.size(), vect.size());

    for(unsigned int i = 0; i < v.size(); i++) {
        EXPECT_EQ(v.at(i), vect.at(i));
        EXPECT_EQ(v[i], vect[i]);
    }
}

TEST(VectorTest, ExceptionIndexRange) {
    Vector<int> vec(5);
    EXPECT_EQ(vec.size(), 5);

    EXPECT_THROW(vec[5], VectorException);
    EXPECT_THROW(vec.at(5), VectorException);
}

TEST(VectorTest, EqualTwoVector) {
    Vector<int> vec1(5);
    Vector<int> vec2 =  {0, 0, 0, 0, 0};

    EXPECT_TRUE(vec1 == vec2);

    Vector<int> vec3(6);

    EXPECT_THROW(vec1 == vec3, VectorException);
}

TEST(VectorTest, NotEqualTwoVector) {
    Vector<int> vec1(5);
    
    Vector<int> vec2(5, 1);
    EXPECT_FALSE(vec1 == vec2);

    Vector<int> vec3(6);
    EXPECT_THROW(vec1 == vec3, VectorException);
}

TEST(VectorTest, SummaVectors) {
    Vector<int> vec1(5, 1);
    Vector<int> vec2 = {1, 2, 3, 4, 5};

    Vector<int> rez = {2, 3, 4, 5, 6};
    EXPECT_EQ(vec1 + vec2, rez);

    Vector<int> bad(6);
    EXPECT_THROW(vec1 + bad, VectorException);
}

TEST(VectorTest, DiffVectors) {
    Vector<int> vec2(5, 1);
    Vector<int> vec1 = {1, 2, 3, 4, 5};

    Vector<int> rez = {0, 1, 2, 3, 4};
    EXPECT_EQ(vec1 - vec2, rez);

    Vector<int> bad(6);
    EXPECT_THROW(vec1 - bad, VectorException);
}

TEST(VectorTest, MultiplyToScalar) {
    Vector<int> vec = {1, 2, 3, 4, 5};
    int scalar = 2;
    Vector<int> rez = {2, 4, 6, 8, 10};
    EXPECT_EQ(vec * scalar, rez);
    EXPECT_EQ(scalar * vec, rez);
}

TEST(VectorTest, MultiplyVectors) {
    Vector<int> vec1(5, 2);
    Vector<int> vec2 = {1, 2, 3, 4, 5};

    int rez = 30;
    EXPECT_EQ(vec1 * vec2, rez);

    Vector<int> bad(6);
    EXPECT_THROW(vec1 * bad, VectorException);
}

TEST(VectorTest, GetSlice) {
    Vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    EXPECT_EQ(vec.slice(0, 9, 2), Vector<int>({1, 3, 5, 7, 9}));

    EXPECT_EQ(vec.slice(3, 5), Vector<int>({4, 5, 6}));

    EXPECT_EQ(vec.slice(9, 0, -3), Vector<int>({10, 7, 4, 1}));

    EXPECT_EQ(vec.slice(-1, -10, -3), Vector<int>({10, 7, 4, 1}));
}

TEST(VectorTest, GetBadSlice) {
    Vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    EXPECT_THROW(vec.slice(1, 9, 0), VectorException);

}
