#pragma once

class AttackControl : public wxPanel
{
	class AddDialog : public wxDialog
	{
		wxTextCtrl* input[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
		wxChoice* diceType = nullptr;
	public:
		AddDialog(wxWindow* parent,
			wxWindowID 	id,
			const wxString& title,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long 	style = wxDEFAULT_DIALOG_STYLE,
			const wxString& name = wxDialogNameStr
		);

		std::string getInput(int index);
		std::string getName()		{ return input[0]->GetValue().ToStdString(); }
		int getMod()				{ return std::stoi(input[1]->GetValue().ToStdString()); }
		int getDmgDice()			{ return std::stoi(input[2]->GetValue().ToStdString()); }
		std::string getDieType()	{ return diceType->GetString(diceType->GetSelection()).ToStdString(); };
		int getDamageMod()			{ return std::stoi(input[3]->GetValue().ToStdString()); }
		std::string getDmgType()	{ return input[4]->GetValue().ToStdString(); }

	};

	wxButton* Add = nullptr;
	wxButton* Rem = nullptr;
	wxListView* list = nullptr;

public:
	AttackControl(wxWindow* parent,
		wxWindowID 	id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxTAB_TRAVERSAL,
		const wxString& name = wxPanelNameStr
	);

	struct ListItem
	{
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);

		std::string name;
		std::string to_hit;
		std::string dice;
		std::string dmg_mod;
		std::string damageType;
	};

	wxListView* getList()		{ return list; }
	wxButton* getAddButton()	{ return Add; }
	wxButton* getRemButton()	{ return Rem; }

	void resizeList();

	void onAddButton(wxCommandEvent& event);
	void onRemButton(wxCommandEvent& event);
	void onListDClick(wxListEvent& event);

	void addEntry(std::string name, std::string mod, std::string dice, std::string damageType);
	void addEntry(ListItem entry);
	ListItem getEntry(int x);
};

template<class Archive>
inline void AttackControl::ListItem::serialize(Archive& ar, const unsigned int version)
{
	ar& name;
	ar& to_hit;
	ar& dice;
	ar& dmg_mod;
	ar& damageType;
}
