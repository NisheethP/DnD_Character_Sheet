#pragma once

namespace WizardPages
{

	class ClassLevelSelectionPage : public wxWizardPageSimple
	{
		wxComboBox* charClass = nullptr;
		wxComboBox* classType = nullptr;
		wxSpinCtrl* charLevel = nullptr;

	public:
		ClassLevelSelectionPage(wxWizard* parent,
			wxWizardPage* prev = NULL,
			wxWizardPage* next = NULL,
			const wxBitmap& bitmap = wxNullBitmap);

		std::string getCharClass() { return charClass->GetValue().ToStdString(); }
		std::string getClassType() { return classType->GetValue().ToStdString(); }
		int getCharLevel() { return charLevel->GetValue(); }
	};

	class StatSelectionPage : public wxWizardPageSimple
	{
		wxSpinCtrl* StatInput[6];
	public:
		StatSelectionPage(wxWizard* parent,
			wxWizardPage* prev = NULL,
			wxWizardPage* next = NULL,
			const wxBitmap& bitmap = wxNullBitmap);

		int getStat(int index);

	};

	class HP_SpeedSelectionPage : public wxWizardPageSimple
	{
		int level = 1;
		wxChoice* HP_Mode = nullptr;
		std::vector<wxSpinCtrl*> HP_Spin;
		std::vector<wxStaticText*> levelHP_Text;
		wxSpinCtrl* Speed_Spin = nullptr;
	public:
		HP_SpeedSelectionPage(wxWizard* parent,
			int p_level = 1,
			wxWizardPage* prev = NULL,
			wxWizardPage* next = NULL,
			const wxBitmap& bitmap = wxNullBitmap);

		void onComboBoxChange(wxCommandEvent& event);
	};
}

class NewCharacterWizard : public wxWizard
{
	WizardPages::ClassLevelSelectionPage* page1 = nullptr;
	WizardPages::StatSelectionPage* page2 = nullptr;
	WizardPages::HP_SpeedSelectionPage* page3 = nullptr;
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
};