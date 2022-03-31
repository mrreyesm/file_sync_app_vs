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
class OverwriteDialog : public wxDialog
{
public:
    //constructor
    OverwriteDialog(wxWindow* parent, wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE,
        const wxString& name = wxASCII_STR(wxDialogNameStr));
    //destructor
    virtual ~OverwriteDialog();
private:
    //Event handlers/Functions
    void OnIdFile(wxCommandEvent& event);
    //Declaration of objects
    wxBoxSizer* mainSizer;
    wxBoxSizer* overwriteSizer;
    wxStaticText* questionLable;
    wxButton* gotobtn;
    wxButton* yesBtn;
    wxButton* nobtn;
};
