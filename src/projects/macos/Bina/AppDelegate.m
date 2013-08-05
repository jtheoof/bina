//
//  AppDelegate.m
//  Bina
//
//  Created by Johan Attali on 8/5/13.
//  Copyright (c) 2013 Johan Attali. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    self.windowController = [[NSWindowController alloc] initWithWindowNibName:@"MainWindow" owner:self];
    
    [self.windowController showWindow:self];
}

@end
