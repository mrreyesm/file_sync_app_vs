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
    //Constructor
    MainWindow(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));
    //Deconstructor           
    ~MainWindow();
private:
    //Event handlers/Functions
    void onIdFile(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnAddSourceDir(wxCommandEvent& event);
    void OnDeleteSourceDir(wxCommandEvent& event);
    void OnClearSourceDirs(wxCommandEvent& event);
    void OnSearchSourceDirs(wxCommandEvent& event);
    void OnAddTargetDir(wxCommandEvent& event);
    void OnDeleteTargetDir(wxCommandEvent& event);
    void OnClearTargetDirs(wxCommandEvent& event);
    void OnSearchTargetDirs(wxCommandEvent& event);
    void OnSync(wxCommandEvent& event);
    void onUpdateSyncButton(wxUpdateUIEvent& event);
    void onUpdateSearchSourceButton(wxUpdateUIEvent& event);
    void onUpdateTargetButton(wxUpdateUIEvent& event);
    void OnSourceListBoxDirDClick(wxCommandEvent& event);
    void OnTargetListBoxDirDClick(wxCommandEvent& event);
    void OnMasterListBoxFileDClick(wxCommandEvent& event);
    void OnClientListBoxFileDClick(wxCommandEvent& event);
    void OnSourceClientListBoxFileDClick(wxCommandEvent& event);
    void OnTargetMasterListBoxFileDClick(wxCommandEvent& event);
    void OnMFDuplicatesListBoxFileDClick(wxCommandEvent& event);
    void OnSyncListBoxFileDClick(wxCommandEvent& event);
    //Declaration of objects
    //---------MENU-------
    wxMenuBar* menuBar;
    wxMenu* menuHelp;
    wxMenu* fileMenu;
    wxMenuItem* idFileItem;
    wxMenuItem* quitItem;
    //---------LAYOUT------
    wxPanel* parentPanel;
    wxPanel* leftPanel;
    wxPanel* rightPanel;
    wxBoxSizer* leftPanelSizer;
    wxBoxSizer* rightPanelSizer;
    //---------SOURCE-------
    wxBoxSizer* sourceSizer;
    wxStaticText* sourceLabel;
    wxListBox* sourceDirlistbox;
    wxListBox* sdir_lbx;
    wxBoxSizer* sourcebtnsVbox;
    wxButton* s_addb;
    wxButton* s_deleteb;
    wxButton* s_clearb;
    wxButton* s_searchb;
    //---------MASTER---------
    wxBoxSizer* masterFilesSizer;
    wxStaticText* masterFilesLabel;
    wxListBox* masterFilesListbox;
    wxListBox* mf_lb;
    wxBoxSizer* masterDFilesSizer;
    wxStaticText* masterDFilesLabel;
    wxListBox* masterDFilesListbox;
    wxListBox* mdf_lb;
    wxBoxSizer* sourceCFilesSizer;
    wxStaticText* sourceCFilesLabel;
    wxListBox* scf_lb;
    wxListBox* sourceCFilesBox;
    //--------TARGET---------
    wxBoxSizer* targetSizer;
    wxStaticText* targetLabel;
    wxListBox* targetDirlistbox;
    wxListBox* tdir_lbx;
    wxBoxSizer* targetbtnsVbox;
    wxButton* t_addb;
    wxButton* t_deleteb;
    wxButton* t_clearb;
    wxButton* t_searchb;
    //-------CLIENT---------
    wxBoxSizer* clientFilesSizer;
    wxStaticText* clientFilesLabel;
    wxListBox* clientFilesListbox;
    wxListBox* cf_lb;
    wxBoxSizer* mcFilesbtnsVbox;
    wxButton* mc_syncb;
    wxButton* idFileb;
    wxBoxSizer* targetMFilesSizer;
    wxStaticText* targetMFilesLabel;
    wxListBox* targetMFilesBox;
    wxListBox* tmf_lb;
    //-------SYNC-----------
    wxBoxSizer* syncedFilesSizer;
    wxStaticText* syncedFilesLabel;
    wxListBox* sf_lb;
    wxListBox* syncedFilesBox;
    wxBoxSizer* sFilesbtnsVbox;
    wxButton* exitb;

    DECLARE_EVENT_TABLE()
};