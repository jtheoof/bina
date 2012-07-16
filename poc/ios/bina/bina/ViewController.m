//
//  ViewController.m
//  bina
//
//  Created by Johan Attali on 7/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ViewController.h"

#import "WebViewController.h"
#import "NativeViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
  [super viewDidLoad];
}

- (void)viewDidUnload
{
  [super viewDidUnload];
  // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
  return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}

-(void)dealloc
{
  
  [super dealloc];
}

#pragma mark events

- (void)onTouchWebButton:(id)sender
{
  WebViewController* controller = [[WebViewController alloc] initWithNibName:@"WebViewController"
                                                                      bundle:nil];
  
  [self presentModalViewController:controller animated:NO];
  
  [controller release];
  
}

- (void)onTouchNativeButton:(id)sender
{
  NativeViewController* controller = [[NativeViewController alloc] initWithNibName:@"NativeViewController" bundle:nil];
  
  [self presentModalViewController:controller animated:NO];
  
  [controller release];
}
@end
