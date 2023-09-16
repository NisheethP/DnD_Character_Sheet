#include "PreCompiledHeader.h"

#include "ImagePage.h"
#include"Util.h"

using std::get;

ImagePage::ImagePage(wxWindow* parent,
	wxWindowID 	id,
	std::string pFolderName,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name
):
	wxPanel(parent, id, pos, size, style, name),
	FolderName(pFolderName)
{
	if (!wxDirExists(FolderName))
		wxMkDir(FolderName);

	auto mainSizer = new wxBoxSizer(wxHORIZONTAL);

	auto imgPanel = new wxScrolled<wxPanel>(this);
	imgPanel->SetScrollRate(FromDIP(10), FromDIP(10));

	loadExistingImages();
	
	wxSize bSize(40, -1);
	Add = new wxButton(this, wxID_ANY, "", wxDefaultPosition, bSize);
	Rem = new wxButton(this, wxID_ANY, "", wxDefaultPosition, bSize);

	Add->SetBitmap(wxArtProvider().GetBitmap(wxART_PLUS, wxART_BUTTON));
	Rem->SetBitmap(wxArtProvider().GetBitmap(wxART_MINUS, wxART_BUTTON));

	Search = new wxSearchCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	Search->SetDescriptiveText("File Name");
	Search->ShowCancelButton(true);
	Search->ShowSearchButton(true);


	this->List = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE);
	
	updateList();

	if (images.size() != 0)
		List->SetSelection(0);

	auto searchSizer = new wxBoxSizer(wxHORIZONTAL);
	auto rightSizer = new wxBoxSizer(wxVERTICAL);
	auto leftSizer = new wxBoxSizer(wxVERTICAL);

	Title = new wxStaticText(imgPanel, wxID_ANY, "No Images");
	Title->SetFont(wxFontInfo(18).Bold().Underlined());
	Title->SetForegroundColour(*wxWHITE);

	curImage = new wxStaticBitmap(imgPanel, wxID_ANY, wxArtProvider().GetIcon(wxART_MISSING_IMAGE));
	
	setCurImage(-1);
	if (images.size() != 0)
		setCurImage(0);

	//curImage->SetMaxSize(FromDIP(wxSize(200,400)));

	leftSizer->Add(Title, 0, wxALIGN_CENTER);
	leftSizer->Add(-1,10);
	leftSizer->Add(curImage, 1, wxALIGN_CENTER);

	imgPanel->SetSizer(leftSizer);

	searchSizer->Add(Search);
	mainSizer->Add(2, -1);
	searchSizer->Add(Rem);
	mainSizer->Add(2, -1);
	searchSizer->Add(Add);

	rightSizer->Add(searchSizer, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);
	rightSizer->Add(List, 1, wxEXPAND);

	mainSizer->Add(imgPanel, 1, wxEXPAND);
	mainSizer->Add(10, -1);
	mainSizer->Add(rightSizer, 0, wxEXPAND);

	Rem->Destroy();
	this->SetSizer(mainSizer);
	this->Layout();

	Add->Bind(wxEVT_BUTTON, &ImagePage::onAddButton, this);
	//Rem->Bind(wxEVT_BUTTON, &ImagePage::onRemButton, this);
	Search->Bind(wxEVT_TEXT, &ImagePage::onSearchType, this);
	Search->Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &ImagePage::onSearchEnter, this);
	List->Bind(wxEVT_LISTBOX, &ImagePage::onListSelect, this);
}

bool ImagePage::takeImage()
{
	wxFileDialog openFileDialog(this, "Load Image", FolderName, "", "PNG Files (*.png)|*.png|JPEG Files (*.jpeg)|*.jpeg", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return false;

	wxFileInputStream input_stream(openFileDialog.GetPath());

	wxImage* image = new wxImage();
	image->AddHandler(new wxPNGHandler());
	image->AddHandler(new wxJPEGHandler());
		
	if (!input_stream.IsOk())
	{
		wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
		return false;
	}

	image->LoadFile(input_stream);

	auto str = openFileDialog.GetFilename();
	wxFileOutputStream output_stream(FolderName + "\\" + str.ToStdString());

	image->SaveFile(output_stream, wxBITMAP_TYPE_PNG);
	
	return true;
}

bool ImagePage::loadExistingImages()
{
	images.clear();
	wxDir dir(FolderName);

	wxArrayString allFiles;

	dir.GetAllFiles(FolderName, &allFiles, "*.png", wxDIR_FILES);
	
	for (auto it = allFiles.begin(); it != allFiles.end(); ++it)
	{
		auto title = it->ToStdString();
		wxImage image;
		
		wxFileInputStream input_stream(title);

		wxPNGHandler().LoadFile(&image, input_stream);

		int len = FolderName.size() + 1;
		title = title.substr(len);
		title = title.substr(0, title.size() - 4);
		images.push_back({images.size(), title, image });
	}

	return true;
}

void ImagePage::updateList()
{
	List->Clear();

	for (auto it = images.begin(); it != images.end(); ++it)
	{
		auto str = get<1>(*it);
		str = str.substr(0,str.size());
		List->Append(str);
	}
}

void ImagePage::setCurImage(int index)
{
	if (index == -1)
	{
		curImage->SetBitmap(wxArtProvider().GetIcon(wxART_MISSING_IMAGE));
		return;
	}
	
	if (index >= images.size())
		return;
	
	wxImage img(get<2>(images[index]));
	double w = img.GetWidth();
	double h = img.GetHeight();
	double newHeight = h / w;
	newHeight *= imgWidth;

	img.Rescale(imgWidth, newHeight, wxIMAGE_QUALITY_BILINEAR);
	curImage->SetBitmap(img);
}

void ImagePage::onAddButton(wxCommandEvent& event)
{
	takeImage();
	loadExistingImages();
}

void ImagePage::onRemButton(wxCommandEvent& event)
{
	wxDir dir(FolderName);
	
	wxFileDialog openFileDialog(this, "Select Image to remove", FolderName + "\\", "", "PNG Files (*.png)|*.png", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	auto str = openFileDialog.GetFilename();
	wxRemoveFile(openFileDialog.GetPath());

	for (auto it = images.begin(); it != images.end(); ++it)
	{
		if (get<1>(*it) == str)
		{
			images.erase(it);
			return;
		}
	}

	updateList();
}

void ImagePage::onSearchType(wxCommandEvent& event)
{
	auto str = Search->GetValue().ToStdString();
	
	if (str == "")
	{
		updateList();
		return;
	}
	
	Util::toLowerString(str);
	List->Clear();
	
	for (auto it = images.begin(); it != images.end(); ++it)
	{
		auto title = get<1>(*it);
		Util::toLowerString(title);
		if (title.find(str) != std::string::npos)
			List->Append(get<1>(*it));
	}
}

void ImagePage::onSearchEnter(wxCommandEvent& event)
{
	/*auto str = Search->GetValue().ToStdString();
	
	if (str == "")
	{
		List->SetSelection(0);
		return;
	}
		
	if (List->GetCount() == 1)
	{
		List->Select(0);
		return;
	}

	Util::toLowerString(str);
	
	int x = List->FindString(str);
	List->Select(x);*/
}

void ImagePage::onListSelect(wxCommandEvent& event)
{
	auto str = List->GetString(List->GetSelection()).ToStdString();

	for (int it = 0; it < images.size(); ++it)
	{
		auto title = get<1>(images[it]);
		Util::toLowerString(title);
		Util::toLowerString(str);
		if (title == str)
		{
			setCurImage(it);
			Title->SetLabel(get<1>(images[it]));
		}
	}
}
