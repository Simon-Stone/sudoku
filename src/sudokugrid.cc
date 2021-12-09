//
// Created by Simon Stone on 12/8/2021.
//

#include "sudokugrid.h"
#include "numberinputctrl.h"

#include <wx/valnum.h>



SudokuGrid::SudokuGrid(wxWindow *parent,
                       wxWindowID id,
                       const wxPoint &pos,
                       const wxSize &size,
                       long style,
                       const wxString &name) : wxPanel(parent, id, pos, size, style, name)
{
  auto* gridSizer = new wxGridSizer(9,9, 0, 0);
  for (auto i = 0; i < 81; ++i)
  {
    auto* nic = new NumberInputCtrl(this, wxID_ANY);
    auto val = wxIntegerValidator(&m_grid[i]);
    val.SetRange(1, 9);
    nic->SetValidator(val);
    gridSizer->Add(nic, wxSizerFlags(1).Border(wxALL, 0).Expand());
  }

  this->SetSizerAndFit(gridSizer);
}



