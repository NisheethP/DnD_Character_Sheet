#pragma once

#include "Item.h"

class ItemDisplayControl : public wxPanel
{
	Item item;
	wxStaticText* Name = nullptr;
	wxStaticText* Description = nullptr;
	wxStaticText* Weight = nullptr;
	wxStaticText* reqAttunement = nullptr;
public:
	ItemDisplayControl(wxWindow* parent,
		wxWindowID 	id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxTAB_TRAVERSAL,
		const wxString& name = wxPanelNameStr
	);
};

