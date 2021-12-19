//
// Created by Simon Stone on 12/10/2021.
//

#include "sudoku.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <iterator>
#include <random>
#include <string>
#include <stdexcept>



using namespace sudoku;

Board::Board(const std::array<std::array<unsigned, 9>, 9> &board) : board_(board)
{
}

bool Board::Initialize(unsigned int num_clues)
{
  board_ = std::array<std::array<unsigned, 9>, 9>();
  return true;
}
bool Board::Solve()
{
  std::pair<unsigned, unsigned> pos;  // row index, column index
  try
  {
    pos = FindEmptyCell();
  }
  catch(std::out_of_range&)
  {
    // No empty cell left in board!
    return true;
  }
  for (unsigned value = 1; value <= 9; ++value)
  {
    if (IsValid(pos.first, pos.second, value))
    {
      board_[pos.first][pos.second] = value;
      if (Solve())
      {
        return true;
      }
      // If this does not help solve the sudoku, unassign cell and move on
      board_[pos.first][pos.second] = 0;
    }
  }

  return false;
}
bool Board::CheckBox(unsigned row, unsigned col)
{
  return CheckSection(GetCol(col)) && CheckSection(GetRow(row)) && CheckSection(GetBox(row, col));
}
bool Board::CheckBoard()
{
  // Check all rows
  for(unsigned i = 0; i < board_.size(); ++i)
  {
    if (!CheckSection(GetRow(i)))
      return false;
  }

  // Check all columns
  for(unsigned i = 0; i < board_[0].size(); ++i)
  {
    if (!CheckSection(GetCol(i)))
      return false;
  }

  // Check all boxes
  for(unsigned i = 1; i < board_.size(); i += 3)
  {
    for(unsigned j = 1; j < board_[0].size(); j += 3)
    {
      if (!CheckBox(i, j))
        return false;
    }
  }
  // All checks passed
  return true;
}
bool Board::CheckSection(const std::array<unsigned, 9> &section)
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
std::array<unsigned, 9> Board::GetCol(unsigned colIdx) const
{
  std::array<unsigned, 9> col{0};
  int pos = 0;
  for (const auto& row : board_)
  {
    col[pos++] = row[colIdx];
  }
  return col;
}
std::array<unsigned, 9> Board::GetRow(unsigned rowIdx) const
{
  return board_[rowIdx];
}
std::array<unsigned, 9> Board::GetBox(unsigned rowIdx, unsigned colIdx) const
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

  std::array<unsigned, 9> box{};
  int pos = 0;
  for(const auto& r : rowRange)
  {
    for(const auto& c : colRange)
    {
      box[pos++] = board_[r][c];
    }
  }
  return box;
}
std::array<std::array<unsigned, 9>, 9> Board::GetBoard() const
{
  return board_;
}

void Board::SetBoard(const std::array<std::array<unsigned, 9>, 9>& board)
{
  board_ = board;
}
unsigned Board::GetNumberCols() const
{
  return board_[0].size();
}
unsigned Board::GetNumberRows() const
{
  return board_.size();
}
void Board::SetCell(unsigned int rowIdx, unsigned int colIdx, unsigned int value)
{
  if (value < 1 || value > 9)
  {
    throw std::out_of_range("[Board::SetCell()] Passe value was out of range:" + std::to_string(value));
  }
  board_[rowIdx][colIdx] = value;
}
bool Board::IsInBox(unsigned int value, unsigned int rowIdx, unsigned int colIdx)
{
  return IsInSection(value, GetBox(rowIdx, colIdx));
}
bool Board::IsInCol(unsigned int value, unsigned int colIdx)
{
  return IsInSection(value, GetCol(colIdx));
}
bool Board::IsInRow(unsigned int value, unsigned int rowIdx)
{
  return IsInSection(value, GetRow(rowIdx));
}
bool Board::IsInSection(unsigned int value, const std::array<unsigned int, 9> &section)
{
  return std::find(section.begin(), section.end(), value) != section.end();
}
std::pair<unsigned, unsigned> Board::FindEmptyCell()
{
  unsigned rowIdx, colIdx;
  for (rowIdx = 0; rowIdx < GetNumberRows(); ++rowIdx)
  {
    for(colIdx = 0; colIdx < GetNumberCols(); ++colIdx)
    {
      if (board_[rowIdx][colIdx] == 0)
      {
        return {rowIdx, colIdx};
      }
    }
  }
  throw std::out_of_range("Could not find empty cell!");
}
bool Board::IsValid(unsigned int rowIdx, unsigned int colIdx, unsigned int value)
{
  return !IsInRow(value, rowIdx) && !IsInCol(value, colIdx) && !IsInBox(value, rowIdx, colIdx);
}
std::ostream &sudoku::operator<<(std::ostream &os, const Board &board)
{
  os << "-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t-\n";
  for(const auto& row : board.board_)
  {
    os << "|\t";
    for(const auto& number : row)
    {
      os << number << "\t";
    }
    os << "|\n";
  }
  os << "-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t-\n";
  return os;
}

