models/players/Violator128/violatorTeeth
{
	cull disable
	{
		map models/players/Violator128/violatorTeeth.tga
		rgbGen lightingDiffuse
		alphaFunc GE128
	}
}
models/players/Violator128/violatorTeethred
{
	cull disable
	{
		map models/players/Violator128/violatorTeethred.tga
		rgbGen lightingDiffuse
		alphaFunc GE128
	}
}

models/players/Violator128/magic2
{
	qer_editorimage models/players/Violator128/magic2.tga
	surfaceparm nolightmap
	surfaceparm nomarks
	cull disable
	deformVertexes autosprite
	q3map_surfacelight 5500
	{
		animmap 20 models/players/Violator128/magic2.tga models/players/Violator128/magic3.tga models/players/Violator128/magic1.tga models/players/Violator128/magic2.tga models/players/Violator128/magic3.tga 
                tcmod rotate 460
                rgbGen wave sawtooth 0 1 0 10
		blendfunc add
		rgbGen identity
	}
}

models/players/Violator128/magic1
{
	qer_editorimage models/players/Violator128/magic1.tga
	surfaceparm nolightmap
	surfaceparm nomarks
	cull disable
	deformVertexes autosprite
	q3map_surfacelight 5500
	{
		animmap 20 models/players/Violator128/magic3.tga models/players/Violator128/magic1.tga models/players/Violator128/magic3.tga models/players/Violator128/magic3.tga models/players/Violator128/magic1.tga 
                tcmod rotate 600
		blendfunc add
                rgbGen wave sawtooth 0 1 0 10
		rgbGen identity
	}
}
models/players/Violator128/magic4

{  
       deformVertexes autosprite
        {
		map models/players/Violator128/magic4.tga
                tcmod rotate 760
                blendFunc GL_ONE GL_ONE
		rgbGen wave sawtooth 0 1 0 10
        	
	}
} 

models/players/Violator128/magic3
{
	qer_editorimage models/players/Violator128/magic3.tga
	surfaceparm nolightmap
	surfaceparm nomarks
	cull disable
	deformVertexes autosprite
	q3map_surfacelight 5500
	{
		animmap 20 models/players/Violator128/magic1.tga models/players/Violator128/magic3.tga models/players/Violator128/magic1.tga models/players/Violator128/magic3.tga models/players/Violator128/magic1.tga 
                tcmod rotate 460
                rgbGen wave sawtooth 0 1 0 10
		blendfunc add
		rgbGen identity
	}
}


models/players/Violator128/per128_violator_red_eye
{
	{

		map textures/effects/tinfx2.tga
		rgbGen lightingDiffuse
		tcMod scale 3 3
		tcGen environment  
	}
	{
		map models/players/Violator128/per128_violator_blue_eye.tga
		rgbGen lightingDiffuse
		blendfunc GL_ONE GL_SRC_ALPHA

	}
}
models/players/Violator128/per128_violator_blue_eye
{
	{

		map textures/effects/tinfx2.tga
		rgbGen lightingDiffuse
		tcMod scale 3 3
		tcGen environment  
	}
	{
		map models/players/Violator128/per128_violator_blue_eye.tga
		rgbGen lightingDiffuse
		blendfunc GL_ONE GL_SRC_ALPHA

	}
}

