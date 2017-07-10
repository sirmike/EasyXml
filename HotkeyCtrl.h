#ifndef HOTKEY_H
#define HOTKEY_H

class HotkeyCtrl : public wxTextCtrl
{
    private:
        wxString TranslateKey(long key);
        long hotkeyValue;
    public:
        HotkeyCtrl(wxWindow *parent);
        ~HotkeyCtrl(void);
        long GetHotkeyValue();
        void SetHotkeyValue(long hotkeyValue);
    protected:
        void OnChar(wxKeyEvent& ev);
        DECLARE_EVENT_TABLE()
};

#endif
