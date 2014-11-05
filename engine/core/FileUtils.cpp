//
//  FileUtils.cpp
//  jEngine
//
//  Created by Khanh Bui on 10/20/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "FileUtils.h"
#include <iostream>
#include <string>

NAMESPACE_USING

FileUtils * FileUtils::_instance = NULL;

FileUtils * FileUtils::Instance()
{
    return FileUtils::_instance;
}

void FileUtils::Create(const char * ResourcePath)
{
    if (!FileUtils::_instance)
    {
        _instance = new FileUtils(ResourcePath);
    }
}

void FileUtils::Delete()
{
    DELETE(_instance);
}

FileUtils::FileUtils(const char * ResourcePath)
{
    __resourcePath = new char[strlen(ResourcePath) + 1];
    strcpy(__resourcePath, ResourcePath);
    strcat(__resourcePath, "/");
    
    LOGD("Resource Path: %s", __resourcePath);
}

FileUtils::~FileUtils()
{
    DELETE_ARRAY(__resourcePath);
}

const char * FileUtils::getFileName(const char * FileName) const
{
    char * filename = new char[strlen(__resourcePath) + strlen(FileName)];
    strcpy(filename, __resourcePath);
    LOGD("%s", filename);
    strcat(filename, FileName);
    LOGD("%s", filename);
    return filename;
}