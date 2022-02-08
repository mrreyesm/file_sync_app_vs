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

    void OnNew(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);

    void OnNew2(wxCommandEvent& event);
    void OnClear2(wxCommandEvent& event);
    void OnDelete2(wxCommandEvent& event);

    wxListBox* m_lb;
    wxListBox* m_lb2;
    wxListBox* listbox;
    wxListBox* listbox2;
    wxPanel* m_parent;

    wxButton* m_newb;
    wxButton* m_clearb;
    wxButton* m_deleteb;

    wxButton* m_newb2;
    wxButton* m_clearb2;
    wxButton* m_deleteb2;

    //Data Members
    const int ID_LISTBOX  = 5;
    const int ID_LISTBOX2 = 6;
    const int ID_LISTBOX3 = 7;
    const int ID_LISTBOX4 = 8;
    const int ID_LISTBOX5 = 9;
    const int DirPickerID = 10;

};