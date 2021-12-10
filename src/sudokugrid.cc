//
// Created by Simon Stone on 12/8/2021.
//

#include "sudokugrid.h"

#include <wx/valnum.h>

wxBEGIN_EVENT_TABLE(SudokuGrid, wxGrid)
  EVT_SIZE(SudokuGrid::OnSize)
  EVT_GRID_EDITOR_CREATED(SudokuGrid::OnEditorCreated)
wxEND_EVENT_TABLE()

SudokuGrid::SudokuGrid(wxWindow *parent,
                       wxWindowID id,
                       const wxPoint &pos,
                       const wxSize &size,
                       long style,
                       const wxString &name) : wxGrid(parent, id, pos, size, style, name)
{
  this->CreateGrid(9, 10);
  this->HideColLabels();
  this->HideRowLabels();
  this->DisableDragRowSize();
  this->DisableDragColSize();
  this->SetDefaultRowSize(this->GetDefaultColSize());
  this->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
  RecalcFontSize();
  CenterGrid();
}

void SudokuGrid::OnSize(wxSizeEvent &event)
{
  if (openEditor_)
  {
    openEditor_->Show(false);
  }

  RecalcCellSize();
  RecalcFontSize();
  CenterGrid();
  this->Refresh();
  event.Skip();
}

void SudokuGrid::OnEditorCreated(wxGridEditorCreatedEvent& event)
{
  auto val = wxIntegerValidator<unsigned>();
  val.SetRange(1, 9);
  dynamic_cast<wxTextCtrl*>(event.GetControl())->SetValidator(val);
  openEditor_ = dynamic_cast<wxTextCtrl*>(event.GetControl());
}

void SudokuGrid::RecalcFontSize()
{
  auto font = this->GetDefaultCellFont();
  font.SetPixelSize(wxSize(0, this->GetDefaultColSize() * 0.75));
  this->SetDefaultCellFont(font);
}

void SudokuGrid::RecalcCellSize()
{
  auto newSize = this->GetSize();
  this->SetDefaultRowSize(newSize.GetHeight() / 9);
  this->SetDefaultColSize(newSize.GetHeight() / 9);
}

void SudokuGrid::CenterGrid()
{
  // Center the grid on the area available to it
  auto size = this->GetSize();

  auto whitespace = (size.GetWidth() - this->GetGridWidth()) / 2;

  this->SetColSize(0, whitespace);
  for (int i = 1; i < this->GetNumberCols(); ++i)
  {
    this->SetColSize(i, this->GetRowSize(0));
  }
}

int SudokuGrid::GetGridWidth()
{
  int sum = 0;
  for (unsigned i = 1; i < this->GetNumberCols(); ++i)
  {
    sum += this->GetColSize(i);
  }
  return sum;
}

int SudokuGrid::GetGridHeight()
{
  int sum = 0;
  for (unsigned i = 0; i < this->GetNumberRows(); ++i)
  {
    sum += this->GetRowSize(i);
  }
  return sum;
}
wxPen SudokuGrid::GetColGridLinePen(int col)
{
  auto gridPen = wxGrid::GetColGridLinePen(col);
  gridPen.SetColour(*wxBLACK);
  switch(col)
  {
    case 0:
    case 3:
    case 6:
    case 9:
      gridPen.SetWidth(gridPen.GetWidth() * 3);
      break;
    default:
      break;
  }
  return gridPen;
}

wxPen SudokuGrid::GetRowGridLinePen(int col)
{
  auto gridPen = wxGrid::GetRowGridLinePen(col);
  gridPen.SetColour(*wxBLACK);
  switch(col)
  {
    case 2:
    case 5:
    case 9:
      gridPen.SetWidth(gridPen.GetWidth() * 3);
      break;
    default:
      break;
  }
  return gridPen;
}

