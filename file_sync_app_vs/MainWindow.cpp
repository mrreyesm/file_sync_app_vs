/////////////////////////////////////////////////////////////////////////////
// Name:        File Sync
// Purpose:     An application that Syncs Files
// Developed:   15.01.2022 @HTW Berlin under the Coordination of T. Wendler
/////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------
#include "MainWindow.h"
#include "IdDialog.h"
#include "id.h"
#include <wx/artprov.h>
#include <wx/listbox.h>
#include <wx/dir.h>
#include <wx/regex.h>
#include <wx/textfile.h>
//Event table for static events
BEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_UPDATE_UI(window::id::ID_SYNC,
    MainWindow::onUpdateSyncButton)
EVT_UPDATE_UI(window::id::ID_S_SEARCH,
    MainWindow::onUpdateSearchSourceButton)
EVT_UPDATE_UI(window::id::ID_T_SEARCH,
    MainWindow::onUpdateTargetButton)
EVT_LISTBOX_DCLICK(window::id::ID_S_LISTBOX,
    MainWindow::OnSourceListBoxDirDClick)
EVT_LISTBOX_DCLICK(window::id::ID_T_LISTBOX,
    MainWindow::OnTargetListBoxDirDClick)
EVT_LISTBOX_DCLICK(window::id::ID_MF_LISTBOX,
    MainWindow::OnMasterListBoxFileDClick)
EVT_LISTBOX_DCLICK(window::id::ID_CF_LISTBOX,
    MainWindow::OnClientListBoxFileDClick)
EVT_LISTBOX_DCLICK(window::id::ID_SCF_LISTBOX,
    MainWindow::OnSourceClientListBoxFileDClick)
EVT_LISTBOX_DCLICK(window::id::ID_TMF_LISTBOX,
    MainWindow::OnTargetMasterListBoxFileDClick)
EVT_LISTBOX_DCLICK(window::id::ID_MDF_LISTBOX,
    MainWindow::OnMFDuplicatesListBoxFileDClick)
EVT_LISTBOX_DCLICK(window::id::ID_SF_LISTBOX,
    MainWindow::OnSyncListBoxFileDClick)
END_EVENT_TABLE()
// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
//These global variables keep track of the selected directories so that
//they don't have to be selected manually every time the program runs.
wxTextFile stxtFile(_T("sourcedirs.txt"));
wxTextFile ttxtFile(_T("targetdirs.txt"));
int num_of_sdirs = 0;
int num_of_tdirs = 0;
int nonexistingsdirs = 0;
int nonexistingtdirs = 0;
//These global arrays keeps track of the master/client files 
//that are going to be synced
wxArrayString sourceDirs;
wxArrayString targetDirs;
wxArrayString masterFiles;
wxArrayString clientFiles;
wxArrayString allmasterFiles;
wxArrayString allclientFiles;
wxArrayString syncFiles;
// ----------------------------------------------------------------------------
// Main Window
// ----------------------------------------------------------------------------
MainWindow::MainWindow(wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name) :
    wxFrame(parent, id, title, pos, size, style, name){
// ----------------------------------------------------------------------------
// Menu bar
// ----------------------------------------------------------------------------
    menuBar = new wxMenuBar();
    // ------------------------------------------------------------------------
    // Help
    // ------------------------------------------------------------------------
    menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);
    // ------------------------------------------------------------------------
    // File
    // ------------------------------------------------------------------------
    fileMenu = new wxMenu();
    //Creates the menu item "ID File" that creates unique IDs for a files.
    idFileItem = fileMenu->Append(wxID_ANY, _("&ID Files\tCtrl+I"));
    Bind(wxEVT_MENU, &MainWindow::onIdFile, this, idFileItem->GetId());
    //Separates features from the close event
    fileMenu->AppendSeparator();
    //Creates and defines the menu item to close the app
    quitItem = new wxMenuItem(fileMenu, wxID_EXIT);
    quitItem->SetBitmap(wxArtProvider::GetBitmap("wxART_QUIT"));
    Bind(wxEVT_MENU, [=](wxCommandEvent&) { Close(true); }, wxID_EXIT);
    //Appends the created objects to the menu bar
    fileMenu->Append(quitItem);
    menuBar->Append(fileMenu, _("&File"));
    menuBar->Append(menuHelp, _("&Help"));
    SetMenuBar(menuBar);
// ----------------------------------------------------------------------------
// Layout
// ----------------------------------------------------------------------------
    //Main parent panel
    parentPanel = new wxPanel(this, wxID_ANY);
    //Creates horizontal box sizer to fit left and right panels
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    //Creates left and right panels and 
    //adds them to hbox/horizontal box sizer
    leftPanel = new wxPanel(parentPanel, -1, wxPoint(-1, -1),
        wxSize(-1, -1), wxBORDER_SUNKEN);
    rightPanel = new wxPanel(parentPanel, wxID_ANY, wxDefaultPosition,
        wxSize(270, 150), wxBORDER_SUNKEN);
    hbox->Add(leftPanel, 1, wxEXPAND | wxALL, 5);
    hbox->Add(rightPanel, 1, wxEXPAND | wxALL, 5);
    parentPanel->SetSizer(hbox);
    //Creates vertical boxes for the left and the right panels
    //and assigns them to those panels
    leftPanelSizer = new wxBoxSizer(wxVERTICAL);
    rightPanelSizer = new wxBoxSizer(wxVERTICAL);
    leftPanel->SetSizer(leftPanelSizer);
    rightPanel->SetSizer(rightPanelSizer);
// ------------------------------------------------------------------------
// Left panel
// ------------------------------------------------------------------------
//The left panel contains the source directories and master files
    // ------------------------------------------------------------------------
    // Source directories
    // ------------------------------------------------------------------------
    //Create source sizer,label and listbox
    sourceSizer = new wxBoxSizer(wxHORIZONTAL);
    sourceLabel = new wxStaticText(leftPanel, wxID_ANY, _("Source"));
    sourceLabel->SetMinSize(wxSize(70, sourceLabel->GetMinSize().y));
    sourceSizer->Add(sourceLabel, 0, wxLEFT, 10);
    sdir_lbx = sourceDirlistbox = new wxListBox(leftPanel,
        window::id::ID_S_LISTBOX,wxDefaultPosition, wxSize(450, 110)); //wxLB_OWNERDRAW
    //Fill listbox with source directories from a text file
    wxTextFile stxtFile(_T("sourcedirs.txt"));
    stxtFile.Open();
    std::string sdirNameStr;
    for (sdirNameStr = stxtFile.GetFirstLine(); !stxtFile.Eof();
        sdirNameStr = stxtFile.GetNextLine()){
        if (!wxDirExists(sdirNameStr)) { 
            nonexistingsdirs++; 
            sdir_lbx->Append(sdirNameStr + "<<<<<<<<<WARNING! DIRECTORY NOT FOUND.");
            sdir_lbx->SetBackgroundColour(wxT("red"));
        }
        else {
            sdir_lbx->Append(sdirNameStr);
        }
        num_of_sdirs++;
    }
    if (nonexistingsdirs > 0) {
        wxMessageBox("One or more of the registered source directories has not been found.",
            "Directory not found", wxOK | wxICON_WARNING);
    }
    stxtFile.Close();
    sourceSizer->Add(sourceDirlistbox, 0, wxEXPAND | wxALL, 5);
    // ------------------------------------------------------------------------
    // Action buttons for Source dirs
    // ------------------------------------------------------------------------
    //Create vertical box that contains the action buttons
    sourcebtnsVbox = new wxBoxSizer(wxVERTICAL);
    s_addb = new wxButton(leftPanel, window::id::ID_S_ADD, wxT("Add"));
    s_deleteb = new wxButton(leftPanel, window::id::ID_S_REMOVE, wxT("Remove"));
    s_clearb = new wxButton(leftPanel, window::id::ID_S_CLEAR, wxT("Clear"));
    s_searchb = new wxButton(leftPanel, window::id::ID_S_SEARCH, wxT("Search"));
    //Assign actions to those buttons
    Connect(window::id::ID_S_ADD, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainWindow::OnAddSourceDir));
    Connect(window::id::ID_S_REMOVE, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainWindow::OnDeleteSourceDir));
    Connect(window::id::ID_S_CLEAR, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainWindow::OnClearSourceDirs));
    Connect(window::id::ID_S_SEARCH, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainWindow::OnSearchSourceDirs));
    //Adds each button to the Left panel
    sourcebtnsVbox->Add(-1, 5);
    sourcebtnsVbox->Add(s_addb);
    sourcebtnsVbox->Add(s_deleteb, 0, wxTOP, 5);
    sourcebtnsVbox->Add(s_clearb, 0, wxTOP, 5);
    sourcebtnsVbox->Add(s_searchb, 0, wxTOP, 5);
    sourceSizer->Add(sourcebtnsVbox, 2, wxEXPAND | wxRIGHT, 10);
    leftPanelSizer->Add(-1, 10);
    leftPanelSizer->Add(sourceSizer, 0, wxEXPAND | wxALL, 5);
    // ------------------------------------------------------------------------
    // Master files
    // ------------------------------------------------------------------------
    //Create master files sizer, label and listbox
    masterFilesSizer = new wxBoxSizer(wxHORIZONTAL);
    masterFilesLabel = new wxStaticText(leftPanel, wxID_ANY, _("Master Files"));   
    mf_lb = masterFilesListbox = new wxListBox(leftPanel,
        window::id::ID_MF_LISTBOX, wxDefaultPosition, wxSize(450, 100));
    masterFilesLabel->SetMinSize(wxSize(70, masterFilesLabel->GetMinSize().y));
    masterFilesSizer->Add(masterFilesLabel, 0, wxLEFT, 10);
    masterFilesSizer->Add(masterFilesListbox, 0, wxEXPAND | wxALL, 5);
    leftPanelSizer->Add(masterFilesSizer, 0, wxEXPAND | wxALL, 5);
    // ------------------------------------------------------------------------
    // Master dupplicate files
    // ------------------------------------------------------------------------
    //Create Master duplicates files sizer, label and listbox
    masterDFilesSizer = new wxBoxSizer(wxHORIZONTAL);
    masterDFilesLabel = new wxStaticText(leftPanel,
        wxID_ANY, _("Master Files\nDuplicates"));
    masterDFilesSizer->Add(masterDFilesLabel, 0, wxLEFT, 10);
    masterDFilesLabel->SetMinSize(wxSize(70, masterDFilesLabel->GetMinSize().y));
    mdf_lb = masterDFilesListbox = new wxListBox(leftPanel,
        window::id::ID_MDF_LISTBOX, wxDefaultPosition, wxSize(450, 100));
    masterDFilesSizer->Add(masterDFilesListbox, 0, wxEXPAND | wxALL, 5);
    leftPanelSizer->Add(masterDFilesSizer, 0, wxEXPAND | wxALL, 5);
    // ------------------------------------------------------------------------
    // Source client files
    // ------------------------------------------------------------------------
    // Create Client files in Master directories sizer, label and listbox
    wxBoxSizer* sourceCFilesSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* sourceCFilesLabel = new wxStaticText(leftPanel,
        wxID_ANY, _("Client Files\nin Source"));
    sourceCFilesSizer->Add(sourceCFilesLabel, 0, wxLEFT, 10);
    sourceCFilesLabel->SetMinSize(wxSize(70, sourceCFilesLabel->GetMinSize().y));
    scf_lb = sourceCFilesBox = new wxListBox(leftPanel,
        window::id::ID_SCF_LISTBOX, wxDefaultPosition, wxSize(450, 100));
    sourceCFilesSizer->Add(sourceCFilesBox, 0, wxEXPAND | wxALL, 5);
    leftPanelSizer->Add(sourceCFilesSizer, 0, wxEXPAND | wxALL, 5);
// ------------------------------------------------------------------------
// Right panel
// ------------------------------------------------------------------------
//The right panel contains the target directories and client files
    // ------------------------------------------------------------------------
    // Target directories
    // ------------------------------------------------------------------------
    //Create target file sizer, label and box
    targetSizer = new wxBoxSizer(wxHORIZONTAL);
    targetLabel = new wxStaticText(rightPanel, wxID_ANY, _("Target"));
    targetLabel->SetMinSize(wxSize(50, targetLabel->GetMinSize().y));
    targetSizer->Add(targetLabel, 0, wxLEFT, 10);
    tdir_lbx = targetDirlistbox = new wxListBox(rightPanel,
        window::id::ID_T_LISTBOX, wxDefaultPosition, wxSize(450, 110));
    //Fill listbox with target directories from a text file
    wxTextFile ttxtFile(_T("targetdirs.txt"));
    ttxtFile.Open();
    std::string tdirNameStr;
    for (tdirNameStr = ttxtFile.GetFirstLine(); !ttxtFile.Eof();
        tdirNameStr = ttxtFile.GetNextLine()){
        if (!wxDirExists(tdirNameStr)) {
            nonexistingtdirs++; 
            tdir_lbx->Append(tdirNameStr + "<<<<<<<<<WARNING! DIRECTORY NOT FOUND.");
            tdir_lbx->SetBackgroundColour(wxT("red"));
        }
        else {
            tdir_lbx->Append(tdirNameStr);
        }       
        num_of_tdirs++;
    }
    if (nonexistingtdirs > 0) {
        wxMessageBox("One or more of the registered target directories has not been found.",
            "Directory not found", wxOK | wxICON_WARNING);
    }
    ttxtFile.Close();
    //<<<<<<<<<<<<<<<<<<<<<<RED BACKGROUND
    targetSizer->Add(targetDirlistbox, 0, wxEXPAND | wxALL, 5);
    // ------------------------------------------------------------------------
    // Action buttons for Target dirs
    // ------------------------------------------------------------------------
    //Create vertical box that contains the action buttons
    targetbtnsVbox = new wxBoxSizer(wxVERTICAL);
    t_addb = new wxButton(rightPanel, window::id::ID_T_ADD, wxT("Add"));
    t_deleteb = new wxButton(rightPanel, window::id::ID_T_REMOVE, wxT("Remove"));
    t_clearb = new wxButton(rightPanel, window::id::ID_T_CLEAR, wxT("Clear"));
    t_searchb = new wxButton(rightPanel, window::id::ID_T_SEARCH, wxT("Search"));
    //Assign actions to those buttons
    Connect(window::id::ID_T_ADD, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainWindow::OnAddTargetDir));
    Connect(window::id::ID_T_REMOVE, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainWindow::OnDeleteTargetDir));
    Connect(window::id::ID_T_CLEAR, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainWindow::OnClearTargetDirs));
    Connect(window::id::ID_T_SEARCH, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainWindow::OnSearchTargetDirs));
    //Adds each button to the Right panel
    targetbtnsVbox->Add(-1, 5);
    targetbtnsVbox->Add(t_addb);
    targetbtnsVbox->Add(t_deleteb, 0, wxTOP, 5);
    targetbtnsVbox->Add(t_clearb, 0, wxTOP, 5);
    targetbtnsVbox->Add(t_searchb, 0, wxTOP, 5);
    targetSizer->Add(targetbtnsVbox, 2, wxEXPAND | wxRIGHT, 10);
    rightPanelSizer->Add(-1, 10);
    rightPanelSizer->Add(targetSizer, 0, wxEXPAND | wxALL, 5);
    // ------------------------------------------------------------------------
    // Client files
    // ------------------------------------------------------------------------
    //Create client files sizer, label and listbox
    clientFilesSizer = new wxBoxSizer(wxHORIZONTAL);
    clientFilesLabel = new wxStaticText(rightPanel, wxID_ANY, _("Client\nFiles"));
    clientFilesSizer->Add(clientFilesLabel, 0, wxLEFT, 10);
    clientFilesLabel->SetMinSize(wxSize(50, clientFilesLabel->GetMinSize().y));
    cf_lb = clientFilesListbox = new wxListBox(rightPanel,
        window::id::ID_CF_LISTBOX, wxDefaultPosition, wxSize(450, 100));
    clientFilesSizer->Add(clientFilesListbox, 0, wxEXPAND | wxALL, 5);
    // ------------------------------------------------------------------------
    // Action buttons for Master & Client files
    // ------------------------------------------------------------------------
    //Create vertical box that contains the action buttons
    mcFilesbtnsVbox = new wxBoxSizer(wxVERTICAL);
    mc_syncb = new wxButton(rightPanel, window::id::ID_SYNC, wxT("SYNC"));
    exitb = new wxButton(rightPanel, window::id::ID_EXITAPP, wxT("EXIT"));
    //Assigns actions to those buttons
    Connect(window::id::ID_SYNC, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainWindow::OnSync));
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, [=](wxCommandEvent&) { Close(true); }, window::id::ID_EXITAPP);
    //Add buttons to the right panel
    mcFilesbtnsVbox->Add(-1, 5);
    mcFilesbtnsVbox->Add(mc_syncb);
    mcFilesbtnsVbox->Add(exitb);
    clientFilesSizer->Add(mcFilesbtnsVbox, 2, wxEXPAND | wxRIGHT, 10);
    rightPanelSizer->Add(-1, 10);
    rightPanelSizer->Add(clientFilesSizer, 0, wxEXPAND | wxALL, 5);
    // ------------------------------------------------------------------------
    // Target master files
    // ------------------------------------------------------------------------
    // Create master files in Source directories sizer, label and listbox
    targetMFilesSizer = new wxBoxSizer(wxHORIZONTAL);
    targetMFilesLabel = new wxStaticText(rightPanel, wxID_ANY,
        _("Master\nFiles in\nTarget"));
    targetMFilesLabel->SetMinSize(wxSize(50, targetMFilesLabel->GetMinSize().y));
    targetMFilesSizer->Add(targetMFilesLabel, 0, wxLEFT, 10);
    tmf_lb = targetMFilesBox = new wxListBox(rightPanel,
        window::id::ID_TMF_LISTBOX, wxDefaultPosition, wxSize(450, 100));
    targetMFilesSizer->Add(targetMFilesBox, 0, wxEXPAND | wxALL, 5);
    rightPanelSizer->Add(targetMFilesSizer, 0, wxEXPAND | wxALL, 5);
    // ------------------------------------------------------------------------
    // Synced files
    // ------------------------------------------------------------------------
    // Create synced sizer, label and listbox
    wxBoxSizer* syncedFilesSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* syncedFilesLabel = new wxStaticText(rightPanel,
        wxID_ANY, _("Synced\nFiles"));
    syncedFilesSizer->Add(syncedFilesLabel, 0, wxLEFT, 10);
    syncedFilesLabel->SetMinSize(wxSize(50, syncedFilesLabel->GetMinSize().y));
    sf_lb = syncedFilesBox = new wxListBox(rightPanel,
        window::id::ID_SF_LISTBOX, wxDefaultPosition, wxSize(450, 100));
    syncedFilesSizer->Add(syncedFilesBox, 0, wxEXPAND | wxALL, 5);
    rightPanelSizer->Add(syncedFilesSizer, 0, wxEXPAND | wxALL, 5);
// ------------------------------------------------------------------------
// Status bar and default size
// ------------------------------------------------------------------------
    CreateStatusBar();
    SetStatusText(_(""));
    SetMinSize(wxSize(1300, 600));
}
/*This function creates an instance of the dialog window
when the "ID File" button is pressed*/
void MainWindow::onIdFile(wxCommandEvent& WXUNUSED(event))
{
    IdDialog *idDialog = new IdDialog(this, wxID_ANY, _("ID File"));
    idDialog->Show();
}
/*Gives general information of the app when the "About" button inside
the Help item in the menu bar is pressed*/
void MainWindow::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is an Application to Sync Files and avoid duplicates.",
        "About File Sync", wxOK | wxICON_INFORMATION);
}
/*Opens a dialog box to choose a directory to add
to the Source directories listbox*/
void MainWindow::OnAddSourceDir(wxCommandEvent& event)
{
    wxDirDialog* openDirDialog = new wxDirDialog(this, "Choose Source directories");
    if (openDirDialog->ShowModal() == wxID_OK) {
        wxString DirName = openDirDialog->GetPath();
        sdir_lbx->Append(DirName);
        num_of_sdirs++;
    }
}
//Clears all listboxes in the left panel
void MainWindow::OnClearSourceDirs(wxCommandEvent& event)
{
    int dialog_return_value = wxID_NO;
    wxMessageDialog* dial = new wxMessageDialog(NULL,
        _("Are you sure you want to remove from the list all the current Source directories"),
        _("Remove Source Directories"), wxYES_NO | wxICON_WARNING);
    dialog_return_value = dial->ShowModal();
    switch (dialog_return_value) // Use switch, scales to more buttons later
    {
    case wxID_YES:
        sdir_lbx->Clear();
        mf_lb->Clear();
        mdf_lb->Clear();
        scf_lb->Clear();
        num_of_sdirs = 0;
        nonexistingsdirs = 0;
        sdir_lbx->SetBackgroundColour(wxT("white"));
        //Clears the source directories names in the text file
        stxtFile.Open();
        stxtFile.Clear();
        stxtFile.Close();
    case wxID_NO:
        break;
    default:;
    };
}
//Removes a selected directory from the Source listbox
void MainWindow::OnDeleteSourceDir(wxCommandEvent& event)
{
    int sel = sdir_lbx->GetSelection();
    if (sel != -1) {
        wxString dir = sdir_lbx->GetString(sel);
        if (!wxDirExists(dir)) { nonexistingsdirs--; }
        if (nonexistingsdirs == 0) { sdir_lbx->SetBackgroundColour(wxT("white")); }
        sdir_lbx->Delete(sel);
        num_of_sdirs--;
    }
}
/*Searches for master files inside the directories in the Source listbox.
It also looks for duplicate master files and for client files inside the
source directories.*/
void MainWindow::OnSearchSourceDirs(wxCommandEvent& event)
{
    //Clears all listboxes and arrays and sets counters to cero
    mf_lb->Clear();
    mdf_lb->Clear();
    scf_lb->Clear();
    sf_lb->Clear();
    masterFiles.clear();
    syncFiles.clear();
    int items = 0;
    int seldirs = 0;
    //iterates through all directories to search for files
    for (seldirs ; seldirs < num_of_sdirs; seldirs ++)
    { 
        //extract the path of the selected directory
        wxString pathtmp = sdir_lbx->GetString(seldirs);
        std::string s = std::string(pathtmp.mb_str());
        wxDir dir(s);
        wxString dirName = dir.GetName();
        //fills an array with all the found files
        wxArrayString dirList;
        dir.GetAllFiles(dirName, &dirList, wxEmptyString, wxDIR_FILES | wxDIR_DIRS);
        //creates arrays for the filtered and client files
        wxArrayString filteredDirList;
        wxArrayString clientDirList;
        //regular expressions to spot master and client files
        wxRegEx reMaster(".*IDM.*");
        wxRegEx reClient(".*IDC.*");
        //iterates trough all the files to find master and client files
        for (int i = 0; i < dirList.GetCount(); i++)
        {
            allmasterFiles.Add(dirList[i]);
            if (reMaster.Matches(dirList[i]))
            {
                wxString temp;
                masterFiles.Add(dirList[i]);
                temp = wxFileNameFromPath(dirList[i]);
                filteredDirList.Add(temp);
                items++;
            }
            if (reClient.Matches(dirList[i]))
            {
                wxString temp;
                temp = wxFileNameFromPath(dirList[i]);
                clientDirList.Add(temp);
            }
        }
        //append those files to the listboxes
        mf_lb->Append(filteredDirList);
        scf_lb->Append(clientDirList);
    }
    //Look for duplicate files
    //Declaration of some variables to store characteristics of each file
    wxString ltemp;
    wxString wxltmp;
    std::string ltmp;
    std::string lt;
    std::string lidhash;
    std::string lExtension;
    int lfilelastindex = 0;
    int lidhashindex = 0;
    int lend = 0;

    wxString mtemp;
    wxString wxmtmp;
    std::string mtmp;
    std::string mt;
    std::string midhash;
    std::string mExtension;
    int mfilelastindex = 0;
    int midhashindex = 0;
    int mend = 0;
    for (int l = 0; l < items; l++)
    {
        for (int m = l+1; m < items; m++)
        {
            wxltmp = mf_lb->GetString(l);
            ltmp = std::string(wxltmp.mb_str());
            ltemp = wxFileNameFromPath(ltmp);
            lt = ltemp.ToStdString();
            lend = lt.size();
            lfilelastindex = lt.find_last_of(".");
            lidhashindex = lt.find_last_of("-");
            lidhash = lt.substr(lidhashindex, lfilelastindex);
            lExtension = lt.substr(lfilelastindex, lend);
                
            wxmtmp = mf_lb->GetString(m);
            mtmp = std::string(wxmtmp.mb_str());
            mtemp = wxFileNameFromPath(mtmp);
            mt = mtemp.ToStdString();
            mend = mt.size();
            mfilelastindex = mt.find_last_of(".");
            midhashindex = mt.find_last_of("-");
            midhash = mt.substr(midhashindex, mfilelastindex);
            mExtension = mt.substr(mfilelastindex, mend);
            //Compares the HASHID and the extension of each file to look
            //for duplicate files
            if (lidhash == midhash && lExtension == mExtension)
            {
                mdf_lb->Append(mf_lb->GetString(l));
                wxMessageBox("There is a duplicated Master file. Remove it or rename it\nto continue with the Syncronization",
                    "Duplicate Master file", wxOK | wxICON_WARNING);
            }         
        }
    }
    //Saves the Source directories names into a text file
    stxtFile.Open();
    stxtFile.Clear();
    wxArrayString list = sdir_lbx->GetStrings();
    size_t count = list.Count();
    for (size_t i = 0; i < count; ++i)
        stxtFile.AddLine(list[i]);
    stxtFile.Write();
    stxtFile.Close();
}
/*Opens a dialog box to choose a directory to add
to the Target directories listbox*/
void MainWindow::OnAddTargetDir(wxCommandEvent& event)
{
    wxDirDialog* openDirDialog = new wxDirDialog(this, "Choose Target directories");
    if (openDirDialog->ShowModal() == wxID_OK) {
        wxString DirName = openDirDialog->GetPath();
        tdir_lbx->Append(DirName);
        num_of_tdirs++;
    }
}
//Clears all listboxes in the right panel
void MainWindow::OnClearTargetDirs(wxCommandEvent& event)
{
    int dialog_return_value = wxID_NO;
    wxMessageDialog* dial = new wxMessageDialog(NULL,
        _("Are you sure you want to remove from the list all the current Target directories"),
        _("Remove Target Directories"), wxYES_NO | wxICON_WARNING);
    dialog_return_value = dial->ShowModal();
    switch (dialog_return_value) // Use switch, scales to more buttons later
    {
    case wxID_YES:
        tdir_lbx->Clear();
        cf_lb->Clear();
        tmf_lb->Clear();
        num_of_tdirs = 0;
        nonexistingtdirs = 0;
        tdir_lbx->SetBackgroundColour(wxT("white"));
        //Clears the Target directories names in the text file
        ttxtFile.Open();
        ttxtFile.Clear();
        ttxtFile.Close();
    case wxID_NO:
        break;
    default:;
    };
}
//Removes a selected directory from the Target listbox
void MainWindow::OnDeleteTargetDir(wxCommandEvent& event)
{
    int sel = tdir_lbx->GetSelection();
    if (sel != -1) {
        wxString dir = tdir_lbx->GetString(sel);
        if (!wxDirExists(dir)) { nonexistingtdirs--; }
        if (nonexistingtdirs == 0) { tdir_lbx->SetBackgroundColour(wxT("white")); }
        tdir_lbx->Delete(sel);
        num_of_tdirs--;
    }
}
/*Searches for client files inside the directories in the Target listbox.
It also looks for master files inside the target directories.*/
void MainWindow::OnSearchTargetDirs(wxCommandEvent& event)
{
    //Clears all listboxes and arrays and sets counters to cero
    cf_lb->Clear();
    tmf_lb->Clear();
    sf_lb->Clear();
    syncFiles.clear();
    clientFiles.clear();
    int seldirs = 0;
    //iterates through all directories to search for files
    for (seldirs; seldirs < num_of_tdirs; seldirs++)
    {
        //extract the path of the selected directory
        wxString pathtmp = tdir_lbx->GetString(seldirs);
        std::string s = std::string(pathtmp.mb_str());
        wxDir dir(s);
        wxString dirName = dir.GetName();
        //fills an array with all the found files
        wxArrayString dirList;
        dir.GetAllFiles(dirName, &dirList, wxEmptyString, wxDIR_FILES | wxDIR_DIRS);
        //fills an array with all the found files
        wxArrayString filteredDirList;
        wxArrayString masterDirList;
        //regular expressions to spot master and client files
        wxRegEx reMaster(".*IDM.*");
        wxRegEx reClient(".*IDC.*");
        //iterates trough all the files to find master and client files
        for (int i = 0; i < dirList.GetCount(); i++)
        {
            allclientFiles.Add(dirList[i]);
            if (reClient.Matches(dirList[i]))
            {
                wxString temp;
                clientFiles.Add(dirList[i]);
                temp = wxFileNameFromPath(dirList[i]);
                filteredDirList.Add(temp);
            }
            if (reMaster.Matches(dirList[i]))
            {
                wxString temp;
                temp = wxFileNameFromPath(dirList[i]);
                masterDirList.Add(temp);
            }
        }
        //append those files to the listboxes
        cf_lb->Append(filteredDirList);
        tmf_lb->Append(masterDirList);
    }
    //Saves the Target directories names into a text file
    ttxtFile.Open();
    ttxtFile.Clear();
    wxArrayString list = tdir_lbx->GetStrings();
    size_t count = list.Count();
    for (size_t i = 0; i < count; ++i)
        ttxtFile.AddLine(list[i]);
    ttxtFile.Write();
    ttxtFile.Close();
}
/*This function syncronizes the client files with the master files.
It looks for files with the same names and extensions but different IDS
and copies the master files to the location of the client file with the
client file name.
NOTE: In order to use it there should not be duplicate master files.
*/
void MainWindow::OnSync(wxCommandEvent& event)
{
    //This make sure that there are no duplicate master files and 
    //that the master and client files listboxes are not empty
    if (mdf_lb->IsEmpty()&& !mf_lb->IsEmpty() && !cf_lb->IsEmpty())
    {
        //Creates variables to store names, extensions, ids and location
        //of certain characters inside strings
        wxString mastertemp;
        std::string masterNameTemp;
        std::string masterExtension;
        std::string m;
        int mend = 0;
        int masterlastindex = 0;
        int masteridhashindex = 0;
        int masteridindex = 0;
        wxString clienttemp;
        std::string clientNameTemp;
        std::string clientExtension;
        std::string clientIdTemp;
        std::string c;
        int cend = 0;
        int clientlastindex = 0;
        int clientidindex = 0;
        int clientidhashindex = 0;
        //clears synced files listbox and array
        syncFiles.clear();
        sf_lb->Clear();
        //this flag/counter makes sure that there are files to be updated
        int flag = 0;
        //Iterates through all master files and compares them to the client files
        for (int l = 0; l < masterFiles.GetCount(); l++)
        {
            //stores different characteristics of a master file
            mastertemp = wxFileNameFromPath(masterFiles[l]);
            m = mastertemp.ToStdString();
            mend = m.size();
            masterlastindex = m.find_last_of(".");
            masteridindex = m.find_last_of("_");
            masteridhashindex = m.find_last_of("-");
            masterNameTemp = m.substr(masteridhashindex, masterlastindex);
            masterExtension = m.substr(masterlastindex, mend);
            //iterates through the client files and compares them to the current
            //master file
            for (int n = 0; n < clientFiles.GetCount(); n++)
            {
                //stores different characteristics of a client file
                clienttemp = wxFileNameFromPath(clientFiles[n]);
                c = clienttemp.ToStdString();
                cend = c.size();
                clientlastindex = c.find_last_of(".");
                clientidindex = c.find_last_of("_");
                clientidhashindex = c.find_last_of("-");
                clientNameTemp = c.substr(clientidhashindex, clientlastindex);
                clientExtension = c.substr(clientlastindex, cend);
                clientIdTemp = c.substr(clientidindex, clientlastindex);
                //if the name and extension of a master file and a client file match
                //the master file is copied to the client file location and overwrites
                //that file with a copy of the master file but with the client file name
                if (masterNameTemp == clientNameTemp && masterExtension == clientExtension)
                {
                    time_t mfiletimestamp = wxFileModificationTime(masterFiles[l]);
                    time_t cfiletimestamp = wxFileModificationTime(clientFiles[n]);
                    if (cfiletimestamp < mfiletimestamp) {
                        flag++;
                        syncFiles.Add(clientFiles[n]);
                        if (wxCopyFile(masterFiles[l], clientFiles[n]) != 0)
                            perror("Error moving file");
                        PushStatusText(_("Syncing!"));
                    }
                    else {
                        int dialog_return_value = wxID_NO;
                        wxMessageDialog* dial = new wxMessageDialog(NULL,
                            _("The file\n" + clientFiles[n] + "\nHas been modified more recently than the master file.\nWould you like to overwrite it?"),
                            _("Sync Files"), wxYES_NO | wxICON_WARNING);
                        dialog_return_value = dial->ShowModal();
                        switch (dialog_return_value) // Use switch, scales to more buttons later
                        {
                        case wxID_YES:
                            flag++;
                            syncFiles.Add(clientFiles[n]);
                            if (wxCopyFile(masterFiles[l], clientFiles[n]) != 0)
                                perror("Error moving file");
                            PushStatusText(_("Syncing!"));
                        case wxID_NO:
                            break;
                        default:;
                        };
                    }
                }
            }
        }
        //if there are no files to be updated it raises a dialog window
        if (!flag)
        {
            wxMessageBox("There are no files to be updated.",
                "Notification", wxOK | wxICON_INFORMATION);
        }
        //shows the updates/syncronized files in the listbox and throws a dialog
        sf_lb->Append(syncFiles);
        wxMessageBox("Files are syncronized!",
            "Sync", wxOK | wxICON_INFORMATION);
        PushStatusText(_(""));
    }
    //Throws a dialog if one of the master/client listboxes is empty
    else {
        wxMessageBox("Please make sure that there are files in the Master files box and\
            in the Client files box and that there are no duplicated Master Files.",
            "Warning", wxOK | wxICON_WARNING);
    }  
}
void MainWindow::onUpdateSyncButton(wxUpdateUIEvent& event)
{
    event.Enable(false);
    if (mdf_lb->IsEmpty() && !mf_lb->IsEmpty() && !cf_lb->IsEmpty())
    {
     event.Enable(true);
    }
}
void MainWindow::onUpdateSearchSourceButton(wxUpdateUIEvent& event)
{
    if (nonexistingsdirs == 0)
    {
        event.Enable(true);
        sdir_lbx->SetBackgroundColour(wxT("white"));
    }
    else {
        event.Enable(false);
    }
}
void MainWindow::onUpdateTargetButton(wxUpdateUIEvent& event)
{
    if (nonexistingtdirs == 0)
    {
        event.Enable(true);
        tdir_lbx->SetBackgroundColour(wxT("white"));
    }
    else {
        event.Enable(false);
    }
}
//this function opens a file explorer with the selected source dir
void MainWindow::OnSourceListBoxDirDClick(wxCommandEvent& event)
{
    int sel = sdir_lbx->GetSelection();
    wxString pathtmp = sdir_lbx->GetString(sel);
    std::string path = std::string(pathtmp.mb_str());
    if (wxDirExists(path)) {
        wxString command = "explorer " + pathtmp;
        wxExecute(command, wxEXEC_ASYNC, NULL);
    }
    else {
        wxMessageBox("The selected directory does not exist.",
            "Warning", wxOK | wxICON_WARNING);
    }

}
//this function opens a file explorer with the selected target dir
void MainWindow::OnTargetListBoxDirDClick(wxCommandEvent& event)
{
    int sel = tdir_lbx->GetSelection();
    wxString pathtmp = tdir_lbx->GetString(sel);
    std::string path = std::string(pathtmp.mb_str());
    if (wxDirExists(path)) {
        wxString command = "explorer " + pathtmp;
        wxExecute(command, wxEXEC_ASYNC, NULL);
    }
    else {
        wxMessageBox("The selected directory does not exist.",
            "Warning", wxOK | wxICON_WARNING);
    }
}
//this function opens a file explorer with the selected file
//and it can also open the file if requested
void MainWindow::OnMasterListBoxFileDClick(wxCommandEvent& event)
{
    int sel = mf_lb->GetSelection();
    wxString selectedfile = mf_lb->GetString(sel);
    for (int i = 0; i < masterFiles.Count(); i++) {
        if (selectedfile == wxFileNameFromPath(masterFiles[i])) {
            wxString completepath = masterFiles[i];
            wxString pathtmp = wxPathOnly(masterFiles[i]);
            wxString opendir = "explorer " + pathtmp;
            wxString openfile = "explorer " + completepath;
            wxExecute(opendir, wxEXEC_ASYNC, NULL);

            int dialog_return_value = wxID_NO;
            wxMessageDialog* dial = new wxMessageDialog(NULL,
                _("Directory opened.\nWould you like to open the file?"),
                _("Master Files"), wxYES_NO | wxICON_QUESTION);
            dialog_return_value = dial->ShowModal();
            switch (dialog_return_value) // Use switch, scales to more buttons later
            {
            case wxID_YES:
                wxExecute(openfile, wxEXEC_ASYNC, NULL);
            case wxID_NO:
                break;
            default:;
            };
        }
    }
}
//this function opens a file explorer with the selected file
//and it can also open the file if requested
void MainWindow::OnClientListBoxFileDClick(wxCommandEvent& event)///////////////////
{
    int sel = cf_lb->GetSelection();
    wxString selectedfile = cf_lb->GetString(sel);
    for (int i = 0; i < clientFiles.Count(); i++) {
        if (selectedfile == wxFileNameFromPath(clientFiles[i])) {
            wxString completepath = clientFiles[i];
            wxString pathtmp = wxPathOnly(clientFiles[i]);
            wxString opendir = "explorer " + pathtmp;
            wxString openfile = "explorer " + completepath;
            wxExecute(opendir, wxEXEC_ASYNC, NULL);

            int dialog_return_value = wxID_NO;
            wxMessageDialog* dial = new wxMessageDialog(NULL,
                _("Directory opened.\nWould you like to open the file?"),
                _("Client Files"), wxYES_NO | wxICON_QUESTION);
            dialog_return_value = dial->ShowModal();
            switch (dialog_return_value) // Use switch, scales to more buttons later
            {
            case wxID_YES:
                wxExecute(openfile, wxEXEC_ASYNC, NULL);
            case wxID_NO:
                break;
            default:;
            };
        }
    }
}
//this function opens a file explorer with the selected file
//and it can also open the file if requested
void MainWindow::OnSourceClientListBoxFileDClick(wxCommandEvent& event)//////////////////
{
    int sel = scf_lb->GetSelection();
    wxString selectedfile = scf_lb->GetString(sel);
    for (int i = 0; i < allmasterFiles.Count(); i++) {
        if (selectedfile == wxFileNameFromPath(allmasterFiles[i])) {
            wxString completepath = allmasterFiles[i];
            wxString pathtmp = wxPathOnly(allmasterFiles[i]);
            wxString opendir = "explorer " + pathtmp;
            wxString openfile = "explorer " + completepath;
            wxExecute(opendir, wxEXEC_ASYNC, NULL);

            int dialog_return_value = wxID_NO;
            wxMessageDialog* dial = new wxMessageDialog(NULL,
                _("Directory opened.\nWould you like to open the file?"),
                _("Client files in Source directories"), wxYES_NO | wxICON_QUESTION);
            dialog_return_value = dial->ShowModal();
            switch (dialog_return_value) // Use switch, scales to more buttons later
            {
            case wxID_YES:
                wxExecute(openfile, wxEXEC_ASYNC, NULL);
            case wxID_NO:
                break;
            default:;
            };
        }
    }
}
//this function opens a file explorer with the selected file
//and it can also open the file if requested
void MainWindow::OnTargetMasterListBoxFileDClick(wxCommandEvent& event)///////////
{
    int sel = tmf_lb->GetSelection();
    wxString selectedfile = tmf_lb->GetString(sel);
    for (int i = 0; i < allclientFiles.Count(); i++) {
        if (selectedfile == wxFileNameFromPath(allclientFiles[i])) {
            wxString completepath = allclientFiles[i];
            wxString pathtmp = wxPathOnly(allclientFiles[i]);
            wxString opendir = "explorer " + pathtmp;
            wxString openfile = "explorer " + completepath;
            wxExecute(opendir, wxEXEC_ASYNC, NULL);

            int dialog_return_value = wxID_NO;
            wxMessageDialog* dial = new wxMessageDialog(NULL,
                _("Directory opened.\nWould you like to open the file?"),
                _("Master Files in Target directories"), wxYES_NO | wxICON_QUESTION);
            dialog_return_value = dial->ShowModal();
            switch (dialog_return_value) // Use switch, scales to more buttons later
            {
            case wxID_YES:
                wxExecute(openfile, wxEXEC_ASYNC, NULL);
            case wxID_NO:
                break;
            default:;
            };
        }
    }
}
//this function opens a file explorer with the selected file
//and it can also open the file if requested
void MainWindow::OnMFDuplicatesListBoxFileDClick(wxCommandEvent& event)//////////////
{
    int sel = mdf_lb->GetSelection();
    wxString selectedfile = mdf_lb->GetString(sel);
    for (int i = 0; i < masterFiles.Count(); i++) {
        if (selectedfile == wxFileNameFromPath(masterFiles[i])) {
            wxString completepath = masterFiles[i];
            wxString pathtmp = wxPathOnly(masterFiles[i]);
            wxString opendir = "explorer " + pathtmp;
            wxString openfile = "explorer " + completepath;
            wxExecute(opendir, wxEXEC_ASYNC, NULL);

            int dialog_return_value = wxID_NO;
            wxMessageDialog* dial = new wxMessageDialog(NULL,
                _("Directory opened.\nWould you like to open the file?"),
                _("Master Duplicated Files"), wxYES_NO | wxICON_QUESTION);
            dialog_return_value = dial->ShowModal();
            switch (dialog_return_value) // Use switch, scales to more buttons later
            {
            case wxID_YES:
                wxExecute(openfile, wxEXEC_ASYNC, NULL);
            case wxID_NO:
                break;
            default:;
            };
        }
    }
}
//this function opens a file explorer with the selected file
//and it can also open the file if requested
void MainWindow::OnSyncListBoxFileDClick(wxCommandEvent& event)
{
    int sel = sf_lb->GetSelection();
    wxString filepath = sf_lb->GetString(sel);
    std::string file = std::string(filepath.mb_str());
    wxString pathtmp = wxPathOnly(sf_lb->GetString(sel));
    std::string path = std::string(pathtmp.mb_str());
    wxString opendir = "explorer " + path;
    wxString openfile = "explorer " + file;
    wxExecute(opendir, wxEXEC_ASYNC, NULL);

    int dialog_return_value = wxID_NO;
    wxMessageDialog* dial = new wxMessageDialog(NULL,
        _("Directory opened.\nWould you like to open the file?"),
        _("Synced Files"), wxYES_NO | wxICON_QUESTION);
    dialog_return_value = dial->ShowModal();
    switch (dialog_return_value) // Use switch, scales to more buttons later
    {
    case wxID_YES:
        wxExecute(openfile, wxEXEC_ASYNC, NULL);
    case wxID_NO:
        break;
    default:;
    };  
}
MainWindow::~MainWindow() {}