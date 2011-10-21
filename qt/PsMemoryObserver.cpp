#include "PsMemoryObserver.h"
#include <iostream>

PsMemoryObserver::PsMemoryObserver(QObject* parent)
: QObject(parent)
{ }

void PsMemoryObserver::handleMemoryChanged()
{
    emit memoryChanged();
}

#include "PsMemoryObserver.moc"
