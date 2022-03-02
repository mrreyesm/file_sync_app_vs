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
    wxTextCtrl* f_textCtrl;
    wxTextCtrl* fid_textCtrl;
    wxButton* d_addb;
    wxButton* d_idb;
    wxCheckBox* m_cb;
    wxCheckBox* m_cb2;
};