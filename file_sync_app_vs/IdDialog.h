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
#include <wx/filepicker.h>

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
    void OnPathChanged(wxFileDirPickerEvent& event);

    wxTextCtrl* m_textCtrl;
    wxFilePickerCtrl* filePickerCtrl;
    const int FilePickerID = 20;

};