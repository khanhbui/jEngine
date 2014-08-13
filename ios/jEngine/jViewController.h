//
//  jViewController.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/30/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "ObjectAL.h"
#import "GADBannerView.h"

@interface jViewController : UIView<GADBannerViewDelegate>
{
@private
    id _displayLink;
    EAGLContext * _eglContext;
    
    ALDevice * _device;
    ALContext * _context;

    ALChannelSource * _channel0;
    ALChannelSource * _channel1;
    ALChannelSource * _channel2;
    
    ALBuffer * _sfxs[10];

    int touchesX[20];
    int touchesY[20];
    int numOfTouches;
    
    bool isDisplayAd;
    bool isGameCenterEnabled;

    NSLock *writeLock;
    NSString * leaderboard;
}

@property (readonly, nonatomic) bool animating;
@property (nonatomic) int frameInterval;
@property (nonatomic, strong) GADBannerView *admobBannerView;

+ (jViewController *) instance;

- (void) setup;
- (void) start;
- (void) stop;
- (void) enterFrame: (id) Sender;

@end
