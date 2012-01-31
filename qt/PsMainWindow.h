/***********************************************************************
 *
 * This file is part of Anthaxia.
 * 
 * Anthaxia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Anthaxia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Anthaxia.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

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
