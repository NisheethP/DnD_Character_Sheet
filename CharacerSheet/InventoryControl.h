#pragma once

#include "Item.h"

class InventoryControl :
    public wxPanel
{
	wxButton* Add = nullptr;
    wxButton* Rem = nullptr;
    wxListView* list = nullptr;
	wxTextCtrl* totalWeight = nullptr;
	wxStaticText* title = nullptr;
	wxTextCtrl* weight = nullptr;

	wxSearchCtrl* search = nullptr;
	wxChoice* searchType = nullptr;

	std::vector<std::string> allTags;
	std::vector<Item> items;
	std::vector<int> displayEntries;

	class AddDialog : public wxDialog
	{
		wxTextCtrl* itemName  = nullptr;
		wxTextCtrl* description = nullptr;
		wxSpinCtrlDouble* weight = nullptr;
		wxSpinCtrl* count = nullptr;
		wxCheckBox* attunement = nullptr;
		wxEditableListBox* tags = nullptr;

	public:
		AddDialog(wxWindow* parent,
			wxWindowID 	id,
			const wxString& title,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long 	style = wxDEFAULT_DIALOG_STYLE,
			const wxString& name = wxDialogNameStr
		);

		std::string getName() { return itemName->GetValue().ToStdString(); }
		std::string getDescription() { return description->GetValue().ToStdString(); }
		bool getAttunement() { return attunement->GetValue(); }
		float getWeight() { return weight->GetValue(); }
		int getCount() { return count->GetValue(); }

		wxEditableListBox* getTags() { return tags; }

		void openDialog(std::string pItemName, std::string pItemDesc, float pWeight, int pCount, bool pAttunement, std::vector<std::string>* pTags);
		void setDialog(std::string pItemName, std::string pItemDesc, float pWeight, int pCount, bool pAttunement, std::vector<std::string>* pTags);
	};

public:
	InventoryControl(wxWindow* parent,
		wxWindowID 	id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxTAB_TRAVERSAL,
		const wxString& name = wxPanelNameStr
	);


	void onAddButton(wxCommandEvent& event);
	void onRemButton(wxCommandEvent& event);
	void onListDClick(wxListEvent& event);
	void onSearchType(wxCommandEvent& event);

	void updateItem(AddDialog& dialog, int curItem = -1);
	void fillList();

	bool isDisplayed(int index);
	void addToDisplayedList(int x);

	bool isSearchable(std::string entry);

	void resizeCtrl();

	bool hasTag(std::string pTag);
	void updateAllTags();

	wxListView* getList() { return list; }

	std::vector<std::string> getAllTags() const { return allTags; }
};

