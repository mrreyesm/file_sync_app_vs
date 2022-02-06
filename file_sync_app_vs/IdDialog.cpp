/////////////////////////////////////////////////////////////////////////////
// Name:        File Sync
// Purpose:     An application that Syncs Files
// Developed:   15.01.2022 @HTW Berlin under the Coordination of Tilo Wendler
/////////////////////////////////////////////////////////////////////////////
#include "IdDialog.h"
#include <wx/valgen.h>
#include <wx/valtext.h>

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
    idFileSizer->Add(idFileLable);
    // Create box for the File that is going to be modified
    idFileBox = new wxTextCtrl(this, wxID_ANY);
    wxTextValidator idFileValidator(wxFILTER_ALPHANUMERIC);
    idFileBox->SetValidator(idFileValidator);
    idFileSizer->Add(idFileBox, 1);
    // Add button to select file
    wxButton* addButton = new wxButton(this, wxID_OK, _("Add"));
    idFileSizer->Add(addButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    // Add button to create ID
    wxButton* idButton = new wxButton(this, wxID_OK, _("Create ID"));
    idFileSizer->Add(idButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    mainSizer->Add(idFileSizer, 0, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);
    SetMinSize(wxSize(400, 100)); //min size of the dialog
    Fit();
}

IdDialog::~IdDialog() {}