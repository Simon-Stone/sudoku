//
// Created by Simon Stone on 12/8/2021.
//

#ifndef SUDOKU_SRC_SUDOKUGRID_H_
#define SUDOKU_SRC_SUDOKUGRID_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class SudokuGrid : public wxPanel
{
 public:
  SudokuGrid() = default;
  explicit SudokuGrid(wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition,
             const wxSize &size=wxDefaultSize, long style=wxTAB_TRAVERSAL, const wxString &name=wxPanelNameStr);
  ~SudokuGrid() override = default;

 private:
  std::vector<unsigned> m_grid = std::vector<unsigned>(81);
};

#endif //SUDOKU_SRC_SUDOKUGRID_H_
