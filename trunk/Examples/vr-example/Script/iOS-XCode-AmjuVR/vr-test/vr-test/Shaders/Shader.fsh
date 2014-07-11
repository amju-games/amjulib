//
//  Shader.fsh
//  vr-test
//
//  Created by JC on 09/07/2014.
//  Copyright (c) 2014 Amju Games. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
