#include "PsUI.h"
#include "PsMainWindow.h"
#include <QApplication>

PsUI::PsUI()
: QObject()
, mMainWindow(0)
{ }

PsUI::~PsUI()
{ }

void PsUI::parseArgs(int &argc, char **argv)
{
    mApp = new QApplication(argc, argv);
}

// TODO: Passing the Modelcontrol down two levels here is rather ugly, I think
// we can move the UI code into the main application or an application class
void PsUI::install(ModelControl& mc)
{
    mMainWindow = new PsMainWindow(mc);
    mMainWindow->show();
}

void PsUI::loop()
{
    mApp->exec();
}

void PsUI::uninstall()
{
    delete mMainWindow;
    mMainWindow = 0;

    delete mApp;
    mApp = 0;
}

std::string PsUI::getApplicationDirPath()
{
    Q_ASSERT(0!=mApp);
    return mApp->applicationDirPath().toStdString();
}

#include "PsUI.moc"
