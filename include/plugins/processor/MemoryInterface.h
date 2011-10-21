#ifndef _MEMORYINTERFACE_H
#define _MEMORYINTERFACE_H

class MemoryObserver;

class MemoryByteInterface {
public:
    virtual bool getByte(int address, unsigned char& value) const =0;
    virtual bool setByte(int address, unsigned char value) =0;
    virtual void registerMemoryObserver(MemoryObserver* observer) =0;
    /// call this function to indicate that one or multiple memory changes via
    /// setByte have been completed and that registered memory observers should be
    /// notified about it
    virtual void transactionComplete() =0;
};

class MemoryWordInterface {
public:
    virtual bool getWord(int address, unsigned int& value) const =0;
    virtual bool setWord(int address, unsigned int value) =0;
    virtual void registerMemoryObserver(MemoryObserver* observer) =0;
};

#endif
