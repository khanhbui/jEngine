//
//  jViewController.m
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/30/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#import "jViewController.h"
#import <QuartzCore/QuartzCore.h>
#import <AudioToolbox/AudioToolbox.h>
#import <GLKit/GLKit.h>

#import "Engine.h"
#import "jFont/MainScene.h"

@implementation jViewController

@synthesize animating;
@synthesize frameInterval;
@synthesize admobBannerView;

static jViewController * instance;

// You must implement this method
+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id) initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];

    
    instance = self;
    self.multipleTouchEnabled = false;

    // Create the device and context.
    // Note that it's easier to just let OALSimpleAudio handle
    // these rather than make and manage them yourself.
    _device = [[ALDevice deviceWithDeviceSpecifier:nil] retain];
    _context = [[ALContext contextOnDevice:_device attributes:nil] retain];
    [OpenALManager sharedInstance].currentContext = _context;
    
    // Deal with interruptions for me!
    [OALAudioSession sharedInstance].handleInterruptions = YES;
    
    // We don't want ipod music to keep playing since
    // we have our own bg music.
    [OALAudioSession sharedInstance].allowIpod = YES;
    [OALAudioSession sharedInstance].useHardwareIfAvailable = NO;
    
    // Mute all audio if the silent switch is turned on.
    [OALAudioSession sharedInstance].honorSilentSwitch = YES;
    
    // Take all 32 sources for this channel.
    // (we probably won't use that many but what the heck!)
    _channel0 = [[ALChannelSource channelWithSources:2] retain];
    _channel1 = [[ALChannelSource channelWithSources:2] retain];
    _channel2 = [[ALChannelSource channelWithSources:2] retain];
    
    _sfxs[0] = [[[OpenALManager sharedInstance] bufferFromFile:@"sfx_point.aif"] retain];

    isDisplayAd = true;
    animating = false;

    return self;
}

- (void) dealloc
{
    [_channel0 release];
    [_channel1 release];
    [_channel2 release];
    
    [_context release];
    [_device release];
    
    instance = nil;
    
    j::Engine::Delete();
    
    if ([EAGLContext currentContext] == _eglContext)
    {
        [EAGLContext setCurrentContext:nil];
    }
    _eglContext = nil;
    
    [super dealloc];
}

+ (jViewController *) instance
{
    return instance;
}

- (void) setup
{
    CAEAGLLayer * eaglLayer = (CAEAGLLayer *)self.layer;
    eaglLayer.opaque = true;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
    frameInterval = 1;
    
    _eglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    [EAGLContext setCurrentContext:_eglContext];
    
    numOfTouches = 0;

    j::FileUtils::Create([[[NSBundle mainBundle] resourcePath] UTF8String]);
    j::Engine::Create();
    j::Engine::Instance()->setup();
    
    [self setTextureAtlas];

    j::Engine::Instance()->setDefaultScene(new jFont::MainScene(false));
}

- (void) setTextureAtlas
{
    //atlas1
    GLuint texName;
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"atlas1" ofType:@"png"];
    LOGD("path %s", [path UTF8String]);
    NSData *texData = [[NSData alloc] initWithContentsOfFile:path];
    UIImage *image = [[UIImage alloc] initWithData:texData];
    if (image == nil)
        NSLog(@"Do real error checking here");
    
    GLuint width = static_cast<GLuint>(CGImageGetWidth(image.CGImage));
    GLuint height = static_cast<GLuint>(CGImageGetHeight(image.CGImage));
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    void *imageData = malloc( height * width * 4 );
    CGContextRef _pContext = CGBitmapContextCreate( imageData, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
    CGColorSpaceRelease( colorSpace );
    CGContextClearRect( _pContext, CGRectMake( 0, 0, width, height ) );
    CGContextTranslateCTM( _pContext, 0, height - height );
    CGContextDrawImage( _pContext, CGRectMake( 0, 0, width, height ), image.CGImage );
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    
    CGContextRelease(_pContext);
    
    free(imageData);
    [image release];
    [texData release];
    
    NSString * txtPath = [[NSBundle mainBundle] pathForResource:@"atlas1" ofType:@"txt"];
    const char * atlasTxt = [txtPath UTF8String];
    
    char _read_buffer[10240];
    FILE * _atlas_file = fopen(atlasTxt, "r");
    size_t _atlas_len = 0;
    if (_atlas_file)
    {
        _atlas_len = fread(_read_buffer, 1, 10240, _atlas_file);
    }
    fclose(_atlas_file);
    j::Engine::Instance()->setTextureAtlas(texName, _read_buffer, _atlas_len);
    
    
    //atlas2
    glGenTextures(2, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    path = [[NSBundle mainBundle] pathForResource:@"atlas2" ofType:@"png"];
    texData = [[NSData alloc] initWithContentsOfFile:path];
    image = [[UIImage alloc] initWithData:texData];
    if (image == nil)
        NSLog(@"Do real error checking here");
    
    width = static_cast<GLuint>(CGImageGetWidth(image.CGImage));
    height = static_cast<GLuint>(CGImageGetHeight(image.CGImage));
    colorSpace = CGColorSpaceCreateDeviceRGB();
    imageData = malloc( height * width * 4 );
    _pContext = CGBitmapContextCreate( imageData, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
    CGColorSpaceRelease( colorSpace );
    CGContextClearRect( _pContext, CGRectMake( 0, 0, width, height ) );
    CGContextTranslateCTM( _pContext, 0, height - height );
    CGContextDrawImage( _pContext, CGRectMake( 0, 0, width, height ), image.CGImage );
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    
    CGContextRelease(_pContext);
    
    free(imageData);
    [image release];
    [texData release];
    
    txtPath = [[NSBundle mainBundle] pathForResource:@"atlas2" ofType:@"txt"];
    atlasTxt = [txtPath UTF8String];
    
    _atlas_file = fopen(atlasTxt, "r");
    _atlas_len = 0;
    if (_atlas_file)
    {
        _atlas_len = fread(_read_buffer, 1, 10240, _atlas_file);
    }
    fclose(_atlas_file);
    j::Engine::Instance()->setTextureAtlas(texName, _read_buffer, _atlas_len);
    
    
    //atlas3
    glGenTextures(3, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    path = [[NSBundle mainBundle] pathForResource:@"atlas3" ofType:@"png"];
    texData = [[NSData alloc] initWithContentsOfFile:path];
    image = [[UIImage alloc] initWithData:texData];
    if (image == nil)
        NSLog(@"Do real error checking here");
    
    width = static_cast<GLuint>(CGImageGetWidth(image.CGImage));
    height = static_cast<GLuint>(CGImageGetHeight(image.CGImage));
    colorSpace = CGColorSpaceCreateDeviceRGB();
    imageData = malloc( height * width * 4 );
    _pContext = CGBitmapContextCreate( imageData, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
    CGColorSpaceRelease( colorSpace );
    CGContextClearRect( _pContext, CGRectMake( 0, 0, width, height ) );
    CGContextTranslateCTM( _pContext, 0, height - height );
    CGContextDrawImage( _pContext, CGRectMake( 0, 0, width, height ), image.CGImage );
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    
    CGContextRelease(_pContext);
    
    free(imageData);
    [image release];
    [texData release];
    
    txtPath = [[NSBundle mainBundle] pathForResource:@"atlas3" ofType:@"txt"];
    atlasTxt = [txtPath UTF8String];
    
    _atlas_file = fopen(atlasTxt, "r");
    _atlas_len = 0;
    if (_atlas_file)
    {
        _atlas_len = fread(_read_buffer, 1, 10240, _atlas_file);
    }
    fclose(_atlas_file);
    j::Engine::Instance()->setTextureAtlas(texName, _read_buffer, _atlas_len);
    
    
    //atlas4
    glGenTextures(4, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    path = [[NSBundle mainBundle] pathForResource:@"atlas4" ofType:@"png"];
    texData = [[NSData alloc] initWithContentsOfFile:path];
    image = [[UIImage alloc] initWithData:texData];
    if (image == nil)
        NSLog(@"Do real error checking here");
    
    width = static_cast<GLuint>(CGImageGetWidth(image.CGImage));
    height = static_cast<GLuint>(CGImageGetHeight(image.CGImage));
    colorSpace = CGColorSpaceCreateDeviceRGB();
    imageData = malloc( height * width * 4 );
    _pContext = CGBitmapContextCreate( imageData, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
    CGColorSpaceRelease( colorSpace );
    CGContextClearRect( _pContext, CGRectMake( 0, 0, width, height ) );
    CGContextTranslateCTM( _pContext, 0, height - height );
    CGContextDrawImage( _pContext, CGRectMake( 0, 0, width, height ), image.CGImage );
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    
    CGContextRelease(_pContext);
    
    free(imageData);
    [image release];
    [texData release];
    
    txtPath = [[NSBundle mainBundle] pathForResource:@"atlas4" ofType:@"txt"];
    atlasTxt = [txtPath UTF8String];
    
    _atlas_file = fopen(atlasTxt, "r");
    _atlas_len = 0;
    if (_atlas_file)
    {
        _atlas_len = fread(_read_buffer, 1, 10240, _atlas_file);
    }
    fclose(_atlas_file);
    j::Engine::Instance()->setTextureAtlas(texName, _read_buffer, _atlas_len);
}

- (void) start
{
    if (!animating)
    {
        _displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(enterFrame:)];
        [_displayLink setFrameInterval:frameInterval];
        [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        animating = true;
        
        j::Engine::Instance()->onResume();
    }
}

- (void) stop
{
    if (animating)
    {
        [_displayLink invalidate];
        _displayLink = nil;
        animating = false;
        
        j::Engine::Instance()->onPause();
    }
}

- (void) enterFrame:(id)Sender
{
    [EAGLContext setCurrentContext:_eglContext];
    
    j::Engine::Instance()->setTouches(touchesX, touchesY, numOfTouches);
    j::Engine::Instance()->enterFrame();
    [_eglContext presentRenderbuffer:GL_RENDERBUFFER];
}

- (void) layoutSubviews
{
    j::Engine::Instance()->resize(self.layer.bounds.size.width * self.layer.contentsScale, self.layer.bounds.size.height * self.layer.contentsScale);
    [_eglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
    [self enterFrame:nil];
}

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    j::Engine * engine = j::Engine::Instance();
    engine->numOfTouches = 0;
    
    numOfTouches = 0;
    
    for (UITouch * touch : touches)
    {
        int _touch_x = (int)((([touch locationInView:self].x - engine->renderer()->__offsetX) / engine->renderer()->__fitWidth) * SCREEN_WIDTH);
        int _touch_y = (int)((([touch locationInView:self].y - engine->renderer()->__offsetY) / engine->renderer()->__fitHeight) * SCREEN_HEIGHT);
        
        if ([touch phase] == UITouchPhaseBegan)
        {
            engine->touchPressed(_touch_x, _touch_y);
        }
        
        if ([touch phase] == UITouchPhaseBegan || [touch phase] == UITouchPhaseMoved || [touch phase] == UITouchPhaseStationary)
        {
            touchesX[numOfTouches] = _touch_x;
            touchesY[numOfTouches] = _touch_y;
            numOfTouches++;
        }
    }
}

-(void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    j::Engine * engine = j::Engine::Instance();
    engine->numOfTouches = 0;
    
    numOfTouches = 0;
    
    for (UITouch * touch : touches)
    {
        int _touch_x = (int)((([touch locationInView:self].x - engine->renderer()->__offsetX) / engine->renderer()->__fitWidth) * SCREEN_WIDTH);
        int _touch_y = (int)((([touch locationInView:self].y - engine->renderer()->__offsetY) / engine->renderer()->__fitHeight) * SCREEN_HEIGHT);
        
        if ([touch phase] == UITouchPhaseMoved)
        {
            int _prev_x = (int)((([touch previousLocationInView:self].x - engine->renderer()->__offsetX) / engine->renderer()->__fitWidth) * SCREEN_WIDTH);
            int _prev_y = (int)((([touch previousLocationInView:self].y - engine->renderer()->__offsetY) / engine->renderer()->__fitHeight) * SCREEN_HEIGHT);
            engine->touchMoved(_prev_x, _prev_y, _touch_x, _touch_y);
        }
        
        if ([touch phase] == UITouchPhaseBegan || [touch phase] == UITouchPhaseMoved || [touch phase] == UITouchPhaseStationary)
        {
            touchesX[numOfTouches] = _touch_x;
            touchesY[numOfTouches] = _touch_y;
            numOfTouches++;
        }
    }
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    j::Engine * engine = j::Engine::Instance();
    engine->numOfTouches = 0;
    
    numOfTouches = 0;
    
    for (UITouch * touch : touches)
    {
        int _touch_x = (int)((([touch locationInView:self].x - engine->renderer()->__offsetX) / engine->renderer()->__fitWidth) * SCREEN_WIDTH);
        int _touch_y = (int)((([touch locationInView:self].y - engine->renderer()->__offsetY) / engine->renderer()->__fitHeight) * SCREEN_HEIGHT);
        
        if ([touch phase] == UITouchPhaseEnded)
        {
            int _prev_x = (int)((([touch previousLocationInView:self].x - engine->renderer()->__offsetX) / engine->renderer()->__fitWidth) * SCREEN_WIDTH);
            int _prev_y = (int)((([touch previousLocationInView:self].y - engine->renderer()->__offsetY) / engine->renderer()->__fitHeight) * SCREEN_HEIGHT);
            
            engine->touchReleased(_prev_x, _prev_y, _touch_x, _touch_y);
        }
        
        if ([touch phase] == UITouchPhaseBegan || [touch phase] == UITouchPhaseMoved || [touch phase] == UITouchPhaseStationary)
        {
            touchesX[numOfTouches] = _touch_x;
            touchesY[numOfTouches] = _touch_y;
            numOfTouches++;
        }
    }
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    j::Engine * engine = j::Engine::Instance();
    engine->numOfTouches = 0;
    
    numOfTouches = 0;
    
    for (UITouch * touch : touches)
    {
        int _touch_x = (int)((([touch locationInView:self].x - engine->renderer()->__offsetX) / engine->renderer()->__fitWidth) * SCREEN_WIDTH);
        int _touch_y = (int)((([touch locationInView:self].y - engine->renderer()->__offsetY) / engine->renderer()->__fitHeight) * SCREEN_HEIGHT);
        
        if ([touch phase] == UITouchPhaseBegan || [touch phase] == UITouchPhaseMoved || [touch phase] == UITouchPhaseStationary)
        {
            touchesX[numOfTouches] = _touch_x;
            touchesY[numOfTouches] = _touch_y;
            numOfTouches++;
        }
    }
}

@end
