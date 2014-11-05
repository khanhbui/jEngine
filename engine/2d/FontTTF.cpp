//
//  FontTTF.cpp
//  jEngine
//
//  Created by Khanh Bui on 10/16/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "FontTTF.h"
#include "FileUtils.h"

NAMESPACE_USING

FontTTF::FontTTF() : Node(),
__text(NULL)
{
    FT_Library library;
    FT_Face face;

    FT_Error error = FT_Init_FreeType(&library);
    if (error)
    {
        LOGD("%s", "an error occurred during library initialization ...");
        return;
    }

    const char * f = FileUtils::Instance()->getFileName("arial.ttf");
    error = FT_New_Face(library, f, 0, &face);
    LOGD("%d", error);
    if (error == FT_Err_Unknown_File_Format)
    {
        LOGD("%s", "the font file could be opened and read, but it appears that its font format is unsupported...");
    }
    else if (error)
    {
        LOGD("%s", "another error code means that the font file could not be opened or read, or simply that it is broken...");
    }
}

FontTTF::~FontTTF()
{
    DELETE_ARRAY(__text);
}

void FontTTF::setText(const char * Text)
{
    if (!__text || strcmp(__text, Text) != 0)
    {
        DELETE_ARRAY(__text);
        __text = new char[strlen(Text)];
        strcpy(__text, Text);
    }
}