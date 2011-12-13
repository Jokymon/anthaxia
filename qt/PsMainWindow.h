#ifndef _PSMAINWINDOW_H
#define _PSMAINWINDOW_H

#include "PsModelControl.h"
#include <QMainWindow>
#include "model/ModelControl.h"

class QAction;
class QLabel;
class QMenu;
class QTableWidgetItem;
class QToolBar;
class QWidget;

class PsMainWindow : public QMainWindow
{
Q_OBJECT

public:
    PsMainWindow(ModelControl& mc);
    virtual ~PsMainWindow();

private slots:
    void addMemoryViewDockWidget();
    void addRegisterViewDockWidget();
    void addConsoleDockWidget();
    void addModelTreeWidget();

    void handleSimulationStarted();
    void handleSimulationStopped();
    void handleLoadImage();
    void handleLoadProcessor();
    void handleShowPluginInfo();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createMainWidget();
    void createDockWidgets();

private:
    QWidget* mMainWidget;

    QMenu* mFileMenu;
    QMenu* mProcessorMenu;
    QMenu* mViewMenu;
    QMenu* mInfoMenu;

    QToolBar* mSimControlBar;
    QToolBar* mViewBar;

    QAction* mStartSimulationAction;
    QAction* mStepSimulationAction;
    QAction* mStopSimulationAction;
    QAction* mResetSimulationAction;
    QAction* mQuitAction;
    QAction* mLoadImageAction;

    QAction* mLoadProcessor;

    QAction* mAddMemViewAction;
    QAction* mAddConsoleAction;

    QAction* mShowPluginInfo;

    QLabel* mSimulationStatus;

    PsModelControl mModelControl;
};

#endif
