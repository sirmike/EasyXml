#include "pch.h"
#include "main.h"
#include "mainframe.h"
#include "options.h"

IMPLEMENT_APP(EasyXML)

bool EasyXML::OnInit()
{
    Options& options = Options::Instance();

    if(options.GetEmptyFlag())
    {
        DeleteTempFiles();
    }
    wxXmlResource::Get()->InitAllHandlers();
    wxXmlResource::Get()->Load(GetAppPath() + _T("xrc/EasyXml.xrc"));
    frame = new MainFrame(NULL);
    frame->Show(false);
    SetTopWindow(frame);

    return true;
}

int EasyXML::OnExit()
{
    return 0;
}

MainFrame* EasyXML::GetMainFrame()
{
    return frame;
}

bool EasyXML::DeleteTempFiles()
{
    bool result = true;
    wxString file = wxFindFirstFile(GetAppPath() + _T("tmp\\*.xml"));
    while (!file.empty())
    {
        if(!wxRemoveFile(file))
        {
            result = false;
        }
        file = wxFindNextFile();
    }
    return result;
}

wxString EasyXML::GetAppPath()
{
    return wxFileName::FileName(argv[0]).GetPath(wxPATH_GET_SEPARATOR | wxPATH_GET_VOLUME);
}
