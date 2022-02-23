/////////////////////////////////////////////////////////////////////////////
// Name:        File Sync
// Purpose:     An application that Syncs Files
// Developed:   15.01.2022 @HTW Berlin under the Coordination of T. Wendler
/////////////////////////////////////////////////////////////////////////////
#include "MainWindow.h"
#include "IdDialog.h"
#include <wx/artprov.h>
#include <wx/listbox.h>
#include <wx/dir.h>
#include <wx/regex.h>
#include <wx/textfile.h>

// temp solution, to getting number of items from listbox
int num_of_mdirs = 0;
int num_of_cdirs = 0;
wxTextFile file(_T("masterdirs.txt"));
wxTextFile file2(_T("clientdirs.txt"));
 
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
    m_lb = listbox = new wxListBox(m_lp, ID_LISTBOX, wxDefaultPosition, wxSize(450, 110));

    //Fill listbox with master directories
    wxTextFile file(_T("masterdirs.txt"));
    file.Open();
    std::string dirStr;
    for (dirStr = file.GetFirstLine(); !file.Eof(); dirStr = file.GetNextLine())
    {
        m_lb->Append(dirStr);
        num_of_mdirs++;
    }
    file.Close();

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
    m_lb3 = sourceMFileBox = new wxListBox(m_lp, ID_LISTBOX2, wxDefaultPosition, wxSize(450, 100));
    sourceMFileSizer->Add(sourceMFileBox, 0, wxEXPAND | wxALL, 5);
    m_lpSizer->Add(sourceMFileSizer, 0, wxEXPAND | wxALL, 5);


    // Create box for Master duplicates files
    wxBoxSizer* sourceMDFileSizer = new wxBoxSizer(wxHORIZONTAL);
    // Create master file label
    wxStaticText* sourceMDFileLabel = new wxStaticText(m_lp, wxID_ANY, _("Master Files\nDuplicates"));
    sourceMDFileSizer->Add(sourceMDFileLabel, 0, wxLEFT, 10);
    sourceMDFileLabel->SetMinSize(wxSize(70, sourceMDFileLabel->GetMinSize().y));
    // Create Maste files list box
    m_lb5 = sourceMDFileBox = new wxListBox(m_lp, ID_LISTBOX3, wxDefaultPosition, wxSize(450, 100));
    sourceMDFileSizer->Add(sourceMDFileBox, 0, wxEXPAND | wxALL, 5);
    m_lpSizer->Add(sourceMDFileSizer, 0, wxEXPAND | wxALL, 5);

    // Create box for Client files in Master directories
    wxBoxSizer* sourceCFileSizer = new wxBoxSizer(wxHORIZONTAL);
    // Create label
    wxStaticText* sourceCFileLabel = new wxStaticText(m_lp, wxID_ANY, _("Client Files\nin Source"));
    sourceCFileSizer->Add(sourceCFileLabel, 0, wxLEFT, 10);
    sourceCFileLabel->SetMinSize(wxSize(70, sourceCFileLabel->GetMinSize().y));
    // Create list box
    m_lb6 = sourceCFileBox = new wxListBox(m_lp, ID_LISTBOX6, wxDefaultPosition, wxSize(450, 100));
    sourceCFileSizer->Add(sourceCFileBox, 0, wxEXPAND | wxALL, 5);
    m_lpSizer->Add(sourceCFileSizer, 0, wxEXPAND | wxALL, 5);

    //---------------------------------RIGHT PANEL------------------------------------------
    //Create target file sizer, label and box
    wxBoxSizer* targetSizer = new wxBoxSizer(wxHORIZONTAL);
    // Create source label
    wxStaticText* targetLabel = new wxStaticText(m_rp, wxID_ANY, _("Target"));
    targetSizer->Add(targetLabel, 0, wxLEFT, 10);
    targetLabel->SetMinSize(wxSize(50, targetLabel->GetMinSize().y));
    //create listbox for the selected directories
    m_lb2 = listbox2 = new wxListBox(m_rp, ID_LISTBOX4, wxDefaultPosition, wxSize(450, 110));

    //Fill listbox with master directories
    wxTextFile file2(_T("clientdirs.txt"));
    file2.Open();
    std::string cliStr;
    for (cliStr = file2.GetFirstLine(); !file2.Eof(); cliStr = file2.GetNextLine())
    {
        m_lb2->Append(cliStr);
        num_of_cdirs++;
    }
    file2.Close();

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

    // Create box for Client files
    wxBoxSizer* targetCFileSizer = new wxBoxSizer(wxHORIZONTAL);
    // Create Client file label
    wxStaticText* targetCFileLabel = new wxStaticText(m_rp, wxID_ANY, _("Client\nFiles"));
    targetCFileSizer->Add(targetCFileLabel, 0, wxLEFT, 10);
    targetCFileLabel->SetMinSize(wxSize(50, targetCFileLabel->GetMinSize().y));
    // Create Client files list box
    m_lb4 = targetCFileBox = new wxListBox(m_rp, ID_LISTBOX5, wxDefaultPosition, wxSize(450, 100));
    targetCFileSizer->Add(targetCFileBox, 0, wxEXPAND | wxALL, 5);


    // creats a vertical box panel to put one buttons
    wxBoxSizer* vbox3 = new wxBoxSizer(wxVERTICAL);
    m_syncb = new wxButton(m_rp, ID_SYNC, wxT("SYNC"));
    //assigns actions to those buttons
    Connect(ID_SYNC, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnSync));
    // adds each button to the panel
    vbox3->Add(-1, 5);
    vbox3->Add(m_syncb);
    // adds the button panel to the source sizer
    targetCFileSizer->Add(vbox3, 2, wxEXPAND | wxRIGHT, 10);
    m_rpSizer->Add(-1, 10);
    m_rpSizer->Add(targetCFileSizer, 0, wxEXPAND | wxALL, 5);


    // Create box for Master files in target
    wxBoxSizer* targetMFileSizer = new wxBoxSizer(wxHORIZONTAL);
    // Create Client file label
    wxStaticText* targetMFileLabel = new wxStaticText(m_rp, wxID_ANY, _("Master\nFiles in\nTarget"));
    targetMFileSizer->Add(targetMFileLabel, 0, wxLEFT, 10);
    targetMFileLabel->SetMinSize(wxSize(50, targetMFileLabel->GetMinSize().y));
    // Create Client files list box
    m_lb7 = targetMFileBox = new wxListBox(m_rp, ID_LISTBOX7, wxDefaultPosition, wxSize(450, 100));
    targetMFileSizer->Add(targetMFileBox, 0, wxEXPAND | wxALL, 5);
    m_rpSizer->Add(targetMFileSizer, 0, wxEXPAND | wxALL, 5);


    //-----------------------------------------------------------------------------------------
    //Creates Status bar that can be updated in different steps
    CreateStatusBar();
    SetStatusText(_("Syncing!"));

    SetMinSize(wxSize(1300, 600));
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
        num_of_mdirs++;
    }
}

void MainWindow::OnClear(wxCommandEvent& event)
{
    m_lb->Clear();
    m_lb3->Clear();
    m_lb5->Clear();
    m_lb6->Clear();
    num_of_mdirs = 0;
}

void MainWindow::OnDelete(wxCommandEvent& event)
{
    int sel = m_lb->GetSelection();
    if (sel != -1) {
        m_lb->Delete(sel);
        num_of_mdirs--;
    }
}

void MainWindow::OnSearch(wxCommandEvent& event)
{
    m_lb3->Clear();
    m_lb5->Clear();
    m_lb6->Clear();
    int items = 0;
    int seldirs = 0;
    int max = m_lb3->GetCount();
    for (seldirs ; seldirs < num_of_mdirs; seldirs ++)
    { 
    wxString pathtmp = m_lb->GetString(seldirs);
    std::string s = std::string(pathtmp.mb_str());

    wxDir dir(s);
    wxString dirName = dir.GetName();
    wxArrayString dirList;
    dir.GetAllFiles(dirName, &dirList, wxEmptyString, wxDIR_FILES | wxDIR_DIRS);
    wxArrayString filteredDirList;
    wxArrayString clientDirList;
    wxRegEx reMaster(".*IMC.*");
    wxRegEx reClient(".*IDC.*");
        for (int i = 0; i < dirList.GetCount(); i++)
        {
            if (reMaster.Matches(dirList[i]))
            {
                wxString temp;
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
    m_lb3->Append(filteredDirList);
    m_lb6->Append(clientDirList);
    }
    
    for (int l = 0; l < items; l++)
    {
        for (int m = l+1; m < items; m++)
        {
            if (m_lb3->GetString(l) == m_lb3->GetString(m))
            {
                m_lb5->Append(m_lb3->GetString(l));
            }
        }
    }

    file.Open();
    file.Clear();
    wxArrayString list = m_lb->GetStrings();
    size_t count = list.Count();
    for (size_t i = 0; i < count; ++i)
        file.AddLine(list[i]);
    file.Write();
    file.Close();
}

void MainWindow::OnNew2(wxCommandEvent& event)
{
    wxDirDialog* openDirDialog = new wxDirDialog(this, "Choose Client directories");
    if (openDirDialog->ShowModal() == wxID_OK) {
        wxString DirName = openDirDialog->GetPath();
        m_lb2->Append(DirName);
        num_of_cdirs++;
    }
}

void MainWindow::OnClear2(wxCommandEvent& event)
{
    m_lb2->Clear();
    m_lb4->Clear();
    m_lb7->Clear();
    num_of_cdirs = 0;
}

void MainWindow::OnDelete2(wxCommandEvent& event)
{
    int sel = m_lb2->GetSelection();
    if (sel != -1) {
        m_lb2->Delete(sel);
        num_of_cdirs--;
    }
}


void MainWindow::OnSearch2(wxCommandEvent& event)
{
    m_lb4->Clear();
    m_lb7->Clear();
    int seldirs = 0;
    for (seldirs; seldirs < num_of_cdirs; seldirs++)
    {
        wxString pathtmp = m_lb2->GetString(seldirs);
        std::string s = std::string(pathtmp.mb_str());

        wxDir dir(s);
        wxString dirName = dir.GetName();
        wxArrayString dirList;
        dir.GetAllFiles(dirName, &dirList, wxEmptyString, wxDIR_FILES | wxDIR_DIRS);
        wxArrayString filteredDirList;
        wxArrayString masterDirList;
        wxRegEx reMaster(".*IMC.*");
        wxRegEx reClient(".*IDC.*");

        for (int i = 0; i < dirList.GetCount(); i++)
        {
            if (reClient.Matches(dirList[i]))
            {
                wxString temp;
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
        m_lb4->Append(filteredDirList);
        m_lb7->Append(masterDirList);
    }
    file2.Open();
    file2.Clear();
    wxArrayString list = m_lb2->GetStrings();
    size_t count = list.Count();
    for (size_t i = 0; i < count; ++i)
        file2.AddLine(list[i]);
    file2.Write();
    file2.Close();
}

void MainWindow::OnSync(wxCommandEvent& event)
{
    wxMessageBox("under development");
}

MainWindow::~MainWindow() {}