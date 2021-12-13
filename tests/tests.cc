#include <gtest/gtest.h>

#include "sudoku.h"

TEST(BackendTest, CheckSection)
{
  EXPECT_TRUE(Sudoku::Check({1, 2, 0, 3, 4, 5, 6, 7, 9}));
  EXPECT_FALSE(Sudoku::Check({1, 2, 0, 3, 4, 5, 7, 7, 9}));
  EXPECT_DEATH(Sudoku::Check({1, 2, 0, 30, 4, 5, 7, 7, 9}), "out of range");
}

TEST(BackendTest, GetBox)
{
  std::vector<std::vector<unsigned>> board = {
      {0, 1, 2, 3, 4, 5, 6, 7, 8},
      {1, 2, 3, 4, 5, 6, 7, 8, 0},
      {2, 3, 4, 5, 6, 7, 8, 0, 9},
      {3, 4, 5, 6, 7, 8, 0, 9, 2},
      {4, 5, 6, 7, 8, 0, 9, 3, 0},
      {5, 6, 7, 8, 0, 9, 1, 2, 3},
      {6, 7, 8, 0, 9, 1, 2, 3, 4},
      {7, 8, 0, 9, 1, 2, 3, 4, 5},
      {8, 0, 9, 1, 2, 3, 4, 5, 6}
  };

  auto box = Sudoku::GetBox(board, 1, 1);
  std::vector<unsigned> target = {0, 1, 2, 1, 2, 3, 2, 3, 4};
  ASSERT_EQ(box.size(), target.size()) << "Vectors are of unequal length";
  for (int i = 0; i < target.size(); ++i)
  {
    EXPECT_EQ(box[i], target[i]) << "Vectors differ at index " << i;
  }

  box = Sudoku::GetBox(board, 0, 4);
  target = {3, 4, 5, 4, 5, 6, 5, 6, 7};
  ASSERT_EQ(box.size(), target.size()) << "Vectors are of unequal length";
  for (int i = 0; i < target.size(); ++i)
  {
    EXPECT_EQ(box[i], target[i]) << "Vectors differ at index " << i;
  }

  box = Sudoku::GetBox(board, 8, 0);
  target = {6, 7, 8, 7, 8, 0, 8, 0, 9};
  ASSERT_EQ(box.size(), target.size()) << "Vectors are of unequal length";
  for (int i = 0; i < target.size(); ++i)
  {
    EXPECT_EQ(box[i], target[i]) << "Vectors differ at index " << i;
  }
}

TEST(BackendTests, Check)
{

}
