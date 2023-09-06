#include "AttackControl.h"
#include <wx/artprov.h>

AttackControl::AttackControl(
	wxWindow* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) :
	wxPanel(parent, id, pos, size, style, name)
{
	list = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_SINGLE_SEL);

	list->AppendColumn("NAME");
	list->AppendColumn("ATK MOD");
	list->AppendColumn("DMG/TYPE");

	list->SetColumnWidth(0, size.x*.4);
	list->SetColumnWidth(1, size.x*.2);
	list->SetColumnWidth(2, size.x*.4);
	auto sizer = new wxBoxSizer(wxVERTICAL);
	auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);

	wxSize bSize(40, -1);
	Add = new wxButton(this, wxID_ANY, "", wxDefaultPosition, bSize);
	Rem = new wxButton(this, wxID_ANY, "", wxDefaultPosition, bSize);

	Add->SetBitmap(wxArtProvider().GetBitmap(wxART_PLUS, wxART_BUTTON));
	Rem->SetBitmap(wxArtProvider().GetBitmap(wxART_MINUS, wxART_BUTTON));

	buttonSizer->Add(Rem, 0, wxALL, 3);
	buttonSizer->Add(Add, 0, wxALL, 3);

	sizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxRIGHT, 5);
	sizer->Add(list, 1, wxEXPAND);
	
	SetSizer(sizer);
	Layout();

	Add->Bind(wxEVT_BUTTON, &AttackControl::onAddButton, this);
	Rem->Bind(wxEVT_BUTTON, &AttackControl::onRemButton, this);
}

void AttackControl::onAddButton(wxCommandEvent& event)
{
	AddDialog dialog(this, wxID_ANY, "");

	if (dialog.ShowModal() == wxID_CANCEL)
		return;

	std::string name = dialog.getInput(0);
	std::string mod = dialog.getInput(1);
	std::string dmg = dialog.getInput(2);

	int x = list->GetItemCount();
	list->InsertItem(x, name);
	list->SetItem(x, 1, mod);
	list->SetItem(x, 2, dmg);
}

void AttackControl::onRemButton(wxCommandEvent& event)
{
	long item = -1;
	for (;; )
	{
		item = list->GetNextItem(item,
			wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED);
		if (item == -1)
			break;
		// this item is selected - do whatever is needed with it
		list->DeleteItem(item);
	}
}

AttackControl::AddDialog::AddDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
	wxDialog(parent, id, title, pos, size, style, name)
{
	int gap = 3;
	auto sizer = new wxBoxSizer(wxVERTICAL);
	std::string names[] = {
		"Name",
		"Attack Bonus",
		"Damage Type"
	};

	std::string defVal[] =
	{
		"Dagger",
		"0",
		"1d4 Piercing"
	};

	for (int i = 0; i < 3; ++i)
	{
		auto horSizer = new wxBoxSizer(wxHORIZONTAL);
		auto text = new wxStaticText(this, wxID_ANY, names[i]);
		auto val = input[i] = new wxTextCtrl(this, wxID_ANY, defVal[i]);

		if (names[i] == "Attack Bonus")
			val->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
		
		horSizer->Add(text);
		horSizer->Add(10, -1);
		horSizer->Add(val);
		sizer->Add(horSizer, 0, wxALIGN_RIGHT | wxTOP | wxLEFT | wxRIGHT, gap);
	}

	sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, gap);

	SetSizerAndFit(sizer);
}

std::string AttackControl::AddDialog::getInput(int index)
{
	switch (index)
	{
	case 0:
	case 1:
	case 2:
		return input[index]->GetValue().ToStdString();
	default:
		return "";
	}
}

AttackControl::RemDialog::RemDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
	wxDialog(parent, id, title, pos, size, style, name)
{
}
