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
    wxStaticText* questionLable = new wxStaticText(this, wxID_ANY, _(name + "\nHas been modified more recently than the master file.\nWould you like to overwrite it?"));
    questionLable->SetMinSize(wxSize(questionLable->GetMinSize().x, 50));
    overwriteSizer->Add(questionLable, 0, wxALL, 5);
    mainSizer->Add(overwriteSizer, 0, wxEXPAND | wxALL, 5);
    // ------------------------------------------------------------------------
    // Create buttons
    // ------------------------------------------------------------------------
    overwriteBtnsSizer = new wxBoxSizer(wxHORIZONTAL);
    //Button to select file that will be ID
    gotoBtn = new wxButton(this, window::id::ID_GOTO, wxT("Jump to file..."));
    Connect(window::id::ID_GOTO, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(OverwriteDialog::OnSelectFile));
    overwriteBtnsSizer->Add(gotoBtn, 0, wxTOP, 5);
    //
    yesBtn = new wxButton(this, window::id::ID_YESBTN, _("Yes"));
    Connect(window::id::ID_YESBTN, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(OverwriteDialog::OnYes));
    overwriteBtnsSizer->Add(yesBtn, 0, wxALL, 5);
    //
    noBtn = new wxButton(this, window::id::ID_NOBTN, _("No"));
    Connect(window::id::ID_NOBTN, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(OverwriteDialog::OnNo));
    overwriteBtnsSizer->Add(noBtn, 0, wxALL, 5);
    mainSizer->Add(overwriteBtnsSizer, 0, wxEXPAND | wxALL, 5);
    
    SetSizer(mainSizer);
    // ------------------------------------------------------------------------
    // Default size
    // ------------------------------------------------------------------------
    SetMinSize(wxSize(300, 120)); //min size of the dialog
    Fit();
}
//
void OverwriteDialog::OnSelectFile(wxCommandEvent& event)
{
 
}
//
void OverwriteDialog::OnYes(wxCommandEvent& event)
{

}
//
void OverwriteDialog::OnNo(wxCommandEvent& event)
{
    Close();
}
OverwriteDialog::~OverwriteDialog() {}