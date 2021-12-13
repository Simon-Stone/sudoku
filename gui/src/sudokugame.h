//
// Created by Simon Stone on 12/8/2021.
//

#ifndef SUDOKUGAME_H_
#define SUDOKUGAME_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class SudokuGame: public wxApp
{
public:
    virtual bool OnInit();
};
DECLARE_APP(SudokuGame)
#endif //SUDOKUGAME_H_
