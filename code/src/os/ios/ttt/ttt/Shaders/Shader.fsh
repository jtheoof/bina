//
//  Shader.fsh
//  ttt
//
//  Created by Johan Attali on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
