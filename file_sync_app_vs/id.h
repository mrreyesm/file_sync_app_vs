/////////////////////////////////////////////////////////////////////////////
// Name:        File Sync
// Purpose:     An application that Syncs Files
// Developed:   15.01.2022 @HTW Berlin under the Coordination of Tilo Wendler
/////////////////////////////////////////////////////////////////////////////
#pragma once
namespace window
{
    enum id
    {
        MAINWINDOW = wxID_HIGHEST + 1,
        ID_S_LISTBOX = 50,
        ID_S_ADD = 51,
        ID_S_REMOVE = 52,
        ID_S_CLEAR = 53,
        ID_S_SEARCH = 54,      
        ID_MF_LISTBOX = 6,
        ID_MDF_LISTBOX = 7,
        ID_SCF_LISTBOX = 91,
        ID_T_LISTBOX = 60,
        ID_T_ADD = 61,
        ID_T_REMOVE = 62,
        ID_T_CLEAR = 63,
        ID_T_SEARCH = 64,
        ID_CF_LISTBOX = 9,
        ID_SYNC = 13,
        ID_TMF_LISTBOX = 92,
        ID_SF_LISTBOX = 93,
        ID_IDFILE = 20,
        ID_IDADD = 21,
        ID_MASTERCHBOX = 22,
        ID_CLIENTCHBOX = 23
    };
}