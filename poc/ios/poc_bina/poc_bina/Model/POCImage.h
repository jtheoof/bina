//
//  POCImage.h
//  poc_bina
//
//  Created by Johan Attali on 7/6/13.
//  Copyright (c) 2013 Johan Attali. All rights reserved.
//

#ifndef __poc_bina__POCImage__
#define __poc_bina__POCImage__

#include <iostream>

class POCImage {
private:
    std::string _title;
public:
    POCImage(const std::string& title);
    
    inline std::string GetTitle() {return _title; }


};

#endif /* defined(__poc_bina__POCImage__) */
