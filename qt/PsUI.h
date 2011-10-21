#ifndef _PSUI_H
#define _PSUI_H

#include <QObject>
#include <string>

class QApplication;
class PsMainWindow;

class PsUI : public QObject
{
Q_OBJECT

public:
    PsUI();
    ~PsUI();

    void parseArgs(int &argc, char **argv);

    void install();
    void loop();
    void uninstall();

    std::string getApplicationDirPath();

private:
    QApplication* mApp;
    PsMainWindow* mMainWindow;

};

#endif
