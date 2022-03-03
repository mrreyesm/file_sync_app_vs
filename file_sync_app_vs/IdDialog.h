/////////////////////////////////////////////////////////////////////////////
// Name:        File Sync
// Purpose:     An application that Syncs Files
// Developed:   15.01.2022 @HTW Berlin under the Coordination of Tilo Wendler
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/textctrl.h>
class IdDialog : public wxDialog
{
public:
    //constructor
    IdDialog(wxWindow* parent, wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE,
        const wxString& name = wxASCII_STR(wxDialogNameStr));
    //destructor
    virtual ~IdDialog();
private:
    //Event handlers/Functions
    void OnIdFile(wxCommandEvent& event);
    void OnSelectFile(wxCommandEvent& event);
    void OnToggle(wxCommandEvent& event);
    //Declaration of objects
    wxBoxSizer* mainSizer;
    wxBoxSizer* idFileSizer;
    wxStaticText* fileLable;
    wxTextCtrl* filetxtCtrl;
    wxTextCtrl* idFiletxtCtrl;
    wxButton* addFileBtn;
    wxBoxSizer* vbox;
    wxCheckBox* masterCheckBox;
    wxCheckBox* clientCheckBox;
    wxButton* createIDbtn;
};