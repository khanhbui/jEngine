//
//  FileUtils.h
//  jEngine
//
//  Created by Khanh Bui on 10/20/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__FileUtils__
#define __jEngine__FileUtils__

#include "Macro.h"

NAMESPACE_BEGIN

class FileUtils
{
private:
    char * __resourcePath;

protected:
    static FileUtils * _instance;

    FileUtils(const char * ResourcePath);

public:
    ~FileUtils();

    const char * getFileName(const char * FileName) const;

    static FileUtils * Instance();
    static void Create(const char * ResourcePath);
    static void Delete();
};

NAMESPACE_END

#endif /* defined(__jEngine__FileUtils__) */
