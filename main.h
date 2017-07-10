#ifndef MAIN_H
#define MAIN_H

class MainFrame;
class EasyXML : public wxApp
{
    virtual bool OnInit();
    virtual int OnExit();
    MainFrame *frame;

    public:
        MainFrame *GetMainFrame();
        bool DeleteTempFiles();
        wxString GetAppPath();
};

#endif
