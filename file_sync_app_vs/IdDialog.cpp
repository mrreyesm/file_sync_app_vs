/////////////////////////////////////////////////////////////////////////////
// Name:        File Sync
// Purpose:     An application that Syncs Files
// Developed:   15.01.2022 @HTW Berlin under the Coordination of Tilo Wendler
/////////////////////////////////////////////////////////////////////////////
#include "IdDialog.h"
#include <wx/valgen.h>
#include <wx/valtext.h>
#include <wx/filepicker.h>

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

    // Create a wxFilePickerCtrl control
    filePickerCtrl = new wxFilePickerCtrl(this,FilePickerID,wxEmptyString,
        wxFileSelectorPromptStr, wxFileSelectorDefaultWildcardStr, wxDefaultPosition, wxSize(500, wxDefaultCoord));
    idFileSizer->Add(filePickerCtrl, 0, wxEXPAND | wxALL, 5);
    //Bind(EVT_FILEPICKER_CHANGED, &IdDialog::OnPathChanged, this, filePickerCtrl->GetId());

    // Create box for the File that is going to be modified
    m_textCtrl = new wxTextCtrl(this, wxID_ANY);
    idFileSizer->Add(m_textCtrl, 1, wxEXPAND | wxALL, 5);

    // Add button to create ID
    wxButton* idButton = new wxButton(this, wxID_CANCEL, _("Create ID"));
    idFileSizer->Add(idButton, 0, wxALL, 5);

    mainSizer->Add(idFileSizer, 0, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);
    SetMinSize(wxSize(1000, 100)); //min size of the dialog
    Fit();
}

void IdDialog::OnPathChanged(wxFileDirPickerEvent& event)
{
    if (m_textCtrl)
    {
        m_textCtrl->SetValue(event.GetPath());
    }
}

IdDialog::~IdDialog() {}