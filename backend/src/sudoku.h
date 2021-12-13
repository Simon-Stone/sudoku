//
// Created by Simon Stone on 12/10/2021.
//

#ifndef SUDOKU_SRC_SUDOKU_H_
#define SUDOKU_SRC_SUDOKU_H_

#include <vector>

class Sudoku
{
 public:
  static std::vector<std::vector<unsigned>> GetInitializedBoard(unsigned num_clues);

  static bool Check(const std::vector<unsigned>& section);
  static bool Check(const std::vector<std::vector<unsigned>>& board);
  static bool Check(const std::vector<std::vector<unsigned>>& board, unsigned row, unsigned col);
  static std::vector<std::vector<unsigned>> Solve(std::vector<std::vector<unsigned>> board);

  static std::vector<unsigned> GetBox(const std::vector<std::vector<unsigned>>& board, unsigned rowIdx, unsigned colIdx);
  static std::vector<unsigned> GetCol(const std::vector<std::vector<unsigned>>& board, unsigned colIdx);
  static std::vector<unsigned> GetRow(const std::vector<std::vector<unsigned>>& board, unsigned rowIdx);


};

#endif //SUDOKU_SRC_SUDOKU_H_
