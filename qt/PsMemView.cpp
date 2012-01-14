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

#include "PsMemView.h"

#include "PsMemoryObserver.h"
#include "PsModelControl.h"
#include "Logging/Logging.h"
#include "plugins/processor/MemoryInterface.h"
// Todolist
// TODO: can we avoid using QFontMetrics distributed all over the code

#include <QAction>
#include <QFont>
#include <QMenu>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QtGlobal>
#include <iostream>

DEFINE_STATIC_LOGGER("gui.MemView");

PsMemView::PsMemView(QWidget* parent, PsModelControl* modelControl, MemoryWordInterface* wordInterface, EWindowMode windowMode)
: QScrollArea(parent)
, mViewMode(eVMDisassembly)
, mFollowMode(eFMStay)
, mWindowMode(windowMode)
, mModelControl(modelControl)
, mWordInterface(wordInterface)
, mTopAddress(0)
, mSelectedAddress(0)
, mContextMenu(new QMenu(this))
, mMainFont(new QFont("Courier", 12))
{ 
    Q_ASSERT(0!=wordInterface);
    verticalScrollBar()->setMinimum(0);
    verticalScrollBar()->setMaximum(65535);
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(valueChanged(int)));

    LOG_DEBUG("Creating a memory observer");
    mMemObserver = new PsMemoryObserver(this);
    LOG_DEBUG("Registering observer with memory interface");
    mWordInterface->registerMemoryObserver(mMemObserver);
    LOG_DEBUG("Connecting 'changed' signal to MemoryView");
    connect(mMemObserver, SIGNAL(memoryChanged()), this, SLOT(handleMemoryChanged()));
    LOG_DEBUG("Connecting done");

    connect(mModelControl, SIGNAL(modelChanged()), this, SLOT(handleModelChanged()));

    mSetHexView = new QAction(tr("Hex View"), this);
    mSetHexView->setCheckable(true);
    connect(mSetHexView, SIGNAL(triggered()), this, SLOT(handleSetHexView()));
    mSetDisassemblyView = new QAction(tr("Disassembly View"), this);
    mSetDisassemblyView->setCheckable(true);
    mSetDisassemblyView->setChecked(true);
    connect(mSetDisassemblyView, SIGNAL(triggered()), this, SLOT(handleSetDisassemblyView()));
    mSetFollowMode = new QAction(tr("Follow PC"), this);
    mSetFollowMode->setCheckable(true);
    connect(mSetFollowMode, SIGNAL(triggered()), this, SLOT(handleSetFollowMode()));
    mCenterAroundPC = new QAction(tr("Center around PC"), this);
    connect(mCenterAroundPC, SIGNAL(triggered()), this, SLOT(handleCenterAroundPC()));

    mContextMenu->addAction(mSetHexView);
    mContextMenu->addAction(mSetDisassemblyView);
    mContextMenu->addAction(mSetFollowMode);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCenterAroundPC);
}

PsMemView::~PsMemView()
{ 
    delete mContextMenu;
    delete mMainFont;
}

void PsMemView::mouseReleaseEvent(QMouseEvent *me)
{
    if (me->button() == Qt::LeftButton)
    {
        QFontMetrics fm(*mMainFont);
        mSelectedAddress = mTopAddress + me->y()/fm.height();
        viewport()->update();
    }
}

void PsMemView::paintEvent(QPaintEvent *pe)
{
    switch (mViewMode)
    {
        case eVMDisassembly:
            paintDisassembly(pe->rect());
            break;
        case eVMHexView:
            paintHexView(pe->rect());
            break;
        default:
            paintDisassembly(pe->rect());
            break;
    }
}

void PsMemView::paintDisassembly(const QRect& rect)
{
    QPainter painter(viewport());

    painter.setFont(*mMainFont);
    QFontMetrics fm(*mMainFont);
    int noOfLines = (this->height()/fm.height())+1;
    for (int line=0; line<noOfLines; line++)
    {
        paintDisassemblyLine(&painter, line);
    }
}

void PsMemView::paintHexView(const QRect& rect)
{
    QPainter painter(viewport());

    painter.setFont(*mMainFont);
    QFontMetrics fm(*mMainFont);
    int noOfLines = (this->height()/fm.height())+1;
    for (int line=0; line<noOfLines; line++)
    {
        paintHexViewLine(&painter, line);
    }
}

void PsMemView::paintDisassemblyLine(QPainter* painter, int line)
{
    int address = mTopAddress + line;
    int width = size().width();

    QFontMetrics fm = painter->fontMetrics();
    int lineHeight = fm.height();

    int top = lineHeight*line;
    int bottom = lineHeight*(line+1);
    QColor color = Qt::white;
    if (address==mSelectedAddress)
        color = Qt::gray;
    painter->fillRect(0, top, width, bottom, color);
    painter->setPen(Qt::black);

    unsigned int machineCode = 0;
    QString disassembly = "Could not read memory location";
    if (mWordInterface->getWord(address*4, machineCode))
        disassembly = mModelControl->disassembleInstruction(address*4);

    painter->drawText(QPoint(15, bottom-4), QString("%1: %2  %3").arg((uint)address*4, 8, 16, QLatin1Char('0')).arg((uint)machineCode, 8, 16, QLatin1Char('0')).arg(disassembly));
    painter->setRenderHint(QPainter::Antialiasing);
    if ((address%10)==0)
    {
        QPolygon polygon;
        polygon
         << QPoint(0, top)
         << QPoint(20, top)
         << QPoint(20, top+1)
         << QPoint(0, top+1);
        painter->drawPolygon(polygon);
    }
    // Draw a marker for the PC
    if (4*address==mModelControl->getPC())
    {
        painter->setPen(Qt::darkGreen);
        painter->setBrush(QBrush(Qt::green));
        QPolygon pcMarker;
        pcMarker <<
            QPoint(1, top+1) <<
            QPoint(12, top+lineHeight/2) <<
            QPoint(1, bottom-2);
        painter->drawPolygon(pcMarker);
    }
}

void PsMemView::paintHexViewLine(QPainter* painter, int line)
{
    int address = mTopAddress + line * 4;
    int width = size().width();

    QFontMetrics fm = painter->fontMetrics();
    int lineHeight = fm.height();

    int top = lineHeight*line;
    int bottom = lineHeight*(line+1);
    QColor color = Qt::white;
    if (address==mSelectedAddress)
        color = Qt::gray;
    painter->fillRect(0, top, width, bottom, color);
    painter->setPen(Qt::black);

    QString output_line = QString("%1: ").arg((uint)address*4, 8, 16, QLatin1Char('0'));
    for (int i=0; i<4; i++)
    {
        unsigned int machineCode = 0;
        mWordInterface->getWord(address*4, machineCode);
        output_line += QString("%1 ").arg((uint)machineCode, 8, 16, QLatin1Char('0'));
    }

    painter->drawText(QPoint(15, bottom-4), output_line);
    painter->setRenderHint(QPainter::Antialiasing);
    if ((address%10)==0)
    {
        QPolygon polygon;
        polygon
         << QPoint(0, top)
         << QPoint(20, top)
         << QPoint(20, top+1)
         << QPoint(0, top+1);
        painter->drawPolygon(polygon);
    }
    // Draw a marker for the PC
    if (address==mModelControl->getPC())
    {
        painter->setPen(Qt::darkGreen);
        painter->setBrush(QBrush(Qt::green));
        QPolygon pcMarker;
        pcMarker <<
            QPoint(1, top+1) <<
            QPoint(12, top+lineHeight/2) <<
            QPoint(1, bottom-2);
        painter->drawPolygon(pcMarker);
    }
}

void PsMemView::contextMenuEvent(QContextMenuEvent* event)
{
    mContextMenu->exec(event->globalPos());
}

void PsMemView::valueChanged(int value)
{
    mTopAddress = value;
    viewport()->update();
}

void PsMemView::handleCenterAroundPC()
{
    QFontMetrics fm(*mMainFont);
    int noOfLines = (this->height()/fm.height())+1;
    this->verticalScrollBar()->setValue(mModelControl->getPC()/4 - noOfLines/2);
}

void PsMemView::handleModelChanged()
{
    LOG_DEBUG("handleModelChanged");
    disconnect(mMemObserver, SIGNAL(memoryChanged()), this, SLOT(handleMemoryChanged()));
    LOG_DEBUG("Removing the old memory observer");
    delete mMemObserver;
    delete mWordInterface;

    if (mWindowMode==RootWindow)
    {
        LOG_DEBUG("This is a root window so create a new memory observer");
        mWordInterface = mModelControl->getWordInterfaceByIndex(0);
        mMemObserver = new PsMemoryObserver(this);
        mWordInterface->registerMemoryObserver(mMemObserver);
        LOG_DEBUG("Connect to the new memory observer with this window");
        connect(mMemObserver, SIGNAL(memoryChanged()), this, SLOT(handleMemoryChanged()));
    } else {
        // TODO: kill this window
    }
}

void PsMemView::handleMemoryChanged()
{
    LOG_DEBUG("handleMemoryChanged");
    viewport()->update();
    if (eFMFollowPC == mFollowMode)
    {
        this->handleCenterAroundPC();
    }
}

void PsMemView::handleSetDisassemblyView()
{
    mViewMode = eVMDisassembly;
    mSetHexView->setChecked(false);
    mSetDisassemblyView->setChecked(true);
    viewport()->update();
}

void PsMemView::handleSetHexView()
{
    mViewMode = eVMHexView;
    mSetHexView->setChecked(true);
    mSetDisassemblyView->setChecked(false);
    viewport()->update();
}

void PsMemView::handleSetFollowMode()
{
    if (mFollowMode==eFMStay)
    {
        mSetFollowMode->setChecked(true);
        mFollowMode = eFMFollowPC;
        this->handleCenterAroundPC();
    } else
    {
        mSetFollowMode->setChecked(false);
        mFollowMode = eFMStay;
    }
}

#include "PsMemView.moc"
