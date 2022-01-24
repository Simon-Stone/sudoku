//
// Created by Simon Stone on 12/10/2021.
//

#include "include/sudoku.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <iterator>
#include <tuple>
#include <random>
#include <set>
#include <string>
#include <stdexcept>



using namespace sudoku;

Board::Board(const std::array<std::array<unsigned, 9>, 9> &board) : board_(board)
{
}

bool Board::Initialize(unsigned int num_clues)
{
  if (num_clues > 81)
  {
    return false;
  }
  // Fill board with zeros
  Clear();

  // Create a random valid board
  auto solvedBoard = Board();
  solvedBoard.Solve();

  // Pick correct values from random cells
  std::set<std::pair<std::pair<unsigned, unsigned>, unsigned>> clueCells;
  std::uniform_int_distribution<unsigned> range(0, 8);
  std::random_device r;
  std::default_random_engine random_engine(r());

  while(clueCells.size() < num_clues)
  {
    // Get a random row
    auto rowIdx = range(random_engine);
    auto colIdx = range(random_engine);
    clueCells.insert(std::make_pair(std::make_pair(rowIdx, colIdx), solvedBoard[rowIdx][colIdx]));
  }

  for (const auto& cell : clueCells)
  {
    board_[cell.first.first][cell.first.second] = cell.second;
  }
  solution_ = solvedBoard.GetBoard();
  return true;
}
bool Board::Solve()
{
  // Check if starting board is valid
  if(!CheckBoard())
  {
    return false;
  }
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
  // Create a random sequence of possible values
  std::array<unsigned, 9> possibleValues{1, 2, 3, 4, 5, 6, 7, 8, 9};
  // Obtain a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(possibleValues.begin(), possibleValues.end(), std::default_random_engine(seed));
  for (const auto& value : possibleValues)
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
bool Board::CheckBox(unsigned row, unsigned col) const
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
bool Board::IsInBox(unsigned int value, unsigned int rowIdx, unsigned int colIdx) const
{
  return IsInSection(value, GetBox(rowIdx, colIdx));
}
bool Board::IsInCol(unsigned int value, unsigned int colIdx) const
{
  return IsInSection(value, GetCol(colIdx));
}
bool Board::IsInRow(unsigned int value, unsigned int rowIdx) const
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
bool Board::IsValid(unsigned int rowIdx, unsigned int colIdx, unsigned int value) const
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
unsigned Board::GetCell(unsigned int rowIdx, unsigned int colIdx)
{
  return board_[rowIdx][colIdx];
}
std::array<unsigned, 9> Board::operator[](unsigned i) const
{
  return board_[i];
}
std::array<unsigned, 9> &Board::operator[](unsigned i)
{
  return board_[i];
}
void Board::Clear()
{
  board_.fill(std::array<unsigned, 9>{0});
}
bool Board::operator==(const Board &rhs) const
{
  return board_ == rhs.board_;
}
bool Board::operator!=(const Board &rhs) const
{
  return !(rhs == *this);
}
unsigned Board::GetHint(unsigned int row, unsigned int col) const
{
  return solution_[row][col];
}

