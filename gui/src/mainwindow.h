//
// Created by Simon Stone on 12/8/2021.
//

#ifndef SUDOKU_MAINWINDOW_H
#define SUDOKU_MAINWINDOW_H

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "sudoku.h"
#include "sudokugrid.h"

class MainWindow : public wxFrame
{
 public:
  MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);
 private:
  void OnAbout(wxCommandEvent &event);
  void OnExit(wxCommandEvent &event);
  void OnNewGame(wxCommandEvent &event);
  void OnSolve(wxCommandEvent &event);
  void OnUndo(wxCommandEvent &event);

  void updateWidgets();
 wxDECLARE_EVENT_TABLE();

 private:
  SudokuGrid *grid_;
};
enum
{
  ID_Hello = 1
};

#endif //SUDOKU_MAINWINDOW_H
