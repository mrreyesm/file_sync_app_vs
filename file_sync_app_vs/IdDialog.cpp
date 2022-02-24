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
int master = 1;
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

    //Create checkboxes to select if the file is Master or CLient
    // creats a vertical box panel to put the checkboxes
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    m_cb = new wxCheckBox(this, ID_CHECKBOX, wxT("Master"),wxPoint(5, 20));
    m_cb2 = new wxCheckBox(this, ID_CHECKBOX2, wxT("Client"), wxPoint(5, 20));
    m_cb->SetValue(true);
    m_cb2->SetValue(false);
    //assigns actions to those buttons
    Connect(ID_CHECKBOX, wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(IdDialog::OnToggle));
    Connect(ID_CHECKBOX2, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(IdDialog::OnToggle));
    // adds each button to the panel
    vbox->Add(-1, 5);
    vbox->Add(m_cb);
    vbox->Add(m_cb2, 0, wxTOP, 5);
    // adds the button panel to the source sizer
    idFileSizer->Add(vbox);

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
    if(fileNameTemp != "")
    {
        int max = 1000;
        int min = 9999;
        int randNum = rand() % (max - min + 1) + min;
        std::string s = std::to_string(randNum);
        std::string filetype;
        if (master == 1) {
            filetype = "_IMC";
        }
        else {
            filetype = "_IDC";
        }
        std::string fileNameTemp2 = fileNameTemp + filetype + s + extension;
        wxString tempName(fileNameTemp2);
        fid_textCtrl->SetValue(wxFileNameFromPath(fileNameTemp2));
        rename(fileName, tempName);
    }
}

void IdDialog::OnToggle(wxCommandEvent& WXUNUSED(event))
{
    if (m_cb->GetValue()) {
        m_cb2->SetValue(false);
        master = 1;
    }
    else if (!m_cb->GetValue()) {
        m_cb2->SetValue(true);
        master = 0;
    }
}

IdDialog::~IdDialog() {}