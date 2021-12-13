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

class SudokuGrid : public wxGrid
{
 public:
  SudokuGrid() = default;
  explicit SudokuGrid(wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition,
             const wxSize &size=wxDefaultSize, long style=wxTAB_TRAVERSAL, const wxString &name=wxPanelNameStr);
  ~SudokuGrid() override = default;

 private:
  void OnSize(wxSizeEvent& event);
  void OnEditorCreated(wxGridEditorCreatedEvent& event);

  void CenterGrid();
  int GetGridWidth();
  int GetGridHeight();
  void RecalcCellSize();
  void RecalcFontSize();
  void SetupBorders();


  wxTextCtrl* openEditor_{nullptr};
 DECLARE_EVENT_TABLE()
};

#endif //SUDOKU_SRC_SUDOKUGRID_H_
