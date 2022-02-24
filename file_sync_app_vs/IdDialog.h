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
    void OnIdFile(wxCommandEvent& event);
    void OnSelectFile(wxCommandEvent& event);
    void OnToggle(wxCommandEvent& event);

    wxTextCtrl* f_textCtrl;
    wxTextCtrl* fid_textCtrl;

    wxButton* d_addb;
    wxButton* d_idb;
    wxCheckBox* m_cb;
    wxCheckBox* m_cb2;

    const int FileID = 20;
    const int AddID = 21;
    const int ID_CHECKBOX = 22;
    const int ID_CHECKBOX2 = 23;
};