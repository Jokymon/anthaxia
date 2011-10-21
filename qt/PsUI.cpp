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

void PsUI::install()
{
    mMainWindow = new PsMainWindow();
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
