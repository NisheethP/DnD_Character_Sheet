#include "DeathSavesControl.h"

#include <wx/graphics.h>
#include <wx/dcbuffer.h>

DeathSavesControl::DeathSavesControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
	wxWindow(parent, id, pos, size, style, name)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	this->Bind(wxEVT_PAINT, &DeathSavesControl::OnPaint, this);
}

void DeathSavesControl::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	if (gc)
	{
		
		wxSize rectSize1 = this->FromDIP(wxSize(200,100));
		wxSize rectSize2 = this->FromDIP(wxSize(rectSize1.x-thickness, rectSize1.x - thickness));
		wxDouble rad = this->FromDIP(10.f);
		wxPoint rectOrigin = {};


		wxColour fg = this->GetForegroundColour();
		wxColour bg = this->GetBackgroundColour();


		gc->SetBrush(fg);
		gc->DrawRoundedRectangle(rectOrigin.x, rectOrigin.y, rectSize1.GetWidth(), rectSize1.GetHeight(), rad);
		
		gc->SetBrush(bg);
		gc->DrawRoundedRectangle(rectOrigin.x+10, rectOrigin.y+10, rectSize2.GetWidth(), rectSize2.GetHeight(), rad);

		

		delete gc;
	}
}
