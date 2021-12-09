//
// Created by Simon Stone on 12/8/2021.
//
#include "sudoku.h"
#include "mainwindow.h"

wxIMPLEMENT_APP(Sudoku);

bool Sudoku::OnInit()
{
    MainWindow *frame = new MainWindow( "Sudoku", wxDefaultPosition, wxSize(800, 600) );
    frame->CenterOnScreen();
    frame->Show( true );
    return true;
}
