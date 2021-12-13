//
// Created by Simon Stone on 12/8/2021.
//

#include "mainwindow.h"
#include "sudokugrid.h"

#include <wx/aboutdlg.h>

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
                EVT_MENU(wxID_EXIT,  MainWindow::OnExit)
                EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "Welcome to Sudoku!" );

    auto* grid = new SudokuGrid(this);
    auto* szr = new wxBoxSizer(wxHORIZONTAL);
    szr->Add(grid, wxSizerFlags(1).Align(wxALIGN_CENTER).Expand());
    this->SetSizer(szr);
}
void MainWindow::OnExit(wxCommandEvent& event)
{
    Close( true );
}
void MainWindow::OnAbout(wxCommandEvent& event)
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


