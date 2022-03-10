/////////////////////////////////////////////////////////////////////////////
// Name:        File Sync
// Purpose:     An application that Syncs Files
// Developed:   15.01.2022 @HTW Berlin under the Coordination of Tilo Wendler
/////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------
#include "IdDialog.h"
#include "id.h"
#include <wx/valgen.h>
#include <wx/valtext.h>
#include <ctime>
#include <iostream>
#include "md5.h"
#include <algorithm>
// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
//These global variables to store the file, extension and the type of file.
wxString fileName;
std::string fileNameTemp;
std::string extension;
int master = 1;
// ----------------------------------------------------------------------------
// Dialog window
// ----------------------------------------------------------------------------
IdDialog::IdDialog(wxWindow* parent, wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name) :
    wxDialog(parent, id, title, pos, size, style, name){
    //Create main sizer with a horizontal sizer to store the components
    mainSizer = new wxBoxSizer(wxVERTICAL);
    idFileSizer = new wxBoxSizer(wxHORIZONTAL);
    //Creates a label and a textbox to store the file that is going to be ID
    wxStaticText* fileLable = new wxStaticText(this, wxID_ANY, _("File"));
    fileLable->SetMinSize(wxSize(50, fileLable->GetMinSize().y));
    idFileSizer->Add(fileLable, 0, wxALL, 5);
    filetxtCtrl = new wxTextCtrl(this, wxID_ANY);
    idFileSizer->Add(filetxtCtrl, 1, wxEXPAND | wxALL, 5);
    // ------------------------------------------------------------------------
    // Add/Select file button
    // ------------------------------------------------------------------------
    //Button to select file that will be ID
    addFileBtn = new wxButton(this, window::id::ID_IDADD, wxT("Add"));
    Connect(window::id::ID_IDADD, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(IdDialog::OnSelectFile));
    idFileSizer->Add(addFileBtn, 0, wxTOP, 5);
    // Create a text box to see the ID file
    idFiletxtCtrl = new wxTextCtrl(this, wxID_ANY);
    idFileSizer->Add(idFiletxtCtrl, 1, wxEXPAND | wxALL, 5);
    // ------------------------------------------------------------------------
    // Master/Client checkboxes
    // ------------------------------------------------------------------------
    //Create checkboxes to select if the file is Master or CLient
    //Creats a vertical box panel to put the checkboxes
    vbox = new wxBoxSizer(wxVERTICAL);
    masterCheckBox = new wxCheckBox(this, window::id::ID_MASTERCHBOX,
        wxT("Master"),wxPoint(5, 20));
    clientCheckBox = new wxCheckBox(this, window::id::ID_CLIENTCHBOX,
        wxT("Client"), wxPoint(5, 20));
    masterCheckBox->SetValue(true);
    clientCheckBox->SetValue(false);
    //assigns actions to those buttons
    Connect(window::id::ID_MASTERCHBOX, wxEVT_COMMAND_CHECKBOX_CLICKED,
        wxCommandEventHandler(IdDialog::OnToggle));
    Connect(window::id::ID_CLIENTCHBOX, wxEVT_COMMAND_CHECKBOX_CLICKED,
        wxCommandEventHandler(IdDialog::OnToggle));
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
//this function iterates trough the string and capitalizes lower case chars
std::string capitalizeString(std::string s) {
    std::string v;
    std::string x;
    for (int i = 0; i < s.size(); i++) {
        if (isalpha(s[i])) {
            if (isupper(s[i])) {
                v = v + s[i];
            }
            else {
                x = toupper(s[i]);
                v = v + x;
            }
        }
        else v = v + s[i];
    }
    return v;
}
//Opens a dialog window to select a file and gets its name, extension and ID
void IdDialog::OnSelectFile(wxCommandEvent& event)
{
    //Opens a files dialog to select a file
    wxFileDialog* openFileDialog = new wxFileDialog(this, "Choose File for ID");
    if (openFileDialog->ShowModal() == wxID_OK){
        //Once you select a file it gets the path and all its components
        fileName = openFileDialog->GetPath();
        filetxtCtrl->ChangeValue(fileName);
        std::string s = filetxtCtrl->GetValue().ToStdString();
        size_t lastindex = s.find_last_of(".");
        size_t idindex = s.find_last_of("_");
        fileNameTemp = s.substr(0, idindex);
        size_t end = s.size();
        extension = s.substr(lastindex, end);
    }
}
//Creates the ID of the file and renames the file
//It IDs the File based on the date and time, including seconds
//and a hash function so that it is impossible to have 2 same IDs
void IdDialog::OnIdFile(wxCommandEvent& event)
{
    if(fileNameTemp != ""){
        //gets localtime and separates it into different variables
        std::time_t t = std::time(0);   // get time now
        std::tm* now = std::localtime(&t);
        std::string year = std::to_string(now->tm_year+1900);
        std::string month = std::to_string(now->tm_mon + 1);
        std::string day = std::to_string(now->tm_mday);
        std::string hours = std::to_string(now->tm_hour);
        std::string minutes = std::to_string(now->tm_min);
        std::string seconds = std::to_string(now->tm_sec);
        //hash id
        //MD5 needs a string and to make unique IDs the datetime as string is given
        std::string datetime = year + month + day + hours + minutes + seconds;
        std::string md5hash = md5(datetime);
        std::string md5hash7 = md5hash.substr(0, 7);
        std::string md5hash7u = capitalizeString(md5hash7);
        //sets filetype based on the checkboxes
        std::string filetype;
        if (master == 1) {
            filetype = "_IDM-";
        }
        else {
            filetype = "_IDC-";
        }
        //Concatenates everything and renames the file.
        //std::string finalFileName = fileNameTemp + filetype + date + '-' + time + extension;
        std::string finalFileName = fileNameTemp + filetype + md5hash7u + extension;
        wxString tempName(finalFileName);
        idFiletxtCtrl->SetValue(wxFileNameFromPath(finalFileName));
        wxRenameFile(fileName, tempName);
        md5hash7.clear();
        wxMessageBox("The file has been renamed with a unique ID.",
            "ID File", wxOK | wxICON_INFORMATION);
        int dialog_return_value = wxID_NO;
        wxMessageDialog* dial = new wxMessageDialog(NULL,
            _("Would you like to ID another file?"),
            _("ID File"), wxYES_NO | wxICON_QUESTION);
        dialog_return_value = dial->ShowModal();
        switch (dialog_return_value) // Use switch, scales to more buttons later
        {
        case wxID_YES:
            filetxtCtrl->Clear();
            idFiletxtCtrl->Clear();
            break;
        case wxID_NO:
            Close();
            break;
        default:;
        };
    }
    
}
//Small function to select if the files is master or client
void IdDialog::OnToggle(wxCommandEvent& WXUNUSED(event))
{
    //this function only lets you select either master or client type
    //and it always has to have one selected
    if (masterCheckBox->GetValue()) {
        clientCheckBox->SetValue(false);
        master = 1;
    }
    else if (!masterCheckBox->GetValue()) {
        clientCheckBox->SetValue(true);
        master = 0;
    }
}
IdDialog::~IdDialog() {}