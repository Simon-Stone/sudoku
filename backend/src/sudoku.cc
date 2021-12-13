//
// Created by Simon Stone on 12/10/2021.
//

#include "sudoku.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <stdexcept>

std::vector<std::vector<unsigned>> Sudoku::GetInitializedBoard(unsigned int num_clues)
{
  auto grid = std::vector<std::vector<unsigned>>(9, std::vector<unsigned>(9, 0));


  return grid;
}
std::vector<std::vector<unsigned>> Sudoku::Solve(std::vector<std::vector<unsigned int>> board)
{
  auto currentRow = 0;
  auto currentCol = 0;

  for (unsigned num = 1; num <= 9; ++num)
  {
    board[currentRow][currentCol] = num;
    Check(board, currentRow, currentCol);
  }

  return {};
}
bool Sudoku::Check(const std::vector<std::vector<unsigned int>> &board, unsigned row, unsigned col)
{
  return Check(GetCol(board, col)) && Check(GetRow(board, row)) && Check(GetBox(board, row, col));
}
bool Sudoku::Check(const std::vector<std::vector<unsigned int>> &board)
{
  // Check all rows
  for(unsigned i = 0; i < board.size(); ++i)
  {
    if (!Check(GetRow(board, i)))
      return false;
  }

  // Check all columns
  for(unsigned i = 0; i < board[0].size(); ++i)
  {
    if (!Check(GetCol(board, i)))
      return false;
  }

  // Check all boxes
  for(unsigned i = 1; i < board.size(); i += 3)
  {
    for(unsigned j = 1; j < board[0].size(); j += 3)
    {
      if (!Check(board, i, j))
        return false;
    }
  }
  // All checks passed
  return true;
}
bool Sudoku::Check(const std::vector<unsigned int> &section)
{
   // Check the section
   assert(section.size() == 9);

   std::vector<unsigned> seenNumbers;

   for(const auto& x : section)
   {
     // Zeros are always okay as fillers
     if (x == 0) { continue; }

     // Some range checks
     assert(x > 0 && x < 10 && "One of the numbers in the section is out of range!");

     // If the current number has been seen before, the section is invalid!
     if (std::find(begin(seenNumbers), end(seenNumbers), x) != std::end(seenNumbers))
       return false;

     seenNumbers.push_back(x);
   }
   return true;
}
std::vector<unsigned> Sudoku::GetCol(const std::vector<std::vector<unsigned int>> &board, unsigned colIdx)
{
  std::vector<unsigned> col;
  for (const auto& row : board)
  {
    col.push_back(row[colIdx]);
  }
  return col;
}
std::vector<unsigned> Sudoku::GetRow(const std::vector<std::vector<unsigned int>> &board, unsigned rowIdx)
{
  return board[rowIdx];
}
std::vector<unsigned> Sudoku::GetBox(const std::vector<std::vector<unsigned int>> &board, unsigned rowIdx, unsigned colIdx)
{
  std::array<unsigned, 3> rowRange = {0, 0, 0};
  std::array<unsigned, 3> colRange = {0, 0, 0};

  auto relColIdx = colIdx % 3;
  auto relRowIdx = rowIdx % 3;

  switch (relColIdx)
  {
    case 0:
      colRange = {colIdx, colIdx + 1, colIdx + 2};
      break;
    case 1:
      colRange = {colIdx - 1, colIdx, colIdx + 1};
      break;
    case 2:
      colRange = {colIdx - 2, colIdx - 1, colIdx};
      break;
    default:
      throw std::runtime_error("Out of range!");
  }

  switch (relRowIdx)
  {
    case 0:
      rowRange = {rowIdx, rowIdx + 1, rowIdx + 2};
      break;
    case 1:
      rowRange = {rowIdx - 1, rowIdx, rowIdx + 1};
      break;
    case 2:
      rowRange = {rowIdx - 2, rowIdx - 1, rowIdx};
      break;
    default:
      throw std::runtime_error("Out of range!");
  }

  std::vector<unsigned> box;
  for(const auto& r : rowRange)
  {
    for(const auto& c : colRange)
    {
      box.push_back(board[r][c]);
    }
  }
  return box;
}
