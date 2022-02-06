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

class IdDialog;

class MainWindow : public wxFrame
{
public:
    // Constructor
    MainWindow(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));
    // Deconstructor           
    ~MainWindow();
private:
    IdDialog* idDialog = nullptr;
    // Event handlers
    void onIdFile(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    wxTextCtrl* sourceBox = nullptr;
    wxTextCtrl* targetBox = nullptr;
    wxTextCtrl* sourceFileBox = nullptr;
    wxTextCtrl* sourceMFileBox = nullptr;
    wxTextCtrl* targetFileBox = nullptr;
    wxTextCtrl* targetCFileBox = nullptr;
    //Data Members

};