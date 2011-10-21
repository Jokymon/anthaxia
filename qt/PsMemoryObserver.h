#ifndef _PSMEMORYOBSERVER_H
#define _PSMEMORYOBSERVER_H

#include <QObject>
#include "plugins/processor/MemoryUpdateInterface.h"

class PsMemoryObserver : public QObject, public MemoryObserver
{
Q_OBJECT
public:
    PsMemoryObserver(QObject* parent=0);
    void handleMemoryChanged();
signals:
    void memoryChanged();
};

#endif
