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
    // Event handlers
    void onIdFile(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnNew(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnSearch(wxCommandEvent& event);

    void OnNew2(wxCommandEvent& event);
    void OnClear2(wxCommandEvent& event);
    void OnDelete2(wxCommandEvent& event);
    void OnSearch2(wxCommandEvent& event);

    void OnSync(wxCommandEvent& event);

    wxListBox* m_lb;
    wxListBox* m_lb2;
    wxListBox* listbox;
    wxListBox* listbox2;

    wxListBox* sourceMFileBox;
    wxListBox* m_lb3;

    wxListBox* targetCFileBox;
    wxListBox* m_lb4;

    wxListBox* sourceMDFileBox;
    wxListBox* m_lb5;

    wxListBox* sourceCFileBox;
    wxListBox* m_lb6;

    wxListBox* targetMFileBox;
    wxListBox* m_lb7;

    wxListBox* targetUFileBox;
    wxListBox* m_lb8;

    wxPanel* m_parent;

    wxButton* m_newb;
    wxButton* m_clearb;
    wxButton* m_deleteb;
    wxButton* m_searchb;

    wxButton* m_newb2;
    wxButton* m_clearb2;
    wxButton* m_deleteb2;
    wxButton* m_searchb2;

    wxButton* m_syncb;

    //Data Members
    const int ID_LISTBOX  = 5;
    const int ID_LISTBOX2 = 6;
    const int ID_LISTBOX3 = 7;
    const int ID_LISTBOX4 = 8;
    const int ID_LISTBOX5 = 9;
    const int ID_LISTBOX6 = 91;
    const int ID_LISTBOX7 = 92;
    const int ID_LISTBOX8 = 93;
    const int DirPickerID = 10;
    const int ID_SEARCH = 11;
    const int ID_SEARCH2 = 12;
    const int ID_SYNC = 13;
};