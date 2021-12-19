//
// Created by Simon Stone on 12/8/2021.
//

#include "mainwindow.h"

#include <wx/aboutdlg.h>
#include <wx/artprov.h>
#include <wx/numdlg.h>

static const int ID_NEW_GAME = wxNewId();
static const int ID_SOLVE = wxNewId();
static const int ID_REDO = wxNewId();
static const int ID_RESET = wxNewId();
static const int ID_UNDO = wxNewId();


wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
        EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
        EVT_MENU(wxID_EXIT, MainWindow::OnExit)
        EVT_MENU(ID_NEW_GAME, MainWindow::OnNewGame)
        EVT_MENU(ID_SOLVE, MainWindow::OnSolve)
        EVT_MENU(ID_REDO, MainWindow::OnRedo)
        EVT_MENU(ID_RESET, MainWindow::OnReset)
        EVT_MENU(ID_UNDO, MainWindow::OnUndo)
        EVT_UPDATE_UI(ID_REDO, MainWindow::OnUpdateUI)
        EVT_UPDATE_UI(ID_RESET, MainWindow::OnUpdateUI)
        EVT_UPDATE_UI(ID_UNDO, MainWindow::OnUpdateUI)
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
  menuEdit->Append(ID_REDO, "Redo");
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
  // ************************************************************
  auto* toolBar = CreateToolBar();
  toolBar->AddTool(ID_NEW_GAME, "New Game", wxArtProvider::GetBitmap(wxART_NEW));
  toolBar->AddTool(ID_RESET, "Reset Game", wxArtProvider::GetBitmap(wxART_GOTO_FIRST));
  toolBar->AddTool(ID_UNDO, "Undo", wxArtProvider::GetBitmap(wxART_UNDO));
  toolBar->AddTool(ID_REDO, "Redo", wxArtProvider::GetBitmap(wxART_REDO));
  toolBar->AddTool(ID_SOLVE, "Solve", wxArtProvider::GetBitmap(wxART_TIP));
  toolBar->Realize();
  // ************************************************************
  CreateStatusBar();
  SetStatusText("Welcome to Sudoku!");
  // ************************************************************
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
void MainWindow::OnRedo(wxCommandEvent &event)
{
  grid_->Redo();
  updateWidgets();
}
void MainWindow::OnUndo(wxCommandEvent &event)
{
  grid_->Undo();
  updateWidgets();
}
void MainWindow::OnUpdateUI(wxUpdateUIEvent &event)
{
  if(event.GetId() == ID_UNDO || event.GetId() == ID_RESET)
  {
    event.Enable(grid_->UndoStepsLeft() > 0);
  }
  else if(event.GetId() == ID_REDO)
  {
    event.Enable(grid_->RedoStepsLeft() > 0);
  }
}
void MainWindow::OnReset(wxCommandEvent &event)
{
  grid_->Undo(grid_->UndoStepsLeft());
  updateWidgets();
}


