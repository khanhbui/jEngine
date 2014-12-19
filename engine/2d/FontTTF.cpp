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

#define WIDTH   640
#define HEIGHT  480

unsigned char image[HEIGHT][WIDTH];


/* Replace this function with something useful. */

void
draw_bitmap( FT_Bitmap*  bitmap,
            FT_Int      x,
            FT_Int      y)
{
    FT_Int  i, j, p, q;
    FT_Int  x_max = x + bitmap->width;
    FT_Int  y_max = y + bitmap->rows;
    
    
    for ( i = x, p = 0; i < x_max; i++, p++ )
    {
        for ( j = y, q = 0; j < y_max; j++, q++ )
        {
            if ( i < 0      || j < 0       ||
                i >= WIDTH || j >= HEIGHT )
                continue;
            
            image[j][i] |= bitmap->buffer[q * bitmap->width + p];
        }
    }
}

void
show_image( void )
{
    int  i, j;
    
    
    for ( i = 0; i < HEIGHT; i++ )
    {
        for ( j = 0; j < WIDTH; j++ )
            putchar( image[i][j] == 0 ? ' '
                    : image[i][j] < 128 ? '+'
                    : '*' );
        putchar( '\n' );
    }
}


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

    error = FT_Set_Char_Size(face, 12 * 64, 0, 100, 0);
    
    FT_GlyphSlot  slot;
    FT_Matrix     matrix;
    FT_Vector     pen;
    
    double angle = ( 25.0 / 360 ) * 3.14159 * 2;
    int           target_height = HEIGHT;
    int           n, num_chars;
    
    const char * text          = "i";
    num_chars     = strlen( text );

    slot = face->glyph;
    
    /* set up matrix */
    matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
    matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
    matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
    matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

    pen.x = 300 * 64;
    pen.y = ( target_height - 200 ) * 64;
    
    for ( n = 0; n < num_chars; n++ )
    {
        /* set transformation */
        FT_Set_Transform( face, &matrix, &pen );
        
        /* load glyph image into the slot (erase previous one) */
        error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
        if ( error )
            continue;                 /* ignore errors */
        
        /* now, draw to our target surface (convert position) */
        draw_bitmap( &slot->bitmap,
                    slot->bitmap_left,
                    target_height - slot->bitmap_top );
        
        /* increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }
    
    show_image();
    
    FT_Done_Face    ( face );
    FT_Done_FreeType( library );
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