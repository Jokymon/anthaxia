#ifndef _DYNAMICLIBRARY_H
#define _DYNAMICLIBRARY_H

#include <string>

class DynamicLibrary
{
public:
    static DynamicLibrary* load(const std::string& path);
    ~DynamicLibrary();
    void* getSymbol(const std::string& name);

    static const std::string DL_PREFIX;
    static const std::string DL_SUFFIX;
private:
    DynamicLibrary();
    DynamicLibrary(void* handle);
    DynamicLibrary(const DynamicLibrary&);
private:
    void* handle_;
};

#endif
