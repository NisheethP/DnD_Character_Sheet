#include "DiceRollerDialog.h"

#include <wx/artprov.h>
#include <wx/scrolwin.h>
#include <wx/image.h>
#include <wx/valgen.h>
#include <random>
#include <chrono>

DiceRollerDialog::DiceRollerDialog(
	wxWindow* parent,
	wxWindowID id,
	const wxString& heading,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) :
	wxDialog(parent, id, heading, pos, size, style, name)
{

	mainSizer = new wxBoxSizer(wxHORIZONTAL);
	diceSizer = new wxBoxSizer(wxVERTICAL);
	auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	
	auto scrollPanel = new wxScrolled<wxPanel>(this);
	scrollPanel->SetScrollRate(0, FromDIP(10));
	scrollPanel->SetSizer(diceSizer);
	int margin = 5;

	Roll = new wxButton(scrollPanel, wxID_ANY, "ROLL", wxDefaultPosition, wxDefaultSize);
	Add = new wxButton(scrollPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

	//Roll->SetBitmap(wxArtProvider().GetBitmap(wxART_DELETE, wxART_BUTTON));
	Add->SetBitmap(wxArtProvider().GetBitmap(wxART_PLUS, wxART_BUTTON));

	buttonSizer->Add(Add);
	buttonSizer->Add(5,-1);
	buttonSizer->Add(Roll);
	//buttonSizer->Add(-1, 10);

	diceSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxBOTTOM | wxRIGHT, 10);

	History = new wxListBox(this, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(200,-1)));
	History->SetFont(History->GetFont().Larger());
	
	wxPanel* p = createDiceLine(scrollPanel);
	diceSizer->Add(p, 0, wxRIGHT, 10);
	diceSizer->Add(-1, 2);

	mainSizer->Add(History, 1, wxEXPAND | wxALL, margin);
	mainSizer->Add(scrollPanel, 0, wxEXPAND | wxALL, margin);

	mainSizer->SetMinSize(this->GetMinSize());
	BindAll();
	this->SetSizer(mainSizer);
	this->Layout();
}

wxPanel* DiceRollerDialog::createDiceLine(wxWindow* parent)
{
	auto panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	const int NUM_DICE = 7;
	
	wxString choices[NUM_DICE] =
	{
		"d4",
		"d6",
		"d8",
		"d10",
		"d12",
		"d20",
		"d100",
	};

	auto horSizer = new wxBoxSizer(wxHORIZONTAL);

	auto dice = new wxSpinCtrl(panel, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_CENTER_HORIZONTAL);
	auto combo = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, NUM_DICE, choices);
	auto plusText = new wxStaticText(panel, wxID_ANY, "+");
	auto mod = new wxSpinCtrl(panel, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_CENTER_HORIZONTAL);
	auto remButton = new wxButton(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	dice->SetMin(1);

	mod->SetMin(-100);
	mod->SetMax(100);
	mod->SetValue(0);

	combo->Select(0);

	remButton->SetBitmap(wxArtProvider().GetBitmap(wxART_DELETE, wxART_BUTTON));

	horSizer->Add(dice);
	horSizer->Add(combo);
	horSizer->Add(3,-1);
	horSizer->Add(plusText);
	horSizer->Add(3,-1);
	horSizer->Add(mod);
	horSizer->Add(10, -1);
	horSizer->Add(remButton);

	auto numDie = new int;
	auto dieType = new wxString;
	auto modifier = new int;
		
	dice->SetValidator(wxGenericValidator(numDie));
	combo->SetValidator(wxGenericValidator(dieType));
	mod->SetValidator(wxGenericValidator(modifier));
	
	TransferDataFromWindow();
	*numDie = 1;
	*modifier = 0;
	TransferDataToWindow();

	DiceLine.push_back({ dice, combo, mod, remButton });
	diceData.push_back({ numDie, dieType, modifier });

	remButton->Bind(wxEVT_BUTTON, &DiceRollerDialog::onRemoveRow, this);

	panel->SetSizer(horSizer);
	panel->Layout();
	return panel;
}

DiceRollerDialog::~DiceRollerDialog()
{
	for (auto& i : diceData)
	{
		delete std::get<0>(i);
		delete std::get<1>(i);
		delete std::get<2>(i);
	}
}

void DiceRollerDialog::onRemoveRow(wxCommandEvent& event)
{
	using std::get;

	auto obj = event.GetEventObject();
	
	auto jt = diceData.begin();
	for (auto it = DiceLine.begin(); it != DiceLine.end(); ++it)
	{
		auto& button = get<3>(*it);
		if (get<3>(*it) == obj)
		{
			auto p = button->GetParent();
			p->DestroyChildren();
			p->Destroy();
			mainSizer->Layout();
			
			DiceLine.erase(it);
			diceData.erase(jt);
			return;
		}

		++jt;
	}
}

void DiceRollerDialog::BindAll()
{
	Add->Bind(wxEVT_BUTTON, &DiceRollerDialog::onAddRow, this);
	Roll->Bind(wxEVT_BUTTON, &DiceRollerDialog::onRoll, this);
}

void DiceRollerDialog::onAddRow(wxCommandEvent& event)
{
	if (DiceLine.size() > 25)
		return;
	wxPanel* p = createDiceLine(Add->GetParent());
	diceSizer->Add(p, 0, wxRIGHT, 10);
	diceSizer->Add(-1, 2);
	mainSizer->Layout();
}

void DiceRollerDialog::onRoll(wxCommandEvent& event)
{
	TransferDataFromWindow();
	using std::get;

	std::string str = "";
	
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	
	int total = 0;

	for (int i = 0; i < DiceLine.size(); ++i)
	{
		int curTotal = 0;
			
		auto num = get<0>(diceData[i]);
		auto type = get<1>(diceData[i]);
		auto mod = get<2>(diceData[i]);
		
		std::string diceType = type->ToStdString();

		int DieMax = 4;

		if (diceType == "d4")
			DieMax = 4;
		if (diceType == "d6")
			DieMax = 6;
		if (diceType == "d8")
			DieMax = 8;
		if (diceType == "d10")
			DieMax = 10;
		if (diceType == "d12")
			DieMax = 12;
		if (diceType == "d20")
			DieMax = 20;
		if (diceType == "d100")
			DieMax = 100;

		auto dice_rand = std::bind(std::uniform_int_distribution<int>(1, DieMax), std::mt19937(seed));

		str += "[";

		for (int j = 0; j < *num; ++j)
		{
			int x = dice_rand();
			str += std::to_string(x);
			if (j+1 != *num)
				str += ", ";
			curTotal += x;
		}
		str += "]";
		if (*mod != 0)
		{
			str += " + ";
			str += std::to_string(*mod);
		}

		if (*num != 1 && DiceLine.size() != 1)
		{
			str += " : ";
			str += std::to_string(curTotal + *mod);
		}
		
		total += curTotal + *mod;

		if (i + 1 != DiceLine.size())
			str += "; ";
	}
	str += " = " + std::to_string(total);
	History->Insert(str, 0);
	if (History->GetCount() == maxHistory+1)
		History->Delete(maxHistory);
}
