//
//  Shader.vsh
//  iOSMyGameXCode
//
//  Created by Jason Colman on 11/07/2012.
//  Copyright 2012 Amju Games. All rights reserved.
//

attribute vec4 position;
attribute vec4 color;

varying vec4 colorVarying;

uniform float translate;

void main()
{
    gl_Position = position;
    gl_Position.y += sin(translate) / 2.0;

    colorVarying = color;
}
