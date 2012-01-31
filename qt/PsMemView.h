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

#ifndef _PSMEMVIEW_H
#define _PSMEMVIEW_H

//#include <QWidget>
#include <QScrollArea>

class MemoryWordInterface;
class PsModelControl;
class PsMemoryObserver;
class QAction;
class QContextMenuEvent;
class QFont;
class QMenu;
class QPaintEvent;

class PsMemView : public QScrollArea
{
Q_OBJECT
public:
    enum EWindowMode { RootWindow, AdditionalWindow };

    PsMemView(QWidget* parent=0, PsModelControl* modeControl=0, MemoryWordInterface* wordInterface=0, EWindowMode windowMode=AdditionalWindow);
    ~PsMemView();
public slots:
    void valueChanged(int value);
protected:
    void mouseReleaseEvent(QMouseEvent *me);
    void paintEvent(QPaintEvent *pe);
protected:
    void paintDisassembly(const QRect& rect);
    void paintDisassemblyLine(QPainter* painter, int line);
    void paintHexView(const QRect& rect);
    void paintHexViewLine(QPainter* painter, int line);
    void contextMenuEvent(QContextMenuEvent* event);

private slots:
    void handleCenterAroundPC();
    void handleModelChanged();
    void handleMemoryChanged();
    void handleSetDisassemblyView();
    void handleSetHexView();
    void handleSetFollowMode();

private:
    enum EViewMode { eVMDisassembly, eVMHexView };
    EViewMode mViewMode;

    enum EFollowMode { eFMStay, eFMFollowPC };
    EFollowMode mFollowMode;

    EWindowMode mWindowMode;

    MemoryWordInterface* mWordInterface;
    PsModelControl *mModelControl;
    int mTopAddress;
    int mSelectedAddress;

    QMenu* mContextMenu;
    QAction* mSetHexView;
    QAction* mSetDisassemblyView;
    QAction* mSetFollowMode;
    QAction* mCenterAroundPC;

    QFont* mMainFont;

    PsMemoryObserver* mMemObserver;
};

#endif
