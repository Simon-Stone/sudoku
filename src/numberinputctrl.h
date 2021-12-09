//
// Created by Simon Stone on 12/8/2021.
//

#ifndef SUDOKU_SRC_NUMBERINPUTCTRL_H_
#define SUDOKU_SRC_NUMBERINPUTCTRL_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class NumberInputCtrl : public wxTextCtrl
{
 public:
  NumberInputCtrl() = default;
  NumberInputCtrl(wxWindow *parent, wxWindowID id);
  ~NumberInputCtrl() override = default;


  void OnSize(wxSizeEvent& event);

 DECLARE_EVENT_TABLE()
};

#endif //SUDOKU_SRC_NUMBERINPUTCTRL_H_
