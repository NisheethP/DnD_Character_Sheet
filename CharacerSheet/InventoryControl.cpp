#include "PreCompiledHeader.h"
#include "InventoryControl.h"

InventoryControl::InventoryControl(
	wxWindow* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) :
	wxPanel(parent, id, pos, size, style, name)
{
	list = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
		wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_SINGLE_SEL | wxLC_EDIT_LABELS);

	list->AppendColumn("Item");
	list->AppendColumn("Count");
	list->AppendColumn("Weight");

	list->SetColumnWidth(0, size.x * .6);
	list->SetColumnWidth(1, size.x * .2);
	list->SetColumnWidth(2, size.x * .2);

	auto sizer = new wxBoxSizer(wxVERTICAL);
	auto titleSizer = new wxBoxSizer(wxHORIZONTAL);
	auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);

	wxSize bSize(40, -1);
	Add = new wxButton(this, wxID_ANY, "", wxDefaultPosition, bSize);
	Rem = new wxButton(this, wxID_ANY, "", wxDefaultPosition, bSize);

	Add->SetBitmap(wxArtProvider().GetBitmap(wxART_PLUS, wxART_BUTTON));
	Rem->SetBitmap(wxArtProvider().GetBitmap(wxART_MINUS, wxART_BUTTON));

	title = new wxStaticText(this, wxID_ANY, "Inventory", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	title->SetFont(title->GetFont().MakeBold().MakeLarger());
	title->SetForegroundColour(*wxWHITE);

	auto titleLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxLI_VERTICAL);

	buttonSizer->Add(Rem, 0, wxALL, 3);
	buttonSizer->Add(Add, 0, wxALL, 3);

	weight = new wxStaticText(this, wxID_ANY, "Weight : 0", wxDefaultPosition, wxDefaultSize, wxHTML_ALIGN_CENTER | wxTE_READONLY);
	weight->SetForegroundColour(*wxWHITE);
	weight->SetFont(weight->GetFont().MakeBold().MakeLarger());

	titleSizer->Add(title, 0, wxEXPAND | wxRIGHT | wxLEFT, 10);
	titleSizer->Add(titleLine, 0, wxEXPAND | wxLEFT | wxRIGHT, 3);
	titleSizer->Add(weight, 0, wxEXPAND | wxLEFT, 3);
	titleLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxLI_VERTICAL);
	titleSizer->Add(titleLine, 0, wxEXPAND | wxLEFT | wxRIGHT, 3);
	titleSizer->Add(buttonSizer, 0, wxLEFT, 5);

	

	sizer->Add(titleSizer, 0, wxEXPAND | wxRIGHT, 5);
	sizer->Add(list, 1, wxEXPAND);

	SetSizer(sizer);
	Layout();

	Add->Bind(wxEVT_BUTTON, &InventoryControl::onAddButton, this);
	Rem->Bind(wxEVT_BUTTON, &InventoryControl::onRemButton, this);
	list->Bind(wxEVT_LIST_ITEM_ACTIVATED, &InventoryControl::onListDClick, this);
	Layout();
}

void InventoryControl::onAddButton(wxCommandEvent& event)
{
}

void InventoryControl::onRemButton(wxCommandEvent& event)
{
}

void InventoryControl::onListDClick(wxListEvent& event)
{
}

void InventoryControl::resizeCtrl()
{
	auto size = this->GetMinSize();
	list->SetColumnWidth(0, size.x * .6);
	list->SetColumnWidth(1, size.x * .2);
	list->SetColumnWidth(2, size.x * .2);

	title->SetMinSize(wxSize(size.x - 2 * (40 + 5 + 6 + 10 + 5) - 5 - 3 - weight->GetSize().x*2, -1));
	weight->SetMinSize(weight->GetSize() * 2);
	Layout();
}
