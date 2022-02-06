/////////////////////////////////////////////////////////////////////////////
// Name:        File Sync
// Purpose:     An application that Syncs Files
// Developed:   15.01.2022 @HTW Berlin under the Coordination of Tilo Wendler
/////////////////////////////////////////////////////////////////////////////
#include "MainWindow.h"
#include "IdDialog.h"
#include <wx/artprov.h>

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
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* panel = new wxPanel(this);
    mainSizer->Add(panel, 1, wxEXPAND);
    wxBoxSizer* panelmainSizer = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(panelmainSizer);

    //Create Source file sizer, label and box
    wxBoxSizer* sourceSizer = new wxBoxSizer(wxHORIZONTAL);
    // Create source label
    wxStaticText* sourceLabel = new wxStaticText(panel, wxID_ANY, _("Source"));
    sourceSizer->Add(sourceLabel, 0, wxLEFT, 5);
    sourceLabel->SetMinSize(wxSize(125, sourceLabel->GetMinSize().y));
    // Create source box
    sourceBox = new wxTextCtrl(panel, wxID_ANY);
    sourceSizer->Add(sourceBox, 1);
    panelmainSizer->Add(sourceSizer, 0, wxEXPAND | wxALL, 5);
    // Add button to access files
    wxButton* okButton = new wxButton(panel, wxID_OK, _("Add"));
    sourceSizer->Add(okButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    // Look at files inside the directory
    sourceFileBox = new wxTextCtrl(panel, wxID_ANY);
    sourceSizer->Add(sourceFileBox, 1);

    // Create box for Master files
    wxBoxSizer* sourceMFileSizer = new wxBoxSizer(wxHORIZONTAL);
    sourceMFileBox = new wxTextCtrl(panel, wxID_ANY);
    sourceMFileSizer->Add(sourceMFileBox, 1);
    panelmainSizer->Add(sourceMFileSizer, 0, wxEXPAND | wxALL, 5);


    wxBoxSizer* targetSizer = new wxBoxSizer(wxHORIZONTAL);
    // Create target label
    wxStaticText* targetLabel = new wxStaticText(panel, wxID_ANY, _("Target"));
    targetSizer->Add(targetLabel, 0, wxLEFT, 5);
    targetLabel->SetMinSize(wxSize(125, targetLabel->GetMinSize().y));
    // Create target box
    targetBox = new wxTextCtrl(panel, wxID_ANY);
    targetSizer->Add(targetBox, 1);
    panelmainSizer->Add(targetSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    // Add button to access files
    wxButton* cancelButton = new wxButton(panel, wxID_CANCEL, _("Add"));
    targetSizer->Add(cancelButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    // Look at files inside the directory
    targetFileBox = new wxTextCtrl(panel, wxID_ANY);
    targetSizer->Add(targetFileBox, 1);

    // Create box for Client files
    wxBoxSizer* targetCFileSizer = new wxBoxSizer(wxHORIZONTAL);
    targetCFileBox = new wxTextCtrl(panel, wxID_ANY);
    targetCFileSizer->Add(targetCFileBox, 1);
    panelmainSizer->Add(targetCFileSizer, 0, wxEXPAND | wxALL, 5);

    // Create button to sync 
    wxBoxSizer* syncSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* syncButton = new wxButton(panel, wxID_ANY, _("Search"));
    syncSizer->Add(syncButton, 1);
    panelmainSizer->Add(syncSizer, 0, wxEXPAND | wxALL, 5);



    //Creates Status bar that can be updated in different steps
    CreateStatusBar();
    SetStatusText(_("Syncing!"));

    SetMinSize(wxSize(500, 250));
}

void MainWindow::onIdFile(wxCommandEvent& WXUNUSED(event))
{
    //IdDialog dlg(this, wxID_ANY, _("Address"));
    if (idDialog == nullptr)
    {
        idDialog = new IdDialog(this, wxID_ANY, _("ID File"));
        idDialog->Show();
    }
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is an Application to Sync Files and avoid duplicates.",
        "About File Sync", wxOK | wxICON_INFORMATION);
}

MainWindow::~MainWindow() {}