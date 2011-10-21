#ifndef _PSREGISTERVIEW_H
#define _PSREGISTERVIEW_H

#include "PsModelControl.h"

#include <QMainWindow>

class QTableWidget;

class PsRegisterView : public QMainWindow
{
Q_OBJECT
public:
    PsRegisterView(QWidget* parent=0, PsModelControl* modelControl=0);
    ~PsRegisterView();
    void updateRegisterDisplay();

private slots:
    void handleSimulationStopped();
    void handleSimulationReset();
    void handleModelChanged();

private:
    PsModelControl* mModelControl;
    QTableWidget*   mRegisterView;
};

#endif
