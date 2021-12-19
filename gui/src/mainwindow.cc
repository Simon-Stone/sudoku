//
// Created by Simon Stone on 12/8/2021.
//

#include "mainwindow.h"

#include <wx/aboutdlg.h>
#include <wx/numdlg.h>

static const int ID_NEW_GAME = wxNewId();
static const int ID_SOLVE = wxNewId();
static const int ID_UNDO = wxNewId();

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
        EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
        EVT_MENU(wxID_EXIT, MainWindow::OnExit)
        EVT_MENU(ID_NEW_GAME, MainWindow::OnNewGame)
        EVT_MENU(ID_SOLVE, MainWindow::OnSolve)
        EVT_MENU(ID_UNDO, MainWindow::OnUndo)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
  // ************************************************************
  wxMenu *menuFile = new wxMenu();
  menuFile->Append(ID_NEW_GAME, "New Game");
  menuFile->Append(wxID_EXIT);
  // ************************************************************
  wxMenu *menuEdit = new wxMenu();
  menuEdit->Append(ID_UNDO, "Undo");
  // ************************************************************
  wxMenu *menuHelp = new wxMenu();
  menuHelp->Append(wxID_ABOUT);
  menuHelp->Append(ID_SOLVE, "Solve");
  // ************************************************************
  wxMenuBar *menuBar = new wxMenuBar();
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuEdit, "&Edit");
  menuBar->Append(menuHelp, "&Help");
  // ************************************************************
  SetMenuBar(menuBar);
  CreateStatusBar();
  SetStatusText("Welcome to Sudoku!");

  grid_ = new SudokuGrid(this);
  auto *szr = new wxBoxSizer(wxHORIZONTAL);
  szr->Add(grid_, wxSizerFlags(1).Align(wxALIGN_CENTER).Expand());
  this->SetSizer(szr);
}
void MainWindow::OnExit(wxCommandEvent &event)
{
  Close(true);
}
void MainWindow::OnAbout(wxCommandEvent &event)
{
  wxAboutDialogInfo aboutInfo;
  aboutInfo.SetName("SudokuGame");
  aboutInfo.SetVersion("0.0.1");
  aboutInfo.SetDescription(_("A wxWidgets-based SudokuGame game"));
  aboutInfo.SetCopyright("(C) 2021");
  aboutInfo.SetWebSite("https://github.com/nullpunktTUD/backend");
  aboutInfo.AddDeveloper("Simon Stone");
  wxAboutBox(aboutInfo);
}
void MainWindow::OnSolve(wxCommandEvent &event)
{
  if (wxMessageDialog(this,
                      "Are you sure you want to automatically solve the current board?",
                      "Confirmation",
                      wxYES_NO).ShowModal() == wxID_YES)
  {
    grid_->Solve();
    updateWidgets();
  }
}
void MainWindow::updateWidgets()
{
  grid_->updateWidgets();
  this->Refresh();
}
void MainWindow::OnNewGame(wxCommandEvent &event)
{
  auto numClues = wxGetNumberFromUser("Please choose the number of initially filled cells!",
                      "Initial clues:",
                      "New Game", 5, 0, 80);
  if (numClues == -1)
  {
    // Dialog was canceled
    return;
  }

  grid_->Initialize(numClues);
  updateWidgets();
}
void MainWindow::OnUndo(wxCommandEvent &event)
{
  grid_->Undo();
  updateWidgets();
}


