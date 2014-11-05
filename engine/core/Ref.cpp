//
//  Ref.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/30/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Ref.h"

NAMESPACE_USING

//--------------------------------------------------------------------
// Ref
//--------------------------------------------------------------------

long Ref::_objectCount = 0;

Ref::Ref() : _referenceCount(1), _name(NULL), _objectId(_objectCount++)
{
}

Ref::Ref(const char * name) : Ref()
{
    _name = new char[strlen(name)];
    strcpy(_name, name);
}

Ref::~Ref()
{
    delete [] _name;
    _name = NULL;
    
    if (_referenceCount != 0)
    {
        //ASSERT(false, "Something's wrong with retain/release!!! (%s)._referenceCount=%d", getName(), _referenceCount);
    }
}

void Ref::retain()
{
    ++_referenceCount;
}

void Ref::release()
{
    --_referenceCount;
    if (_referenceCount == 0)
    {
        
        delete this;
    }
}

Ref * Ref::autorelease()
{
    PoolManager::Instance()->getCurrentPool()->addObject(this);
    return this;
}

unsigned int Ref::getReferenceCount() const
{
    return _referenceCount;
}

const char * Ref::getName()
{
    if (_name == NULL)
    {
        const char * name = typeid(* this).name();
        _name = new char[strlen(name) + 5];
        sprintf(_name, "%s_%ld", name, _objectId);
    }
    return _name;
}



//--------------------------------------------------------------------
// AutoreleasePool
//--------------------------------------------------------------------

AutoreleasePool::AutoreleasePool() : _name(""), _isClearing(false)
{
    _managedObjectArray.reserve(150);
    PoolManager::Instance()->push(this);
}

AutoreleasePool::AutoreleasePool(const std::string &name) : _name(name), _isClearing(false)
{
    _managedObjectArray.reserve(150);
    PoolManager::Instance()->push(this);
}

AutoreleasePool::~AutoreleasePool()
{
    LOGI("deallocing AutoreleasePool: %p", this);
    clear();
    
    PoolManager::Instance()->pop();
}

void AutoreleasePool::addObject(Ref * object)
{
    _managedObjectArray.push_back(object);
}

void AutoreleasePool::clear()
{
    _isClearing = true;
    for (size_t i = 0, size = _managedObjectArray.size(); i < size; ++i)
    {
        Ref * obj = _managedObjectArray[i];
        obj->release();
    }
    _managedObjectArray.clear();
    _isClearing = false;
}

bool AutoreleasePool::contains(Ref * object) const
{
    for (size_t i = 0, size = _managedObjectArray.size(); i < size; ++i)
    {
        if (object == _managedObjectArray[i])
        {
            return true;
        }
    }
    return false;
}

void AutoreleasePool::dump()
{
    LOGD("autorelease pool: %s, number of managed object %d\n", _name.c_str(), static_cast<int>(_managedObjectArray.size()));
    LOGD("%20s%20s%20s\n", "Object pointer", "Object name", "Reference count");
    for (size_t i = 0, size = _managedObjectArray.size(); i < size; ++i)
    {
        Ref * obj = _managedObjectArray[i];
        LOGD("%20p%20s%20u\n", obj, obj->getName(), obj->getReferenceCount());
    }
}


//--------------------------------------------------------------------
// PoolManager
//--------------------------------------------------------------------

PoolManager * PoolManager::_instance = NULL;

PoolManager * PoolManager::Instance()
{
    return _instance;
}

void PoolManager::Create()
{
    if (_instance == NULL)
    {
        _instance = new PoolManager();
        new AutoreleasePool("dotEngine autorelease pool");
    }
}

void PoolManager::Delete()
{
    delete _instance;
    _instance = NULL;
}

PoolManager::PoolManager()
{
    _releasePoolStack.reserve(10);
}

PoolManager::~PoolManager()
{
    LOGI("deallocing PoolManager: %p", this);
    
    while (!_releasePoolStack.empty())
    {
        AutoreleasePool * pool = _releasePoolStack.back();
        delete pool;
    }
}

AutoreleasePool * PoolManager::getCurrentPool() const
{
    return _releasePoolStack.back();
}

bool PoolManager::isObjectInPools(Ref * obj) const
{
    for (size_t i = 0, size = _releasePoolStack.size(); i < size; ++i)
    {
        AutoreleasePool * pool = _releasePoolStack[i];
        if (pool->contains(obj))
        {
            return true;
        }
    }
    return false;
}

void PoolManager::push(AutoreleasePool * pool)
{
    _releasePoolStack.push_back(pool);
}

void PoolManager::pop()
{
    _releasePoolStack.pop_back();
}
