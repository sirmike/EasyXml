#include "pch.h"
#include "HotkeyCtrl.h"

HotkeyCtrl::HotkeyCtrl(wxWindow *parent) : wxTextCtrl(parent, -1, wxEmptyString, wxDefaultPosition, wxSize(30, -1))
{
    SetMaxLength(1);
    hotkeyValue = 0;
}

HotkeyCtrl::~HotkeyCtrl(void)
{
}

void HotkeyCtrl::OnChar(wxKeyEvent& ev)
{
    long keyCode = ev.GetKeyCode();
    if((keyCode >= 'A' && keyCode <= 'Z') || (keyCode >= WXK_F1 && keyCode <= WXK_F12))
    {
        SetValue(TranslateKey(keyCode));
        hotkeyValue = keyCode;
    }
}

wxString HotkeyCtrl::TranslateKey(long key)
{
    wxString result = _T("");
    switch(key)
    {
        case WXK_F1:
            result = _T("F1");
            break;
        case WXK_F2:
            result = _T("F2");
            break;
        case WXK_F3:
            result = _T("F3");
            break;
        case WXK_F4:
            result = _T("F4");
            break;
        case WXK_F5:
            result = _T("F5");
            break;
        case WXK_F6:
            result = _T("F6");
            break;
        case WXK_F7:
            result = _T("F7");
            break;
        case WXK_F8:
            result = _T("F8");
            break;
        case WXK_F9:
            result = _T("F9");
            break;
        case WXK_F10:
            result = _T("F10");
            break;
        case WXK_F11:
            result = _T("F11");
            break;
        case WXK_F12:
            result = _T("F12");
            break;
        default:
            result = wxString(wxChar(key));
            break;
    }
    return result;
}

long HotkeyCtrl::GetHotkeyValue()
{
    return hotkeyValue;
}

void HotkeyCtrl::SetHotkeyValue(long hotkeyValue)
{
    SetValue(TranslateKey(hotkeyValue));
    this->hotkeyValue = hotkeyValue;
}

BEGIN_EVENT_TABLE(HotkeyCtrl, wxTextCtrl)
    EVT_KEY_DOWN(HotkeyCtrl::OnChar)
END_EVENT_TABLE()
