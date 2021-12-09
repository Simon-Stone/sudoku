//
// Created by Simon Stone on 12/8/2021.
//

#include "numberinputctrl.h"

wxBEGIN_EVENT_TABLE(NumberInputCtrl, wxPanel)
  EVT_SIZE(NumberInputCtrl::OnSize)
wxEND_EVENT_TABLE()


NumberInputCtrl::NumberInputCtrl(wxWindow *parent,
                                 wxWindowID id)
                                 : wxTextCtrl(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                              wxTE_CENTER | wxBORDER_SIMPLE)
{
}

void NumberInputCtrl::OnSize(wxSizeEvent& event)
{
  auto sz = event.GetSize();
  auto font = this->GetFont();
  auto newSz = wxSize(font.GetPixelSize().x, sz.y*0.75);
  font.SetPixelSize(newSz);
  this->SetFont(font);
  event.Skip();
}