//
// Created by Simon Stone on 12/10/2021.
//

#ifndef SUDOKU_SRC_SUDOKU_H_
#define SUDOKU_SRC_SUDOKU_H_

#include <array>
#include <vector>
#include <ostream>

namespace sudoku
{

class Board
{
 public:
  Board() = default;
  explicit Board(const std::array<std::array<unsigned, 9>, 9>& board);

  bool Initialize(unsigned num_clues);

  [[nodiscard]] std::array<std::array<unsigned, 9>, 9> GetBoard() const;

  void SetBoard(const std::array<std::array<unsigned, 9>, 9>& board);

  bool CheckBoard();
  bool CheckBox(unsigned row, unsigned col) const;
  static bool CheckSection(const std::array<unsigned, 9>& section);

  bool Solve();

  bool IsInBox(unsigned value, unsigned rowIdx, unsigned colIdx) const;
  bool IsInCol(unsigned value, unsigned colIdx) const;
  bool IsInRow(unsigned value, unsigned rowIdx) const;
  static bool IsInSection(unsigned value, const std::array<unsigned, 9>& section);

  [[nodiscard]] std::array<unsigned, 9> GetBox(unsigned rowIdx, unsigned colIdx) const;
  [[nodiscard]] std::array<unsigned, 9> GetCol(unsigned colIdx) const;
  [[nodiscard]] unsigned GetNumberCols() const;
  [[nodiscard]] std::array<unsigned, 9> GetRow(unsigned rowIdx) const;
  [[nodiscard]] unsigned GetNumberRows() const;

  void SetCell(unsigned rowIdx, unsigned colIdx, unsigned value);

  friend std::ostream &operator<<(std::ostream &os, const Board &board);

 private:
  std::pair<unsigned, unsigned> FindEmptyCell();
  bool IsValid(unsigned rowIdx, unsigned colIdx, unsigned value) const;

 private:
  std::array<std::array<unsigned, 9>, 9> board_{std::array<unsigned, 9>{0}};
};

};


#endif //SUDOKU_SRC_SUDOKU_H_
