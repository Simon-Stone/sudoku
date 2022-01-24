//
// Created by Simon Stone on 12/8/2021.
//

#include "sudokugrid.h"

#include <array>

#include <wx/valnum.h>

static const int ID_HINT = wxNewId();

wxBEGIN_EVENT_TABLE(SudokuGrid, wxGrid)
  EVT_SIZE(SudokuGrid::OnSize)
  EVT_GRID_CELL_CHANGED(SudokuGrid::OnCellChanged)
  EVT_GRID_CELL_RIGHT_CLICK(SudokuGrid::OnRightClick)
  EVT_GRID_EDITOR_CREATED(SudokuGrid::OnEditorCreated)
  EVT_MENU(ID_HINT, SudokuGrid::OnHint)
wxEND_EVENT_TABLE()

namespace
{
  class CellBorderRenderer : public wxGridCellNumberRenderer
  {
   public:
    CellBorderRenderer(const wxPen& top=*wxBLACK_PEN, const wxPen& right=*wxBLACK_PEN,
                       const wxPen& bottom=*wxBLACK_PEN, const wxPen& left=*wxBLACK_PEN,
                       bool hideSelection=false)
    : pen_({top, right, bottom, left}), hideSelection_(hideSelection)
    {

    }
    void Draw(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, const wxRect &rect, int row, int col, bool isSelected) override
    {
      attr.SetAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
      wxGridCellNumberRenderer::Draw(grid, attr, dc, rect, row, col, hideSelection_ ? false : isSelected);

      std::array<wxPoint, 5> points{rect.GetTopLeft(), rect.GetTopRight(),
                                    rect.GetBottomRight(), rect.GetBottomLeft(),
                                    rect.GetTopLeft()};

      for (unsigned i = 0; i < pen_.size(); ++i)
      {
        dc.SetPen(pen_[i]);
        dc.DrawLine(points[i], points[i+1]);
      }
    }
   private:
    std::array<wxPen, 4> pen_{*wxBLACK_PEN, *wxBLACK_PEN, *wxBLACK_PEN, *wxBLACK_PEN,};
    bool hideSelection_{false};
  };
}


SudokuGrid::SudokuGrid(wxWindow *parent,
                       wxWindowID id,
                       const wxPoint &pos,
                       const wxSize &size,
                       long style,
                       const wxString &name) : wxGrid(parent, id, pos, size, style, name)
{
  this->CreateGrid(11, 11);
  this->HideColLabels();
  this->HideRowLabels();
  this->DisableDragRowSize();
  this->DisableDragColSize();
  this->SetDefaultRowSize(this->GetDefaultColSize());

  RecalcFontSize();
  CenterGrid();

  SetupBorders();

  // Make cells outside of game board read-only
  for (int r = 0; r < this->GetNumberRows(); ++r)
  {
    for (int c = 0; c < this->GetNumberCols(); ++c)
    {
      if (c == 0 || c == this->GetNumberCols() - 1 || r == 0 || r == this->GetNumberRows() - 1)
      {
        this->SetReadOnly(r, c, true);
      }
    }
  }
  this->SetCellHighlightROPenWidth(0);

  history_.emplace_back();
  board_ = history_.begin();
  FillGrid();
}

void SudokuGrid::OnSize(wxSizeEvent &event)
{
  if (openEditor_)
  {
    openEditor_->Show(false);
  }
  CenterGrid();
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
  font.SetPixelSize(wxSize(0, this->GetDefaultColSize() * 0.6));
  this->SetDefaultCellFont(font);
}

void SudokuGrid::RecalcCellSize()
{
  auto newSize = this->GetSize();
  this->SetDefaultRowSize(newSize.GetHeight() / 11);
  this->SetDefaultColSize(newSize.GetHeight() / 11);
}

void SudokuGrid::CenterGrid()
{
  RecalcCellSize();
  RecalcFontSize();
  // Center the grid on the area available to it
  auto size = this->GetSize();

  auto whitespace = (size.GetWidth() - this->GetGridWidth()) / 2;

  this->SetColSize(0, whitespace);
  for (int i = 1; i < this->GetNumberCols() - 1; ++i)
  {
    this->SetColSize(i, this->GetRowSize(0));
  }
  this->SetColSize(this->GetNumberCols() - 1, whitespace);
}

int SudokuGrid::GetGridWidth()
{
  int sum = 0;
  // The first and the last column is just for framing purposes. They are not part of the game board!
  for (unsigned i = 1; i < this->GetNumberCols() - 1; ++i)
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

void SudokuGrid::SetupBorders()
{
  this->EnableGridLines(false);

  const auto thickBlackPen = wxPen(*wxBLACK, this->FromDIP(2));

  for (int r = 0; r < this->GetNumberRows(); ++r)
  {
    for (int c = 0; c < this->GetNumberCols(); ++c)
    {
      wxPen topPen{*wxLIGHT_GREY_PEN}, rightPen{*wxLIGHT_GREY_PEN}, bottomPen{*wxLIGHT_GREY_PEN}, leftPen{*wxLIGHT_GREY_PEN};
      bool hideSelection = false;

      if (c == 0)  // Empty buffer column for framing purposes, not part of the game board
      {
        topPen = *wxTRANSPARENT_PEN;
        rightPen = thickBlackPen;
        bottomPen = *wxTRANSPARENT_PEN;
        leftPen = *wxTRANSPARENT_PEN;
        hideSelection = true;
      }

      if (c == this->GetNumberCols() - 1)  // Empty buffer column for framing purposes, not part of the game board
      {
        topPen = *wxTRANSPARENT_PEN;
        rightPen = *wxTRANSPARENT_PEN;
        bottomPen = *wxTRANSPARENT_PEN;
        leftPen = *wxTRANSPARENT_PEN;
        hideSelection = true;
      }

      if (c % 3 == 0)  // 3x3 subgrid boxes have thick border
      {
        rightPen = thickBlackPen;
      }

      if (r == 0)  // Empty buffer row for framing purposes, not part of the game board
      {
        topPen = *wxTRANSPARENT_PEN;
        rightPen = *wxTRANSPARENT_PEN;
        bottomPen = c == 0 || c == this->GetNumberCols() - 1 ? *wxTRANSPARENT_PEN : thickBlackPen;
        leftPen = *wxTRANSPARENT_PEN;
        hideSelection = true;
      }

      if (r == this->GetNumberRows() - 1)  // Empty buffer row for framing purposes, not part of the game board
      {
        topPen = c == 0 || c == this->GetNumberCols() - 1 ? *wxTRANSPARENT_PEN : thickBlackPen;
        rightPen = *wxTRANSPARENT_PEN;
        bottomPen = *wxTRANSPARENT_PEN;
        leftPen = *wxTRANSPARENT_PEN;
        hideSelection = true;
      }

      if (r % 3 == 0)  // 3x3 subgrid boxes have thick border
      {
        bottomPen = c == 0 || c == this->GetNumberCols() - 1 ? *wxTRANSPARENT_PEN : thickBlackPen;
      }
      this->SetCellRenderer(r, c, new CellBorderRenderer(topPen, rightPen, bottomPen, leftPen, hideSelection));
    }
  }
}
void SudokuGrid::FillGrid()
{
  numFilledCells_ = 0;
  for (unsigned rowIdx = 0; rowIdx < 9; ++rowIdx)
  {
    for (unsigned colIdx = 0; colIdx < 9; ++colIdx)
    {
      std::string valueString;
      if((*board_)[rowIdx][colIdx] != 0)
      {
        valueString = std::to_string((*board_)[rowIdx][colIdx]);
        numFilledCells_++;
      }
      this->SetCellValue(rowIdx + 1, colIdx + 1, valueString);
    }
  }
}
void SudokuGrid::updateWidgets()
{
  FillGrid();
  Refresh();
}
void SudokuGrid::OnCellChanged(wxGridEvent &event)
{
  history_.push_back(*board_);
  board_ = std::prev(history_.end());
  history_.erase(std::next(board_), history_.end());
  (*board_)[event.GetRow() - 1][event.GetCol() - 1] = wxAtoi(this->GetCellValue(event.GetRow(), event.GetCol()));

  updateWidgets();

  if(numFilledCells_ == 81)
  {
    if(board_->CheckBoard())
    {
      wxMessageDialog(this, "Congratulations! You solved the board!", "Game Won").ShowModal();
    }
    else
    {
      wxMessageDialog(this, "Oh no! Your solution is not correct!", "Invalid board", wxICON_ERROR).ShowModal();
    }
  }
}
bool SudokuGrid::Solve()
{
  history_.push_back(*board_);
  board_ = std::prev(history_.end());
  history_.erase(std::next(board_), history_.end());
  return board_->Solve();
}
bool SudokuGrid::Initialize(unsigned int numClues)
{
    return board_->Initialize(numClues);
}
void SudokuGrid::Undo(unsigned steps)
{
  if (std::distance(history_.begin(), board_) < steps)
  {
    return;
  }
  board_ -= steps;
  updateWidgets();
}
unsigned SudokuGrid::UndoStepsLeft()
{
  return static_cast<unsigned>(std::distance(history_.begin(), board_));
}
unsigned SudokuGrid::RedoStepsLeft()
{
  return static_cast<unsigned>(std::distance(board_, std::prev(history_.end())));
}
void SudokuGrid::Redo(unsigned int steps)
{
  if (RedoStepsLeft() < steps)
  {
      return;
  }
  board_ += steps;
  updateWidgets();
}
void SudokuGrid::OnRightClick(wxGridEvent& event)
{
  wxPoint point = event.GetPosition();
  SetGridCursor(event.GetRow(), event.GetCol());
  wxMenu *menu = new wxMenu();
  menu->Append(ID_HINT, "Get a hint");
  PopupMenu( menu, point);
}
void SudokuGrid::OnHint(wxCommandEvent &event)
{
  // The grid coordinates are off by one from the board coordinates
  auto row = GetGridCursorRow() - 1;
  auto col = GetGridCursorCol() - 1;

  history_.push_back(*board_);
  board_ = std::prev(history_.end());
  history_.erase(std::next(board_), history_.end());
  try
  {
    (*board_).SetCell(row, col, (*board_).GetHint(row, col));
  }
  catch (std::out_of_range&)
  {
    history_.erase(history_.begin(), board_);
    board_ = history_.begin();
    wxMessageBox("Please start a new game first!", "No active game", wxICON_INFORMATION);
  }

  updateWidgets();
}


