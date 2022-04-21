/////////////////////////////////////////////////////////////////////////////
// Name:        File Sync
// Purpose:     An application that Syncs Files
// Developed:   15.01.2022 @HTW Berlin under the Coordination of T. Wendler
/////////////////////////////////////////////////////////////////////////////
#include "App.h"
#include "id.h"
#include "MainWindow.h"
#include <Windows.h>

void HideConsole()
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}
// "int main" is implemented as below in wxWidgets
wxIMPLEMENT_APP(App);
App::App() {}
// initializes the program
bool App::OnInit()
{
    if (!wxApp::OnInit())
        return false;
    HideConsole();
    //Creates an instance of the Main Window object and shows it
    MainWindow* main = new MainWindow(nullptr, window::id::MAINWINDOW, _("File Sync App"));
    main->Show(); 
    return true;
}
App::~App() {}