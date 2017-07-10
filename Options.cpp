#include "pch.h"
#include "Options.h"

static const wxChar *OptionFile = _T("options.cfg");

Options::Options(void)
{
    app = params = _T("");
    ctrl = shift = alt = win = false;
    Load();
}

Options::~Options(void)
{
}

void Options::Load()
{
    wxFile file(GetOptionFullPath(), wxFile::read);
    if(file.IsOpened())
    {
        size_t size = 0;
        file.Read((void*)&size, sizeof(size));
        char *buffer = new char[size + 1];
        size_t x = file.Read((void*)buffer, size);
        buffer[size] = '\0';
        app = wxString(buffer, wxConvUTF8);
        delete buffer;

        file.Read((void*)&size, sizeof(size));
        buffer = new char[size + 1];
        file.Read((void*)buffer, size);
        buffer[size] = '\0';
        params = wxString(buffer, wxConvUTF8);
        delete buffer;

        file.Read((void*)&ctrl, sizeof(ctrl));
        file.Read((void*)&alt, sizeof(alt));
        file.Read((void*)&shift, sizeof(shift));
        file.Read((void*)&win, sizeof(win));
        file.Read((void*)&hotkeyValue, sizeof(hotkeyValue));
        file.Read((void*)&emptyFlag, sizeof(emptyFlag));
        file.Read((void*)&addRootFlag, sizeof(addRootFlag));
        file.Read((void*)&startWithWindows, sizeof(startWithWindows));
        file.Close();
    }
}

void Options::Save(const wxString &app, const wxString &params, const bool &ctrl, const bool &alt, const bool &shift, const bool &win, const long &hotkeyValue, const bool &emptyFlag, const bool &addRootFlag, const bool &startWithWindows)
{
    this->app = app;
    this->params = params;
    this->ctrl = ctrl;
    this->alt = alt;
    this->shift = shift;
    this->win = win;
    this->hotkeyValue = hotkeyValue;
    this->emptyFlag = emptyFlag;
    wxFile file(GetOptionFullPath(), wxFile::write);
    if(file.IsOpened())
    {
        size_t size = app.Length();
        file.Write((void*)&size, sizeof(size));
        file.Write(app);
        size = params.Length();
        file.Write((void*)&size, sizeof(size));
        file.Write(params);
        file.Write((void*)&ctrl, sizeof(ctrl));
        file.Write((void*)&alt, sizeof(alt));
        file.Write((void*)&shift, sizeof(shift));
        file.Write((void*)&win, sizeof(win));
        file.Write((void*)&hotkeyValue, sizeof(hotkeyValue));
        file.Write((void*)&emptyFlag, sizeof(emptyFlag));
        file.Write((void*)&addRootFlag, sizeof(addRootFlag));
        file.Write((void*)&startWithWindows, sizeof(startWithWindows));
        file.Close();
    }
}

const wxString& Options::GetAppString()
{
    return app;
}

const wxString& Options::GetParamsString()
{
    return params;
}

const bool& Options::GetCtrl()
{
    return ctrl;
}

const bool& Options::GetAlt()
{
    return alt;
}

const bool& Options::GetShift()
{
    return shift;
}

const bool& Options::GetWin()
{
    return win;
}

const long& Options::GetHotkeyValue()
{
    return hotkeyValue;
}

const bool& Options::GetEmptyFlag()
{
    return emptyFlag;
}

const bool& Options::GetAddRootFlag()
{
    return addRootFlag;
}

const bool& Options::GetStartWithWindows()
{
    return startWithWindows;
}

wxString Options::GetOptionFullPath()
{
    return wxGetApp().GetAppPath() + OptionFile;
}
