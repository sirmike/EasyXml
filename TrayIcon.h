#ifndef TRAYICON_H
#define TRAYICON_H

#include "main.h"

DECLARE_APP(EasyXML)

class TrayIcon : public wxTaskBarIcon
{
    public:
        TrayIcon();
        ~TrayIcon();

        void OnLeftButtonDClick(wxTaskBarIconEvent&);
        void OnMenuRestore(wxCommandEvent&);
        void OnMenuExit(wxCommandEvent&);

        virtual wxMenu* CreatePopupMenu();

        DECLARE_EVENT_TABLE()
};

enum {
    PU_RESTORE = 20001,
    PU_EXIT,
};

#endif
