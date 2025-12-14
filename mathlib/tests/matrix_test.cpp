#include <gtest/gtest.h>
#include "../src/matrix.hpp"
#include "../src/exception.hpp"

TEST(MatrixTest, ConstructorOnlySize) {
    Matrix<int> matr(3, 3);
    EXPECT_EQ(matr.get_rows(), 3);
    EXPECT_EQ(matr.get_cols(), 3);

    for(unsigned int i = 0; i < matr.get_rows(); i++) 
        for(unsigned int j = 0; j < matr.get_cols(); j++) {
            EXPECT_EQ(matr.at(i, j), 0);
            EXPECT_EQ(matr[i][j], 0);
        }
}

TEST(MatrixTest, ConstructorWithValue) {
    Matrix<int> matr(3, 3, 7);
    EXPECT_EQ(matr.get_rows(), 3);
    EXPECT_EQ(matr.get_cols(), 3);

    for(unsigned int i = 0; i < matr.get_rows(); i++) 
        for(unsigned int j = 0; j < matr.get_cols(); j++) {
            EXPECT_EQ(matr.at(i, j), 7);
            EXPECT_EQ(matr[i][j], 7);
        }
}

TEST(MatrixTest, BadConstructorListValues) {
    EXPECT_THROW(Matrix<int>({{0, 1, 2}, {0, 1, 2, 3}, {0, 1, 2}}), MatrixException);
}


TEST(MatrixTest, ConstructorListValues) {
    Matrix<int> matr({{0, 1, 2}, {0, 1, 2}, {0, 1, 2}});
    EXPECT_EQ(matr.get_rows(), 3);
    EXPECT_EQ(matr.get_cols(), 3);

    for(unsigned int i = 0; i < matr.get_rows(); i++) 
        for(unsigned int j = 0; j < matr.get_cols(); j++) {
            EXPECT_EQ(matr.at(i, j), j);
            EXPECT_EQ(matr[i][j], j);
        }
}
