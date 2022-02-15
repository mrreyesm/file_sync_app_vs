/////////////////////////////////////////////////////////////////////////////
// Name:        File Sync
// Purpose:     An application that Syncs Files
// Developed:   15.01.2022 @HTW Berlin under the Coordination of Tilo Wendler
/////////////////////////////////////////////////////////////////////////////
#include "MainWindow.h"
#include "IdDialog.h"
#include <wx/artprov.h>
#include <wx/listbox.h>
#include <wx/dir.h>
#include <wx/regex.h>

//The Main window object is defined
MainWindow::MainWindow(wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name) :
    wxFrame(parent, id, title, pos, size, style, name)
{
    //Creates Menu Bar 
    wxMenuBar* menuBar = new wxMenuBar();
    //Creates the menu bar option "Help" that contains general info of the app
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);

    //Creates the menu bar option "File" that contains more features of the app
    wxMenu* fileMenu = new wxMenu();
    //Creates the menu item "ID File"that creates a unique ID for a file.
    wxMenuItem* testItem = fileMenu->Append(wxID_ANY, _("&ID Files\tCtrl+I"));
    Bind(wxEVT_MENU, &MainWindow::onIdFile, this, testItem->GetId());
    //Separates features from the close event
    fileMenu->AppendSeparator();
    //Creates and defines the menu item to close the app
    wxMenuItem* quitItem = new wxMenuItem(fileMenu, wxID_EXIT);
    quitItem->SetBitmap(wxArtProvider::GetBitmap("wxART_QUIT"));
    Bind(wxEVT_MENU, [=](wxCommandEvent&) { Close(true); }, wxID_EXIT);
    fileMenu->Append(quitItem);
    //Appends the created objects to the menu bar
    menuBar->Append(fileMenu, _("&File"));
    menuBar->Append(menuHelp, _("&Help"));
    SetMenuBar(menuBar);


    // Create the layout with labels and boxes
    // parent panel
    m_parent = new wxPanel(this, wxID_ANY);
    // creates sizer to fit left and right panels
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    // creates left and right panels and adds them to hbox
    wxPanel *m_lp = new wxPanel(m_parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN);
    wxPanel *m_rp = new wxPanel(m_parent, wxID_ANY, wxDefaultPosition,wxSize(270, 150), wxBORDER_SUNKEN);
    hbox->Add(m_lp, 1, wxEXPAND | wxALL, 5);
    hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);
    m_parent->SetSizer(hbox);
    // creates vertical boxes for the left and the right panels and asigns them to those panels
    wxBoxSizer* m_lpSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* m_rpSizer = new wxBoxSizer(wxVERTICAL);
    m_lp->SetSizer(m_lpSizer);
    m_rp->SetSizer(m_rpSizer);

    //-----------------------------LEFT PANEL--------------------------------------------
    //Create Source file sizer, label and box
    wxBoxSizer* sourceSizer = new wxBoxSizer(wxHORIZONTAL);
    // Create source label
    wxStaticText* sourceLabel = new wxStaticText(m_lp, wxID_ANY, _("Source"));
    sourceSizer->Add(sourceLabel, 0, wxLEFT, 10);
    sourceLabel->SetMinSize(wxSize(70, sourceLabel->GetMinSize().y));
    //create listbox for the selected directories
    m_lb = listbox = new wxListBox(m_lp, ID_LISTBOX, wxDefaultPosition, wxSize(305, 110));
    sourceSizer->Add(listbox, 0, wxEXPAND | wxALL, 5);
    // creats a vertical box panel to put the buttons
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    m_newb = new wxButton(m_lp, wxID_NEW, wxT("Add"));
    m_deleteb = new wxButton(m_lp, wxID_DELETE, wxT("Remove"));
    m_clearb = new wxButton(m_lp, wxID_CLEAR, wxT("Clear"));
    m_searchb = new wxButton(m_lp, ID_SEARCH, wxT("Search"));
    //assigns actions to those buttons
    Connect(wxID_NEW, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnNew));
    Connect(wxID_CLEAR, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnClear));
    Connect(wxID_DELETE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnDelete));
    Connect(ID_SEARCH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnSearch));
    // adds each button to the panel
    vbox->Add(-1, 5);
    vbox->Add(m_newb);
    vbox->Add(m_deleteb, 0, wxTOP, 5);
    vbox->Add(m_clearb, 0, wxTOP, 5);
    vbox->Add(m_searchb, 0, wxTOP, 5);
    // adds the button panel to the source sizer
    sourceSizer->Add(vbox, 2, wxEXPAND | wxRIGHT, 10);
    m_lpSizer->Add(-1, 10);
    m_lpSizer->Add(sourceSizer, 0, wxEXPAND | wxALL, 5);

    // Create box for Master files
    wxBoxSizer* sourceMFileSizer = new wxBoxSizer(wxHORIZONTAL);
    // Create master file label
    wxStaticText* sourceMFileLabel = new wxStaticText(m_lp, wxID_ANY, _("Master Files"));
    sourceMFileSizer->Add(sourceMFileLabel, 0, wxLEFT, 10);
    sourceMFileLabel->SetMinSize(wxSize(70, sourceMFileLabel->GetMinSize().y));
    // Create Maste files list box
    m_lb3 = sourceMFileBox = new wxListBox(m_lp, ID_LISTBOX2, wxDefaultPosition, wxSize(305, 100));
    sourceMFileSizer->Add(sourceMFileBox, 0, wxEXPAND | wxALL, 5);
    m_lpSizer->Add(sourceMFileSizer, 0, wxEXPAND | wxALL, 5);


    // Create box for Master duplicates files
    wxBoxSizer* sourceMDFileSizer = new wxBoxSizer(wxHORIZONTAL);
    // Create master file label
    wxStaticText* sourceMDFileLabel = new wxStaticText(m_lp, wxID_ANY, _("Master Files\nDuplicates"));
    sourceMDFileSizer->Add(sourceMDFileLabel, 0, wxLEFT, 10);
    sourceMDFileLabel->SetMinSize(wxSize(70, sourceMDFileLabel->GetMinSize().y));
    // Create Maste files list box
    wxListBox* sourceMDFileBox = new wxListBox(m_lp, ID_LISTBOX3, wxDefaultPosition, wxSize(305, 100));
    sourceMDFileSizer->Add(sourceMDFileBox, 0, wxEXPAND | wxALL, 5);
    m_lpSizer->Add(sourceMDFileSizer, 0, wxEXPAND | wxALL, 5);



    //---------------------------------RIGHT PANEL------------------------------------------
    //Create target file sizer, label and box
    wxBoxSizer* targetSizer = new wxBoxSizer(wxHORIZONTAL);
    // Create source label
    wxStaticText* targetLabel = new wxStaticText(m_rp, wxID_ANY, _("Target"));
    targetSizer->Add(targetLabel, 0, wxLEFT, 10);
    targetLabel->SetMinSize(wxSize(50, targetLabel->GetMinSize().y));
    //create listbox for the selected directories
    m_lb2 = listbox2 = new wxListBox(m_rp, ID_LISTBOX4, wxDefaultPosition, wxSize(325, 110));
    targetSizer->Add(listbox2, 0, wxEXPAND | wxALL, 5);
    // creats a vertical box panel to put the buttons
    wxBoxSizer* vbox2 = new wxBoxSizer(wxVERTICAL);
    m_newb2 = new wxButton(m_rp, wxID_NEW+1, wxT("Add"));
    m_deleteb2 = new wxButton(m_rp, wxID_DELETE+1, wxT("Remove"));
    m_clearb2 = new wxButton(m_rp, wxID_CLEAR+1, wxT("Clear"));
    m_searchb2 = new wxButton(m_rp, ID_SEARCH2, wxT("Search"));
    //assigns actions to those buttons
    Connect(wxID_NEW+1, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnNew2));
    Connect(wxID_CLEAR+1, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnClear2));
    Connect(wxID_DELETE+1, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnDelete2));
    Connect(ID_SEARCH2, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnSearch2));
    // adds each button to the panel
    vbox2->Add(-1, 5);
    vbox2->Add(m_newb2);
    vbox2->Add(m_deleteb2, 0, wxTOP, 5);
    vbox2->Add(m_clearb2, 0, wxTOP, 5);
    vbox2->Add(m_searchb2, 0, wxTOP, 5);
    // adds the button panel to the source sizer
    targetSizer->Add(vbox2, 2, wxEXPAND | wxRIGHT, 10);
    m_rpSizer->Add(-1, 10);
    m_rpSizer->Add(targetSizer, 0, wxEXPAND | wxALL, 5);

    // Create box for Master files
    wxBoxSizer* targetCFileSizer = new wxBoxSizer(wxHORIZONTAL);
    // Create master file label
    wxStaticText* targetCFileLabel = new wxStaticText(m_rp, wxID_ANY, _("Client Files"));
    targetCFileSizer->Add(targetCFileLabel, 0, wxLEFT, 10);
    targetCFileLabel->SetMinSize(wxSize(50, targetCFileLabel->GetMinSize().y));
    // Create Maste files list box
    m_lb4 = targetCFileBox = new wxListBox(m_rp, ID_LISTBOX5, wxDefaultPosition, wxSize(325, 100));
    targetCFileSizer->Add(targetCFileBox, 0, wxEXPAND | wxALL, 5);
    m_rpSizer->Add(targetCFileSizer, 0, wxEXPAND | wxALL, 5);

    //-----------------------------------------------------------------------------------------
    //Creates Status bar that can be updated in different steps
    CreateStatusBar();
    SetStatusText(_("Syncing!"));

    SetMinSize(wxSize(1020, 500));
}

void MainWindow::onIdFile(wxCommandEvent& WXUNUSED(event))
{
        IdDialog *idDialog = new IdDialog(this, wxID_ANY, _("ID File"));
        idDialog->Show();
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is an Application to Sync Files and avoid duplicates.",
        "About File Sync", wxOK | wxICON_INFORMATION);
}

void MainWindow::OnNew(wxCommandEvent& event)
{
    wxDirDialog* openDirDialog = new wxDirDialog(this, "Choose Master directories");
    if (openDirDialog->ShowModal() == wxID_OK) {
        wxString DirName = openDirDialog->GetPath();
        m_lb->Append(DirName);
    }
}

void MainWindow::OnClear(wxCommandEvent& event)
{
    m_lb->Clear();
}

void MainWindow::OnDelete(wxCommandEvent& event)
{
    int sel = m_lb->GetSelection();
    if (sel != -1) {
        m_lb->Delete(sel);
    }
}

void MainWindow::OnSearch(wxCommandEvent& event)
{
    m_lb3->Clear();
    int seldirs = 0;
    int max = m_lb3->GetCount();
    for (seldirs ; seldirs <= max+1; seldirs ++)
    { 
    wxString pathtmp = m_lb->GetString(seldirs);
    std::string s = std::string(pathtmp.mb_str());

    wxDir dir(s);
    wxString dirName = dir.GetName();
    wxArrayString dirList;
    dir.GetAllFiles(dirName, &dirList, wxEmptyString, wxDIR_FILES | wxDIR_DIRS);
    int arraylength = dirList.GetCount();
    wxArrayString filteredDirList;
    wxRegEx re("tes");
/*
if (re.Matches(string)

        for (int i = 0; i <= arraylength; i++)
        {
            if (re.Matches(dirList.Item(i)))
            {
                wxString temp;
                temp = dirList.Item(i);
                dirList = filteredDirList.Add(temp);
            }
        }
*/
    m_lb3->Append(dirList);
    }
}

void MainWindow::OnNew2(wxCommandEvent& event)
{
    wxDirDialog* openDirDialog = new wxDirDialog(this, "Choose Client directories");
    if (openDirDialog->ShowModal() == wxID_OK) {
        wxString DirName = openDirDialog->GetPath();
        m_lb2->Append(DirName);
    }
}

void MainWindow::OnClear2(wxCommandEvent& event)
{
    m_lb2->Clear();
}

void MainWindow::OnDelete2(wxCommandEvent& event)
{
    int sel = m_lb2->GetSelection();
    if (sel != -1) {
        m_lb2->Delete(sel);
    }
}

void MainWindow::OnSearch2(wxCommandEvent& event)
{
    m_lb4->Clear();
    int seldirs = 0;
    int max = m_lb4->GetCount();
    for (seldirs; seldirs <= max + 1; seldirs++)
    {
        wxString pathtmp = m_lb2->GetString(seldirs);
        std::string s = std::string(pathtmp.mb_str());

        wxDir dir(s);
        wxString dirName = dir.GetName();
        wxArrayString dirList;
        dir.GetAllFiles(dirName, &dirList, wxEmptyString, wxDIR_FILES | wxDIR_DIRS);
        m_lb4->Append(dirList);
    }
}

MainWindow::~MainWindow() {}