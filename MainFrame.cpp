#include "pch.h"
#include "MainFrame.h"
#include "Options.h"
#include "HotkeyCtrl.h"
#include "main.h"

MainFrame::MainFrame(wxWindow* parent)
{
    // load resource file
    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxXmlResource::Get()->LoadFrame(this, parent, _T("MainFrame"));

    // set default background color
    this->SetBackgroundColour(wxColor());

    // load options from file
    Options &options = Options::Instance();

    XRCCTRL(*this, "appText", wxTextCtrl)->SetValue(options.GetAppString());
    XRCCTRL(*this, "parametersText", wxTextCtrl)->SetValue(options.GetParamsString());
    XRCCTRL(*this, "ctrlCheckBox", wxCheckBox)->SetValue(options.GetCtrl());
    XRCCTRL(*this, "altCheckBox", wxCheckBox)->SetValue(options.GetAlt());
    XRCCTRL(*this, "shiftCheckBox", wxCheckBox)->SetValue(options.GetShift());
    XRCCTRL(*this, "winCheckBox", wxCheckBox)->SetValue(options.GetWin());
    XRCCTRL(*this, "emptyFlag", wxCheckBox)->SetValue(options.GetEmptyFlag());
    XRCCTRL(*this, "addRoot", wxCheckBox)->SetValue(options.GetAddRootFlag());
    XRCCTRL(*this, "runWindowsStartCB", wxCheckBox)->SetValue(options.GetStartWithWindows());

    HotkeyCtrl* hotkeyCtrl = new HotkeyCtrl(this);
    bool succeeded = wxXmlResource::Get()->AttachUnknownControl(_T("hotkeyText"), hotkeyCtrl);

    XRCCTRL(*this, "hotkeyText", HotkeyCtrl)->SetHotkeyValue(options.GetHotkeyValue());

    // register hotkey
    hotkeyRegistered = false;
    SetHotkey(options.GetCtrl(), options.GetAlt(), options.GetShift(), options.GetWin(), options.GetHotkeyValue());

    // set app icon
    wxIcon iconBig = wxIcon(_T("APP_ICON_SMALL"));
    SetIcon(iconBig);

    // set tray icon
    wxIcon icon = wxIcon(_T("APP_ICON_SMALL"));
    tray = new TrayIcon();
    tray->SetIcon(icon, _T("EasyXML"));
}

MainFrame::~MainFrame()
{
    if(hotkeyRegistered)
    {
        if(!UnregisterHotKey(HotkeyID))
        {
            wxMessageDialog(this, _T("Could not unregister global hotkey"), _T("Warning")).ShowModal();
        }
        else
        {
            hotkeyRegistered = false;
        }
    }
    if (tray != NULL)
    {
        delete tray;
    }
}

void MainFrame::HotkeyEvent(wxKeyEvent &ev)
{
    // get active window handle
    DWORD currentThreadID = GetCurrentThreadId();
    DWORD fgWindowThreadID = 0;
    HWND fgHwnd = GetForegroundWindow();
    fgWindowThreadID = GetWindowThreadProcessId(fgHwnd, NULL);

    // attach input to active window
    if(currentThreadID != fgWindowThreadID)
    {
        AttachThreadInput(fgWindowThreadID, currentThreadID, TRUE);
    }

    // Send CTRL + C to active window
    INPUT input;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwFlags = 0;
    input.ki.dwExtraInfo = 0;
    input.type = INPUT_KEYBOARD;

    // Press Ctrl Key
    input.ki.wVk = VK_CONTROL;
    SendInput(1, &input, sizeof(input));

    // Press C Key
    input.ki.wVk = VkKeyScan('c');
    SendInput(1, &input, sizeof(input));

    // Release Ctrl Key
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    input.ki.wVk = VkKeyScan('c');
    SendInput(1, &input, sizeof(input));

    // Release C Key
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    input.ki.wVk = VK_CONTROL;
    SendInput(1, &input, sizeof(input));

    // detach input from active window
    if(currentThreadID != fgWindowThreadID)
    {
        AttachThreadInput(fgWindowThreadID, currentThreadID, FALSE);
    }

    wxMilliSleep(100);

    // save clipboard to temp file
    wxString filePath = wxGetApp().GetAppPath() + wxString::Format(_T("tmp\\%d.xml"), wxDateTime::Now().GetTicks());
    wxTextFile file(filePath);

    // check if root flag need to be added
    Options &options = Options::Instance();

    if(options.GetAddRootFlag())
    {
        file.AddLine(_T("<root>"));
    }
    if (wxTheClipboard->Open())
    {
        if (wxTheClipboard->IsSupported(wxDF_TEXT))
        {
            wxTextDataObject data;
            if(wxTheClipboard->GetData(data))
            {
                file.AddLine(data.GetText());
            }
        }
        wxTheClipboard->Close();
    }
    if(options.GetAddRootFlag())
    {
        file.AddLine(_T("</root>"));
    }

    file.Write();
    file.Close();

    wxString parameters = XRCCTRL(*this, "parametersText", wxTextCtrl)->GetValue();
    parameters.Replace(_T("%s"), filePath);
    wxExecute(XRCCTRL(*this, "appText", wxTextCtrl)->GetValue() + _T(" ") + parameters); 
}

void MainFrame::OnBrowse(wxCommandEvent &ev)
{
    wxString fileName = wxFileSelector(_T("Choose a file to open"));
    if(!fileName.empty())
    {
        wxTextCtrl *appText = XRCCTRL(*this, "appText", wxTextCtrl);
        appText->SetValue(fileName);
    }
}

void MainFrame::OnMinimize(wxIconizeEvent &ev)
{
    Show(false);
}

void MainFrame::RestoreFromIcon()
{
    Show();
    Restore();
}

void MainFrame::OnSaveOptions(wxCommandEvent &ev)
{
    bool ctrl = XRCCTRL(*this, "ctrlCheckBox", wxCheckBox)->GetValue();
    bool alt = XRCCTRL(*this, "altCheckBox", wxCheckBox)->GetValue();
    bool shift = XRCCTRL(*this, "shiftCheckBox", wxCheckBox)->GetValue();
    bool win = XRCCTRL(*this, "winCheckBox", wxCheckBox)->GetValue();
    bool empty = XRCCTRL(*this, "emptyFlag", wxCheckBox)->GetValue();
    bool addRoot = XRCCTRL(*this, "addRoot", wxCheckBox)->GetValue();
    bool startWithWindows = XRCCTRL(*this, "runWindowsStartCB", wxCheckBox)->GetValue();
    long hotkeyValue = XRCCTRL(*this, "hotkeyText", HotkeyCtrl)->GetHotkeyValue();

    Options::Instance().Save(XRCCTRL(*this, "appText", wxTextCtrl)->GetValue(),
            XRCCTRL(*this, "parametersText", wxTextCtrl)->GetValue(),
            ctrl, alt, shift, win, hotkeyValue, empty, addRoot, startWithWindows);

    //set hotkey
    SetHotkey(ctrl, alt, shift, win, hotkeyValue);

    // set registry to startup app with windows
    wxRegKey *regKey = new wxRegKey(_T("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"));
    const wxString value = _T("EasyXml");
    if(regKey->Exists())
    {
        if(startWithWindows)
        {
            regKey->SetValue(value, wxGetApp().argv[0]);
        }
        else
        {
            regKey->DeleteValue(value);
        }
    }

    Iconize(true);
}

void MainFrame::SetHotkey(bool ctrl, bool alt, bool shift, bool win, long hotkeyValue)
{
    if((ctrl && (static_cast<char>(hotkeyValue) == 'C')) || hotkeyValue == 0)
    {
        wxMessageDialog(this, _T("Your hotkey is forbidden"), _T("Warning")).ShowModal();
    }
    else
    {
        UnregisterHotKey(HotkeyID);

        int modifier = 0;
        if(ctrl)
        {
            modifier |= wxMOD_CONTROL;
        }
        if(alt)
        {
            modifier |= wxMOD_ALT;
        }
        if(shift)
        {
            modifier |= wxMOD_SHIFT;
        }
        if(win)
        {
            modifier |= wxMOD_WIN;
        }
        if(modifier != 0)
        {
            switch(hotkeyValue)
            {
                case WXK_F1:
                    hotkeyValue = VK_F1;
                    break;
                case WXK_F2:
                    hotkeyValue = VK_F2;
                    break;
                case WXK_F3:
                    hotkeyValue = VK_F3;
                    break;
                case WXK_F4:
                    hotkeyValue = VK_F4;
                    break;
                case WXK_F5:
                    hotkeyValue = VK_F5;
                    break;
                case WXK_F6:
                    hotkeyValue = VK_F6;
                    break;
                case WXK_F7:
                    hotkeyValue = VK_F7;
                    break;
                case WXK_F8:
                    hotkeyValue = VK_F8;
                    break;
                case WXK_F9:
                    hotkeyValue = VK_F9;
                    break;
                case WXK_F10:
                    hotkeyValue = VK_F10;
                    break;
                case WXK_F11:
                    hotkeyValue = VK_F11;
                    break;
                case WXK_F12:
                    hotkeyValue = VK_F12;
                    break;
                default:
                    break;
            }
            if(!RegisterHotKey(HotkeyID, modifier, static_cast<int>(hotkeyValue)))
            {
                wxMessageDialog(this, _T("Could not register global hotkey"), _T("Warning")).ShowModal();
            }
            else
            {
                hotkeyRegistered = true;
            }
        }
    }
}

void MainFrame::OnDeleteTempFiles(wxCommandEvent &ev)
{
    if(!wxGetApp().DeleteTempFiles())
    {
        wxMessageDialog(this, _T("Couldn't delete all temporary files"), _T("Warning"), wxOK | wxICON_EXCLAMATION).ShowModal();
    }
    else
    {
        wxMessageDialog(this, _T("All temporary files have been deleted"), _T("Info"), wxOK | wxICON_INFORMATION).ShowModal();
    }
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_HOTKEY(HotkeyID, MainFrame::HotkeyEvent)
    EVT_BUTTON(XRCID("browseButton"), MainFrame::OnBrowse)
    EVT_BUTTON(XRCID("saveButton"), MainFrame::OnSaveOptions)
    EVT_BUTTON(XRCID("deleteNowButton"), MainFrame::OnDeleteTempFiles)
    EVT_ICONIZE(MainFrame::OnMinimize)
END_EVENT_TABLE()
