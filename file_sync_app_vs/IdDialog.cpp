/////////////////////////////////////////////////////////////////////////////
// Name:        File Sync
// Purpose:     An application that Syncs Files
// Developed:   15.01.2022 @HTW Berlin under the Coordination of Tilo Wendler
/////////////////////////////////////////////////////////////////////////////
#include "IdDialog.h"
#include <wx/valgen.h>
#include <wx/valtext.h>
wxString fileName;
std::string fileNameTemp;
std::string extension;

IdDialog::IdDialog(wxWindow* parent, wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name) :
    wxDialog(parent, id, title, pos, size, style, name)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    //Create idFile field
    wxBoxSizer* idFileSizer = new wxBoxSizer(wxHORIZONTAL);

    //Create idFile lable
    wxStaticText* idFileLable = new wxStaticText(this, wxID_ANY, _("File"));
    idFileLable->SetMinSize(wxSize(50, idFileLable->GetMinSize().y));
    idFileSizer->Add(idFileLable, 0, wxALL, 5);

    
    f_textCtrl = new wxTextCtrl(this, wxID_ANY);
    idFileSizer->Add(f_textCtrl, 1, wxEXPAND | wxALL, 5);

    // Add button to select file that will be renamed
    d_addb = new wxButton(this, AddID, wxT("Add"));
    Connect(AddID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(IdDialog::OnSelectFile));
    idFileSizer->Add(d_addb, 0, wxTOP, 5);

    // Create box for the File that is going to be modified
    fid_textCtrl = new wxTextCtrl(this, wxID_ANY);
    idFileSizer->Add(fid_textCtrl, 1, wxEXPAND | wxALL, 5);

    // Add button to create ID
    wxButton* d_idb = new wxButton(this, FileID, _("Create ID"));
    Connect(FileID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(IdDialog::OnIdFile));
    idFileSizer->Add(d_idb, 0, wxALL, 5);

    mainSizer->Add(idFileSizer, 0, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);
    SetMinSize(wxSize(1020, 100)); //min size of the dialog
    Fit();
}

void IdDialog::OnSelectFile(wxCommandEvent& event)
{
    wxFileDialog* openFileDialog = new wxFileDialog(this, "Choose File for ID");
    if (openFileDialog->ShowModal() == wxID_OK) {
        // wxString fileName = wxFileNameFromPath(openFileDialog->GetPath());
        fileName = openFileDialog->GetPath();
        f_textCtrl->ChangeValue(fileName);
        std::string s = f_textCtrl->GetValue().ToStdString();
        size_t lastindex = s.find_last_of(".");
        size_t idindex = s.find_last_of("_");
        fileNameTemp = s.substr(0, idindex);
        size_t end = s.size();
        extension = s.substr(lastindex, end);
    }
}

void IdDialog::OnIdFile(wxCommandEvent& event)
{
    //wxMessageBox(fileNameTemp);
    int max = 1000;
    int min = 9999;
    int randNum = rand() % (max - min + 1) + min;
    std::string s = std::to_string(randNum);
    std::string fileNameTemp2 = fileNameTemp + "_IDC" + s + extension;
    wxString tempName(fileNameTemp2);
    fid_textCtrl->SetValue(wxFileNameFromPath(fileNameTemp2));
    rename(fileName, tempName);
}

IdDialog::~IdDialog() {}