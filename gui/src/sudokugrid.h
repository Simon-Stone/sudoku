//
// Created by Simon Stone on 12/8/2021.
//

#ifndef SUDOKU_SRC_SUDOKUGRID_H_
#define SUDOKU_SRC_SUDOKUGRID_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>

#include "sudoku.h"

class SudokuGrid : public wxGrid
{
 public:
  SudokuGrid() = default;
  explicit SudokuGrid(wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition,
             const wxSize &size=wxDefaultSize, long style=wxTAB_TRAVERSAL, const wxString &name=wxPanelNameStr);
  ~SudokuGrid() override = default;

  bool Initialize(unsigned numClues);
  bool Solve();
  void Undo(unsigned steps = 1);
  void updateWidgets();

 private:
  void OnCellChanged(wxGridEvent& event);
  void OnEditorCreated(wxGridEditorCreatedEvent& event);
  void OnSize(wxSizeEvent& event);


  void CenterGrid();
  void FillGrid();
  int GetGridWidth();
  int GetGridHeight();
  void RecalcCellSize();
  void RecalcFontSize();
  void SetupBorders();

  std::vector<sudoku::Board>::iterator board_;
  std::vector<sudoku::Board> history_;
  unsigned numFilledCells_;
  wxTextCtrl* openEditor_{nullptr};
 DECLARE_EVENT_TABLE()
};

#endif //SUDOKU_SRC_SUDOKUGRID_H_
