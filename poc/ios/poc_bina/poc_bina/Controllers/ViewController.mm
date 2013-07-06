//
//  ViewController.m
//  poc_bina
//
//  Created by Johan Attali on 7/6/13.
//  Copyright (c) 2013 Johan Attali. All rights reserved.
//

#import "ViewController.h"
#import "POCImage.h"

@interface ViewController ()

@end

@implementation ViewController
@synthesize imageBackground = _imageBackground;

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Use C++ class for image information
    POCImage* image = new POCImage("background");
    
    // add background image
    self.imageBackground             = [[UIImageView alloc] initWithFrame:self.view.bounds];
    _imageBackground.backgroundColor = [UIColor redColor];
    _imageBackground.image           = [UIImage imageNamed:[NSString stringWithUTF8String:image->GetTitle().c_str()]];
    
    [self.view addSubview:_imageBackground];
    
    // delete objects
    delete image;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
