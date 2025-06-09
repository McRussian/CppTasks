#include <gtest/gtest.h>
#include "../src/vector.hpp"

TEST(VectorTest, ConstructorOnlySize) {
    Vector<int> vec(5);
    EXPECT_EQ(vec.size(), 5);

    for(unsigned int i = 0; i < vec.size(); i++) {
        EXPECT_EQ(vec.at(i), 0);
        EXPECT_EQ(vec[i], 0);
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