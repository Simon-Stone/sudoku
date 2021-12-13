//
// Created by Simon Stone on 12/8/2021.
//
#include "sudokugame.h"
#include "mainwindow.h"

wxIMPLEMENT_APP(SudokuGame);

bool SudokuGame::OnInit()
{
    MainWindow *frame = new MainWindow( "SudokuGame", wxDefaultPosition, wxSize(800, 600) );
    frame->CenterOnScreen();
    frame->Show( true );
    return true;
}
