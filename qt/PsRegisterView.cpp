#include "PsRegisterView.h"
#include <QtGui>

PsRegisterView::PsRegisterView(QWidget* parent, PsModelControl* modelControl)
: QMainWindow(parent)
, mModelControl(modelControl)
{
    this->setWindowFlags(Qt::Widget);
    this->menuBar()->addMenu(tr("&View"));

    mRegisterView = new QTableWidget(this);
    this->setCentralWidget(mRegisterView);

    mRegisterView->setColumnCount(2);
    mRegisterView->setSortingEnabled(false);
    mRegisterView->verticalHeader()->hide();
    mRegisterView->horizontalHeader()->hide();

    updateRegisterDisplay();

    connect(mModelControl, SIGNAL(simulationStopped()), this, SLOT(handleSimulationStopped()));
    connect(mModelControl, SIGNAL(simulationReset()), this, SLOT(handleSimulationReset()));
    connect(mModelControl, SIGNAL(modelChanged()), this, SLOT(handleModelChanged()));
}

PsRegisterView::~PsRegisterView()
{
}

// TODO: replace with a better name as this function does not update the
// registers but the representation of the model
void PsRegisterView::updateRegisterDisplay()
{
    mRegisterView->setRowCount(mModelControl->getRegisterCount());

    for (int i=0; i<mModelControl->getRegisterCount(); i++)
    {
        std::string registerName;
        mModelControl->getRegisterName(i, registerName);
        QTableWidgetItem *newItem = new QTableWidgetItem(registerName.c_str());
        mRegisterView->setItem(i, 0, newItem);
        QTableWidgetItem *dataChangeItem = new QTableWidgetItem("0");
        mRegisterView->setItem(i, 1, dataChangeItem);
    }

    mRegisterView->resizeColumnToContents(0);
    mRegisterView->resizeRowsToContents();
}

void PsRegisterView::handleSimulationStopped()
{
    for (int i=0; i<mModelControl->getRegisterCount(); i++)
    {
        int registerValue;
        if (mModelControl->getRegister(i, registerValue))
        {
            if (mModelControl->isRegisterDirty(i))
            {
                mRegisterView->item(i, 1)->setBackground(QBrush(Qt::yellow));
            } else
            {
                mRegisterView->item(i, 1)->setBackground(QBrush(Qt::white));
            }
            mRegisterView->item(i, 1)->setText(QString::number(registerValue));
        }
    }
}

void PsRegisterView::handleSimulationReset()
{
    // TODO: factor this code out into a dedicated function
    for (int i=0; i<mModelControl->getRegisterCount(); i++)
    {
        int registerValue;
        if (mModelControl->getRegister(i, registerValue))
        {
            if (mModelControl->isRegisterDirty(i))
            {
                mRegisterView->item(i, 1)->setBackground(QBrush(Qt::yellow));
            } else
            {
                mRegisterView->item(i, 1)->setBackground(QBrush(Qt::white));
            }
            mRegisterView->item(i, 1)->setText(QString::number(registerValue));
        }
    }
}

void PsRegisterView::handleModelChanged()
{
    updateRegisterDisplay();
}

#include "PsRegisterView.moc"
