//
//  ViewController.h
//  bina
//
//  Created by Johan Attali on 7/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController <UIWebViewDelegate>
{
}

- (IBAction)onTouchWebButton:(id)sender;
- (IBAction)onTouchNativeButton:(id)sender;

@end
