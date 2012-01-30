#ifndef _DUMMYREGISTER_H
#define _DUMMYREGISTER_H

#include "model/SimulationObject.h"

class DummyRegister : public SimulationObject
{
public:
    DummyRegister(std::string name, SimulationObject* _parent);
    virtual int parentIndex();
    virtual SimulationObject* getChild(int index);
    virtual int childCount();

    virtual void accept(SimulationObjectVisitor& visitor);
private:
};

#endif // _DUMMYREGISTER_H
