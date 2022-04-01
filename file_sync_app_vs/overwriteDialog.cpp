/////////////////////////////////////////////////////////////////////////////
// Name:        File Sync
// Purpose:     An application that Syncs Files
// Developed:   15.01.2022 @HTW Berlin under the Coordination of Tilo Wendler
/////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------
#include "overwriteDialog.h"
#include "id.h"
#include <wx/valgen.h>
#include <wx/valtext.h>
#include <ctime>
#include <iostream>
// ----------------------------------------------------------------------------
// Dialog window
// ----------------------------------------------------------------------------
OverwriteDialog::OverwriteDialog(wxWindow* parent, wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name) :
    wxDialog(parent, id, title, pos, size, style, name) {
    //Create main sizer with a horizontal sizer to store the components
    mainSizer = new wxBoxSizer(wxVERTICAL);
    overwriteSizer = new wxBoxSizer(wxHORIZONTAL);
    //Creates a label
    wxStaticText* questionLable = new wxStaticText(this, wxID_ANY, _("Nothing"));
    questionLable->SetMinSize(wxSize(50, questionLable->GetMinSize().y));
    overwriteSizer->Add(questionLable, 0, wxALL, 5);
    // ------------------------------------------------------------------------
    // 
    // ------------------------------------------------------------------------
    //Button to select file that will be ID
    overwriteSizer = new wxButton(this, window::id::ID_IDADD, wxT("Jump to file..."));
    Connect(window::id::ID_IDADD, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(OverwriteDialog::OnSelectFile));
    overwriteSizer->Add(addFileBtn, 0, wxTOP, 5);
    // ------------------------------------------------------------------------
    // Master/Client checkboxes
    // ------------------------------------------------------------------------
    //Create checkboxes to select if the file is Master or CLient
    //Creats a vertical box panel to put the checkboxes
    vbox = new wxBoxSizer(wxVERTICAL);
    masterCheckBox = new wxCheckBox(this, window::id::ID_MASTERCHBOX,
        wxT("Master"), wxPoint(5, 20));
    clientCheckBox = new wxCheckBox(this, window::id::ID_CLIENTCHBOX,
        wxT("Client"), wxPoint(5, 20));
    masterCheckBox->SetValue(true);
    clientCheckBox->SetValue(false);
    //assigns actions to those buttons
    Connect(window::id::ID_MASTERCHBOX, wxEVT_COMMAND_CHECKBOX_CLICKED,
        wxCommandEventHandler(OverwriteDialog::OnToggle));
    Connect(window::id::ID_CLIENTCHBOX, wxEVT_COMMAND_CHECKBOX_CLICKED,
        wxCommandEventHandler(OverwriteDialog::OnToggle));
    // adds each button to the panel
    vbox->Add(-1, 5);
    vbox->Add(masterCheckBox);
    vbox->Add(clientCheckBox, 0, wxTOP, 5);
    // adds the button panel to the source sizer
    idFileSizer->Add(vbox);
    // ------------------------------------------------------------------------
    // Create ID button
    // ------------------------------------------------------------------------
    // Add button to create ID
    createIDbtn = new wxButton(this, window::id::ID_IDFILE, _("Create ID"));
    Connect(window::id::ID_IDFILE, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(IdDialog::OnIdFile));
    idFileSizer->Add(createIDbtn, 0, wxALL, 5);
    mainSizer->Add(idFileSizer, 0, wxEXPAND | wxALL, 5);
    SetSizer(mainSizer);
    // ------------------------------------------------------------------------
    // Default size
    // ------------------------------------------------------------------------
    SetMinSize(wxSize(1020, 100)); //min size of the dialog
    Fit();
}
//
void IdDialog::OnSelectFile(wxCommandEvent& event)
{
 
}
//
void IdDialog::OnIdFile(wxCommandEvent& event)
{

}
//Small function to select if the files is master or client
void IdDialog::OnToggle(wxCommandEvent& WXUNUSED(event))
{

}
IdDialog::~IdDialog() {}