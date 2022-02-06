/////////////////////////////////////////////////////////////////////////////
// Name:        File Sync
// Purpose:     An application that Syncs Files
// Developed:   15.01.2022 @HTW Berlin under the Coordination of Tilo Wendler
/////////////////////////////////////////////////////////////////////////////
#include "App.h"
#include "id.h"
#include "MainWindow.h"

// "int main" is implemented as below in wxWidgets
wxIMPLEMENT_APP(App);

// To create apps with WxWidgets wxApp class should be derived
// and OnInit needs to be override
App::App() {}
// initializes the program 
bool App::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    //Creates an intance of the Main Window object and shows it
    MainWindow* main = new MainWindow(nullptr, window::id::MAINWINDOW, _("File Sync App"));
    main->Show();
    return true;
}

App::~App() {}