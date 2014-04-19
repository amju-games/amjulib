//
//  Shader.fsh
//  iOSMyGameXCode
//
//  Created by Jason Colman on 11/07/2012.
//  Copyright 2012 Amju Games. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
