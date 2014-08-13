//
//  Ref.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/30/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Ref__
#define __jEngine__Ref__

#include <iostream>
#include <vector>
#include <typeinfo>

#include "Macro.h"

NAMESPACE_BEGIN

class Ref
{
    private:
    char * _name;
    
    long _objectId;
    static long _objectCount;
    
    protected:
    unsigned int _referenceCount;
    
    friend class AutoreleasePool;
    
    public:
    Ref();
    Ref(const char * name);
    virtual ~Ref();
    
    void retain();
    void release();
    Ref * autorelease();
    
    unsigned int getReferenceCount() const;
    virtual const char * getName();
};

class AutoreleasePool
{
private:
    std::vector<Ref *> _managedObjectArray;
    std::string _name;
    
    bool _isClearing;
    
    public:
    AutoreleasePool();
    AutoreleasePool(const std::string &name);
    
    ~AutoreleasePool();
    
    void addObject(Ref * object);
    void clear();
    
    bool isClearing() const { return _isClearing; };
    
    bool contains(Ref * object) const;
    
    void dump();
};

class PoolManager
{
public:
    static PoolManager * Instance();
    static void Create();
    static void Delete();
    
    AutoreleasePool * getCurrentPool() const;
    
    bool isObjectInPools(Ref * obj) const;
    
    friend class AutoreleasePool;
    
    private:
    PoolManager();
    ~PoolManager();
    
    void push(AutoreleasePool * pool);
    void pop();
    
    static PoolManager * _instance;
    
    std::vector<AutoreleasePool *> _releasePoolStack;
};

NAMESPACE_END

#endif /* defined(__jEngine__Ref__) */
