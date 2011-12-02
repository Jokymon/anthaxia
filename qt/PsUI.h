#ifndef _PSUI_H
#define _PSUI_H

#include <QObject>
#include <string>
#include "model/ModelControl.h"

class QApplication;
class PsMainWindow;

class PsUI : public QObject
{
Q_OBJECT

public:
    PsUI();
    ~PsUI();

    void parseArgs(int &argc, char **argv);

    void install(ModelControl& mc);
    void loop();
    void uninstall();

    std::string getApplicationDirPath();

private:
    QApplication* mApp;
    PsMainWindow* mMainWindow;

};

#endif
