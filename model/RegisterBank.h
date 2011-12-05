#ifndef _REGISTER_BANK_H
#define _REGISTER_BANK_H

#include "SimulationObject.h"

class RegisterBank : public SimulationObject
{
public:
    RegisterBank(std::string _name);

    virtual bool getRegister(int index, int& value) const =0;
    virtual bool setRegister(int index, int value) =0;
    virtual bool getRegisterName(int index, std::string& name) const =0;
    virtual int getRegisterCount() const =0;
    virtual bool isRegisterDirty(int index) const =0;
};

#endif
