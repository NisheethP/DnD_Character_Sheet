#pragma once

namespace WizardPages
{

	class ClassLevelStatSelectionPage : public wxWizardPageSimple
	{
		wxChoice* charClass = nullptr;
		wxChoice* classType = nullptr;
		wxSpinCtrl* charLevel = nullptr;

		wxSpinCtrl* StatInput[6];

	public:
		ClassLevelStatSelectionPage(wxWizard* parent,
			wxWizardPage* prev = NULL,
			wxWizardPage* next = NULL,
			const wxBitmap& bitmap = wxNullBitmap);

		std::string getCharClass() { return charClass->GetStringSelection().ToStdString(); }
		std::string getClassType() { return classType->GetStringSelection().ToStdString(); }
		int getCharLevel() { return charLevel->GetValue(); }

		int getStat(int index);
	};

	class HP_SpeedSelectionPage : public wxWizardPageSimple
	{
		int level = 1;
		wxChoice* HP_Mode = nullptr;
		std::vector<wxSpinCtrl*> HP_Spin;
		std::vector<wxStaticText*> levelHP_Text;
		wxSpinCtrl* Speed_Spin = nullptr;
		wxFlexGridSizer* HP_Sizer = nullptr;
	public:
		HP_SpeedSelectionPage(wxWizard* parent,
			int p_level = 1,
			wxWizardPage* prev = NULL,
			wxWizardPage* next = NULL,
			const wxBitmap& bitmap = wxNullBitmap);

		void arrange();
		void onChoiceChange(wxCommandEvent& event);
		void setLevel(int p_level) { level = p_level; }
		void Create();
		void setHP_Mode(int x) { HP_Mode->SetSelection(x); }
	};
}

class NewCharacterWizard : public wxWizard
{
	WizardPages::ClassLevelStatSelectionPage* page1 = nullptr;
	//WizardPages::StatSelectionPage* page2 = nullptr;
	WizardPages::HP_SpeedSelectionPage* page2 = nullptr;
	wxWizardPageSimple* page4 = nullptr;

public:
	NewCharacterWizard(
		wxWindow* parent,
		int id = wxID_ANY,
		const wxString& title = wxEmptyString,
		const wxBitmap& bitmap = wxNullBitmap,
		const wxPoint& pos = wxDefaultPosition,
		long style = wxDEFAULT_DIALOG_STYLE
	);

	wxWizardPage* GetFirstPage() const { return page1; }

	void onPageChanging(wxWizardEvent& event);

};