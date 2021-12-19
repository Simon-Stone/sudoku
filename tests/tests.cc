#include <gtest/gtest.h>

#include "include/sudoku.h"

TEST(BackendTest, CheckSection)
{
  sudoku::Board board;
  EXPECT_TRUE(board.CheckSection({1, 2, 0, 3, 4, 5, 6, 7, 9}));
  EXPECT_FALSE(board.CheckSection({1, 2, 0, 3, 4, 5, 7, 7, 9}));
  EXPECT_DEATH(board.CheckSection({1, 2, 0, 30, 4, 5, 7, 7, 9}), "out of range");
}

TEST(BackendTest, GetBox)
{
  sudoku::Board board({{
    {{0, 1, 2, 3, 4, 5, 6, 7, 8}},
    {{1, 2, 3, 4, 5, 6, 7, 8, 0}},
    {{2, 3, 4, 5, 6, 7, 8, 0, 9}},
    {{3, 4, 5, 6, 7, 8, 0, 9, 2}},
    {{4, 5, 6, 7, 8, 0, 9, 3, 0}},
    {{5, 6, 7, 8, 0, 9, 1, 2, 3}},
    {{6, 7, 8, 0, 9, 1, 2, 3, 4}},
    {{7, 8, 0, 9, 1, 2, 3, 4, 5}},
    {{8, 0, 9, 1, 2, 3, 4, 5, 6}}
                      }});

  auto box = board.GetBox(1, 1);
  std::vector<unsigned> target = {0, 1, 2, 1, 2, 3, 2, 3, 4};
  ASSERT_EQ(box.size(), target.size()) << "Vectors are of unequal length";
  for (int i = 0; i < target.size(); ++i)
  {
    EXPECT_EQ(box[i], target[i]) << "Vectors differ at index " << i;
  }

  box = board.GetBox(0, 4);
  target = {3, 4, 5, 4, 5, 6, 5, 6, 7};
  ASSERT_EQ(box.size(), target.size()) << "Vectors are of unequal length";
  for (int i = 0; i < target.size(); ++i)
  {
    EXPECT_EQ(box[i], target[i]) << "Vectors differ at index " << i;
  }

  box = board.GetBox(8, 0);
  target = {6, 7, 8, 7, 8, 0, 8, 0, 9};
  ASSERT_EQ(box.size(), target.size()) << "Vectors are of unequal length";
  for (int i = 0; i < target.size(); ++i)
  {
    EXPECT_EQ(box[i], target[i]) << "Vectors differ at index " << i;
  }
}

TEST(BackendTests, CheckBoard)
{
  sudoku::Board board({{
                           {{0, 1, 2, 3, 4, 5, 6, 7, 8}},
                           {{1, 2, 3, 4, 5, 6, 7, 8, 0}},
                           {{2, 3, 4, 5, 6, 7, 8, 0, 9}},
                           {{3, 4, 5, 6, 7, 8, 0, 9, 2}},
                           {{4, 5, 6, 7, 8, 0, 9, 3, 0}},
                           {{5, 6, 7, 8, 0, 9, 1, 2, 3}},
                           {{6, 7, 8, 0, 9, 1, 2, 3, 4}},
                           {{7, 8, 0, 9, 1, 2, 3, 4, 5}},
                           {{8, 0, 9, 1, 2, 3, 4, 5, 6}}
                       }});

  EXPECT_FALSE(board.CheckBoard());

  board = sudoku::Board({{
             {{8, 2, 7, 1, 5, 4, 3, 9, 6}},
             {{9, 6, 5, 3, 2, 7, 1, 4, 8}},
             {{3, 4, 1, 6, 8, 9, 7, 5, 2}},
             {{5, 9, 3, 4, 6, 8, 2, 7, 1}},
             {{4, 7, 2, 5, 1, 3, 6, 8, 9}},
             {{6, 1, 8, 9, 7, 2, 4, 3, 5}},
             {{7, 8, 6, 2, 3, 5, 9, 1, 4}},
             {{1, 5, 4, 7, 9, 6, 8, 2, 3}},
             {{2, 3, 9, 8, 4, 1, 5, 6, 7}}
         }});
  EXPECT_TRUE(board.CheckBoard());
}
TEST(BackendTests, Clear)
{
  sudoku::Board board;
  board[0][3] = 4;
  EXPECT_FALSE(board == sudoku::Board());
  board.Clear();
  EXPECT_TRUE(board == sudoku::Board());
}

TEST(BackendTests, Solve)
{
  sudoku::Board goodBoard;

  goodBoard.SetCell(0, 1, 3);
  goodBoard.SetCell(7, 4, 8);
  std::cout << goodBoard;
  EXPECT_TRUE(goodBoard.Solve());
  std::cout << goodBoard;
  EXPECT_TRUE(goodBoard.CheckBoard());

  sudoku::Board badBoard;
  badBoard.SetCell(8, 7, 9);
  badBoard.SetCell(8, 8, 9);
  EXPECT_FALSE(badBoard.Solve());
  std::cout << badBoard;
  EXPECT_FALSE(badBoard.CheckBoard());
}

TEST(BackendTests, Initialize)
{
  sudoku::Board board;
  board.Initialize(81);
  std::cout << board;
  EXPECT_TRUE(board.CheckBoard());
  board.Initialize(17);
  std::cout << board;
  EXPECT_TRUE(board.CheckBoard());
}