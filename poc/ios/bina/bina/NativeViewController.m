//
//  NativeViewController.m
//  bina
//
//  Created by Johan Attali on 7/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "NativeViewController.h"

@interface NativeViewController ()

@end

@implementation NativeViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
  return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}

#pragma mark Events

- (void)onTapView:(id)sender
{
  CGPoint point = [sender locationInView:[sender view]];
  
  [vBrian.layer removeAllAnimations];
  
  NSLog(@"touched %@", NSStringFromCGPoint(point));
  
  [UIView animateWithDuration:0.8
                        delay:0
                      options:UIViewAnimationOptionBeginFromCurrentState
                   animations:^{
                     vBrian.center = point;
                   }
                   completion:nil];
}

@end
