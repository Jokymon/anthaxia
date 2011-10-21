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
