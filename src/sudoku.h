//
// Created by Simon Stone on 12/8/2021.
//

#ifndef SUDOKU_SUDOKU_H_
#define SUDOKU_SUDOKU_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class Sudoku: public wxApp
{
public:
    virtual bool OnInit();
};
DECLARE_APP(Sudoku)
#endif //SUDOKU_SUDOKU_H_
