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

#include "PsMainWindow.h"
#include "PsMemView.h"
#include "PsPluginInfoDlg.h"
#include "PsProcChooserDlg.h"
#include "PsRegisterView.h"
#include "PsSimulationModel.h"
#include "Logging/Logging.h"
#include "Settings/Settings.h"
#include "model/SimulationObject.h"
#include "plugins/processor/ProcessorControl.h"
#include "console/qpluginconsole.h"

#include <QtGlobal>
#include <QtGui>
#include <iostream>

DEFINE_STATIC_LOGGER("gui.MainWindow");

class QDecHexValidator : public QValidator
{
public:
    QDecHexValidator(QObject* parent) : QValidator(parent) {}
    virtual ~QDecHexValidator() {}
    virtual State validate(QString& input, int& pos) const
    {
        bool ok;
        input.toInt(&ok, 10);
        if (ok)
            return Acceptable;
        if (input=="0x")
            return Intermediate;
        input.mid(2).toInt(&ok, 16);
        if (ok && input.startsWith("0x"))
            return Acceptable;
        return Invalid;
    }
};

PsMainWindow::PsMainWindow(ModelControl& mc)
: QMainWindow(0)
, mModelControl(mc)
{
    // TODO: load a particular processor model based on settings and/or users
    // selection
    mModelControl.loadModel(QString("MipsModel"));

    mMainWidget = new QWidget;
    mMainWidget->resize(0,0);
    setCentralWidget(mMainWidget);

    LOG_DEBUG("Creating Actions");
    createActions();
    LOG_DEBUG("Creating Menus");
    createMenus();
    LOG_DEBUG("Creating Toolbars");
    createToolBars();
    LOG_DEBUG("Creating Status bar");
    createStatusBar();
    LOG_DEBUG("Creating Main Widget");
    createMainWidget();
    LOG_DEBUG("Creating Dock Widgets");
    createDockWidgets();
    LOG_DEBUG("Creation done");

    setWindowTitle(QString().sprintf("Processor Simulation"));

    int posx = Settings::getInt("gui.mainwindow.posx", 20);
    int posy = Settings::getInt("gui.mainwindow.posy", 20);
    this->move(posx, posy);
    int width = Settings::getInt("gui.mainwindow.width", 600);
    int height = Settings::getInt("gui.mainwindow.height", 400);
    this->resize(QSize(width, height));
    if (Settings::hasKey("gui.mainwindow.geometry"))
    {
        QByteArray state(Settings::getString("gui.mainwindow.geometry").c_str());
        this->restoreState(
            QByteArray::fromBase64(state)
        );
    }
}

PsMainWindow::~PsMainWindow()
{
    Settings::setString("gui.mainwindow.geometry", 
        this->saveState().toBase64().data()
    );
    QPoint pos = this->pos();
    Settings::setInt("gui.mainwindow.posx", pos.x());
    Settings::setInt("gui.mainwindow.posy", pos.y());
    QSize size = this->size();
    Settings::setInt("gui.mainwindow.width", size.width());
    Settings::setInt("gui.mainwindow.height", size.height());

    // TODO: nicer cleanup of these resources
    delete mViewBar;
    delete mSimControlBar;

    delete mAddConsoleAction;
    delete mAddMemViewAction;
    delete mLoadImageAction;
    delete mStopSimulationAction;
    delete mStartSimulationAction;
    delete mResetSimulationAction;
}

void PsMainWindow::createActions()
{
    /* Simulation control actions */
    mStartSimulationAction = new QAction(QIcon("../icons/Play.png"), 
                                         tr("Start"), this);
    mStartSimulationAction->setStatusTip(tr("Start the simulation"));
    connect(mStartSimulationAction, SIGNAL(triggered()), &mModelControl, SLOT(startSimulation()));
    mStopSimulationAction = new QAction(QIcon("../icons/Stop.png"),
                                        tr("Stop"), this);
    mStopSimulationAction->setStatusTip(tr("Stop the simualtion"));
    connect(mStopSimulationAction, SIGNAL(triggered()), &mModelControl, SLOT(stopSimulation()));
    mStepSimulationAction = new QAction(QIcon("../icons/Step.png"),
                                        tr("Step"), this);
    mStepSimulationAction->setStatusTip(tr("Step the simulation"));
    connect(mStepSimulationAction, SIGNAL(triggered()), &mModelControl, SLOT(stepSimulation()));
    mResetSimulationAction = new QAction(QIcon("../icons/Reset.png"),
                                        tr("Reset"), this);
    mResetSimulationAction->setStatusTip(tr("Reset the simulation"));
    connect(mResetSimulationAction, SIGNAL(triggered()), &mModelControl, SLOT(resetSimulation()));
    // TODO: maybe connect simulation changes in a separate function
    connect(&mModelControl, SIGNAL(simulationStarted()), this, SLOT(handleSimulationStarted()));
    connect(&mModelControl, SIGNAL(simulationStopped()), this, SLOT(handleSimulationStopped()));

    /* File menu actions */
    mLoadImageAction = new QAction(QIcon("../icons/16-file-page.png"),
                                   tr("Load image..."), this);
    mLoadImageAction->setStatusTip(tr("Load a software iamge"));
    connect(mLoadImageAction, SIGNAL(triggered()), this, SLOT(handleLoadImage()));

    mQuitAction = new QAction(tr("Quit"), this);
    mQuitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
    connect(mQuitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    /* Processor menu actions */
    mLoadProcessor = new QAction(tr("Load Processor..."), this);
    mLoadProcessor->setStatusTip(tr("Load a processor model"));
    connect(mLoadProcessor, SIGNAL(triggered()), this, SLOT(handleLoadProcessor()));

    /* View menu actions */
    mAddMemViewAction = new QAction(QIcon("../icons/Document.png"),
                                    tr("Add Memory View"), this);
    mAddMemViewAction->setStatusTip(tr("Add a new memory view"));
    connect(mAddMemViewAction, SIGNAL(triggered()), this, SLOT(addMemoryViewDockWidget()));

    mAddConsoleAction = new QAction(QIcon("../icons/Terminal.png"),
                                    tr("Add Console"), this);
    mAddConsoleAction->setStatusTip(tr("Add a console window"));

    /* Info menu actions */
    mShowPluginInfo = new QAction(tr("Show plugin info"), this);
    connect(mShowPluginInfo, SIGNAL(triggered()), this, SLOT(handleShowPluginInfo()));
}

void PsMainWindow::createMenus()
{
    mFileMenu = menuBar()->addMenu(tr("&File"));
    mFileMenu->addAction(mLoadImageAction);
    mFileMenu->addSeparator();
    mFileMenu->addAction(mQuitAction);

    mProcessorMenu = menuBar()->addMenu(tr("&Processor"));
    mProcessorMenu->addAction(mLoadProcessor);

    mViewMenu = menuBar()->addMenu(tr("&View"));
    mViewMenu->addAction(mAddMemViewAction);
    mViewMenu->addAction(mAddConsoleAction);

    mInfoMenu = menuBar()->addMenu(tr("&Info"));
    mInfoMenu->addAction(mShowPluginInfo);
}

void PsMainWindow::createToolBars()
{
    mSimControlBar = addToolBar("Simulation Control");
    mSimControlBar->setObjectName("simulation_control");
    mSimControlBar->addAction(mStartSimulationAction);
    mSimControlBar->addAction(mStepSimulationAction);
    mSimControlBar->addAction(mStopSimulationAction);
    mSimControlBar->addAction(mResetSimulationAction);

    mViewBar = addToolBar("View");
    mViewBar->setObjectName("view_bar");
    mViewBar->addAction(mAddMemViewAction);
    mViewBar->addAction(mAddConsoleAction);
}

void PsMainWindow::createStatusBar()
{
    mSimulationStatus = new QLabel(tr("No simulation running"), this);

    statusBar()->showMessage(tr("Ready"));
    statusBar()->addPermanentWidget(mSimulationStatus);
}

void PsMainWindow::createMainWidget()
{
    Q_ASSERT(0!=mModelControl.getWordInterfaceByIndex(0));
    mMainWidget = new PsMemView(this, &mModelControl, mModelControl.getWordInterfaceByIndex(0), PsMemView::RootWindow);
    setCentralWidget(mMainWidget);
}

void PsMainWindow::createDockWidgets()
{
    addRegisterViewDockWidget();
    addConsoleDockWidget();
    addModelTreeWidget();
}

void PsMainWindow::addMemoryViewDockWidget()
{
    QDockWidget* memDockList = new QDockWidget(tr("Memory View"), this);
    // TODO: give a distinctive name for each memory view instance
    memDockList->setObjectName("memory_view");
    PsMemView*   memList     = new PsMemView(memDockList, &mModelControl, mModelControl.getWordInterfaceByIndex(0));
    memDockList->setWidget(memList);
    addDockWidget(Qt::TopDockWidgetArea, memDockList);
}

void PsMainWindow::addRegisterViewDockWidget()
{
    QDockWidget* registerDockView  = new QDockWidget(tr("Register View"), this);
    // TODO: give a distinctive name for each register view instance
    registerDockView->setObjectName("register_view");
    PsRegisterView* regView        = new PsRegisterView(registerDockView, &mModelControl);
    registerDockView->setWidget(regView);
    addDockWidget(Qt::RightDockWidgetArea, registerDockView);
}

void PsMainWindow::addConsoleDockWidget()
{
    QDockWidget* consoleDockWidget = new QDockWidget(tr("Console"), this);
    // TODO: give a distinctive name for each console instance
    consoleDockWidget->setObjectName("console_view");
    QPluginConsole* console = new QPluginConsole(consoleDockWidget);
    consoleDockWidget->setWidget(console);
    consoleDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDockWidget);
}

void PsMainWindow::addModelTreeWidget()
{
    QDockWidget* modelDockWidget = new QDockWidget(tr("Model Tree"), this);
    modelDockWidget->setObjectName("model_tree_view");

    QTreeView* qtv = new QTreeView(modelDockWidget);
    qtv->setHeaderHidden(true);
    PsSimulationModel* model = new PsSimulationModel;
    model->setRootObject( mModelControl.getProcessor() );
    qtv->setModel(model);

    modelDockWidget->setWidget(qtv);
    addDockWidget(Qt::LeftDockWidgetArea, modelDockWidget);
}

void PsMainWindow::handleSimulationStarted()
{
    mSimulationStatus->setText(tr("Simulation is running"));
}

void PsMainWindow::handleSimulationStopped()
{
    mSimulationStatus->setText(tr("No simulation running"));
}

void PsMainWindow::handleLoadImage()
{
    QString imageFile = QFileDialog::getOpenFileName(this,
        tr("Open Image"), ".", tr("Elf image (*.elf);;Intel HEX file (*.hex);;Plain binary image (*.bin);;All files (*.*)"));
    if (!imageFile.isEmpty())
    {
        mModelControl.handleLoadImage(imageFile);
    }
}

void PsMainWindow::handleLoadProcessor()
{
    PsProcChooserDlg dialog(0, mModelControl.getProcessorList());
    if (dialog.exec())
    {
        mModelControl.loadModel(dialog.selectedProcessor());
    }
}

void PsMainWindow::handleShowPluginInfo()
{
    PsPluginInfoDlg dialog(0);
    (void)dialog.exec();
}

#include "PsMainWindow.moc"
