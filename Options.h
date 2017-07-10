#ifndef OPTIONS_H
#define OPTIONS_H
#include "main.h"
#include "Singleton.h"

DECLARE_APP(EasyXML)

class Options : public Singleton<Options>
{
    protected:
        Options(void);
    private:
        friend class Singleton<Options>;
        wxString app;
        wxString params;
        long hotkeyValue;
        bool ctrl, shift, alt, win;
        bool emptyFlag; // delete files on startup
        bool addRootFlag;
        bool startWithWindows;
        void Load();

        wxString GetOptionFullPath();
    public:
        ~Options(void);
        void Save(const wxString &app, const wxString &params, const bool &ctrl, const bool &alt, const bool &shift, const bool &win, const long &hotkeyValue, const bool &emptyFlag, const bool &addRoot, const bool &startWithWindows);
        const wxString& GetAppString();
        const wxString& GetParamsString();
        const bool& GetCtrl();
        const bool& GetAlt();
        const bool& GetShift();
        const bool& GetWin();
        const long& GetHotkeyValue();
        const bool& GetEmptyFlag();
        const bool& GetAddRootFlag();
        const bool& GetStartWithWindows();
};

#endif
