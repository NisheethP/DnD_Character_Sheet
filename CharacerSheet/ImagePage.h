#pragma once

#include <wx/wx.h>
#include <wx/stream.h>
#include <wx/srchctrl.h>
#include <wx/image.h>
#include <vector>

class ImagePage : public wxPanel
{
	std::string FolderName = "";

	std::vector<std::pair<std::string, wxImage*>> images;
	int curImageIndex = 0;
	wxStaticBitmap* curImage;
	
	wxListBox* List = nullptr;
	wxSearchCtrl* Search = nullptr;
	wxStaticText* Title = nullptr;
	wxButton* Add = nullptr;
	wxButton* Rem = nullptr;

	double imgWidth = 600;
public:
	ImagePage(wxWindow* parent,
		wxWindowID 	id,
		std::string pFolderName,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxTAB_TRAVERSAL,
		const wxString& name = wxPanelNameStr
	);

	bool takeImage();
	bool loadExistingImages();
	void updateList();
	void setCurImage(int index = -1);

	void onAddButton(wxCommandEvent& event);
	void onRemButton(wxCommandEvent& event);
	void onSearchType(wxCommandEvent& event);
	void onSearchEnter(wxCommandEvent& event);
	void onListSelect(wxCommandEvent& event);
};

