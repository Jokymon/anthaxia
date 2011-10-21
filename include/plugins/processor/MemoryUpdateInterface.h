#ifndef _MEMORYUPDATEINTERFACE_H
#define _MEMORYUPDATEINTERFACE_H

class MemoryObserver
{
public:
    virtual void handleMemoryChanged() =0;
};

#endif
