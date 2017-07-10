#include "pch.h"
#include "TrayIcon.h"
#include "MainFrame.h"

TrayIcon::TrayIcon()
{
}

TrayIcon::~TrayIcon()
{
}

void TrayIcon::OnMenuRestore(wxCommandEvent& )
{
    wxGetApp().GetMainFrame()->RestoreFromIcon();
}

void TrayIcon::OnMenuExit(wxCommandEvent& )
{
    wxGetApp().Exit();
}

void TrayIcon::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
    wxGetApp().GetMainFrame()->RestoreFromIcon();
}

wxMenu* TrayIcon::CreatePopupMenu()
{
    wxMenu *menu = new wxMenu();
    menu->Append(PU_RESTORE, _T("Restore"));
    menu->Append(PU_EXIT, _T("Exit"));
    return menu;
}

BEGIN_EVENT_TABLE(TrayIcon, wxTaskBarIcon)
    EVT_MENU(PU_RESTORE, TrayIcon::OnMenuRestore)
    EVT_MENU(PU_EXIT, TrayIcon::OnMenuExit)
    EVT_TASKBAR_LEFT_DCLICK(TrayIcon::OnLeftButtonDClick)
END_EVENT_TABLE()
