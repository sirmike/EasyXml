#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include "TrayIcon.h"

static const int HotkeyID = 2000;

DECLARE_APP(EasyXML)

class MainFrame : public wxFrame
{
    private:
        TrayIcon *tray;
        bool hotkeyRegistered;
        void SetHotkey(bool ctrl, bool alt, bool shift, bool win, long hotkeyValue);
    public:
        MainFrame(wxWindow* parent);
        ~MainFrame();
        void RestoreFromIcon();
    protected:
        void HotkeyEvent(wxKeyEvent &ev);
        void OnBrowse(wxCommandEvent &ev);
        void OnMinimize(wxIconizeEvent &ev);
        void OnSaveOptions(wxCommandEvent &ev);
        void OnDeleteTempFiles(wxCommandEvent &ev);
        DECLARE_EVENT_TABLE();
};

#endif
