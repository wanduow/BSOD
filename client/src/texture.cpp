/* 
 * This file is part of BSOD client
 *
 * Copyright (c) 2011 The University of Waikato, Hamilton, New Zealand.
 *
 * Author: Paul Hunkin
 *
 * Contributors: Shane Alcock
 *
 * All rights reserved.
 *
 * This code has been developed by the University of Waikato WAND research
 * group. For further information please see http://www.wand.net.nz/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id$
 */


#include "main.h"

#define ILUT_USE_OPENGL

//DevIL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

vector<Texture *> mTextures;


/*********************************************
		Makes an empty texture obj
**********************************************/
static Texture *genTexObj(){

	Texture *tex = new Texture;
	
	if(!tex){
		return NULL;
	}
	
	tex->iSizeX = 0;
	tex->iSizeY = 0;
	tex->iGLID = -1;
	tex->iDevilID = -1;
	tex->mData = NULL;
	tex->mFilename = "";
	
	mTextures.push_back(tex);
	
	return tex;
}

/*********************************************
		Makes a DevIL image
**********************************************/
static ILuint makeImage(){
    ILuint ImageName; 
    ilGenImages(1, &ImageName);    
    return ImageName; 
}

/*********************************************
		Loads an image into DevIL
**********************************************/
Texture *App::texLoad(string name, int flags){

#ifdef _WINDOWS
	string path = "data\\" + name;
#else
	string path = "data/" + name;	
#endif
	
	LOG("Loading '%s'...", path.c_str());

	Texture *tex = genTexObj();
	
	if(!tex){
		ERR("Couldn't allocate a texture object!\n");
		return NULL;
	}
		
	ILuint devilID = makeImage();		
	ilBindImage(devilID);
	
	tex->iDevilID = devilID;	
	tex->mFilename = name;
	
	//Load the data	
	if(!ilLoadImage((char *)path.c_str())){
		int err = ilGetError();
		ERR("error: %s, %d!\n", path.c_str(), err);
		return NULL;
	}
	
	//Windows and Linux don't agree on which way images should be pointing.
	//Perry: grab a rusty spoon, and decapitate yourself, it's just easier.
#ifndef _WINDOWS
	iluFlipImage();
#endif

	//OK, so we've flipped it dependent on platform already. Now if
	//necessary we flip *again*, to observe what the user wants. 
	if(bTexFlip){
		iluFlipImage();
	}

		
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
			
	tex->iSizeX = ilGetInteger(IL_IMAGE_WIDTH);
	tex->iSizeY = ilGetInteger(IL_IMAGE_HEIGHT);
	
	if(flags != TEXTURE_NO_GL){	
		tex->mData = new byte[tex->iSizeX * tex->iSizeY * 4];
		ilCopyPixels(0, 0, 0, tex->iSizeX, tex->iSizeY, 1, IL_RGBA,
				IL_UNSIGNED_BYTE, tex->mData);
		
		tex->iGLID = ilutGLBindTexImage(); //ilutGLBindMipmaps();
		
		LOG("(got id %d)\n", tex->iGLID);	    
	}	
	
	return tex;
}

/*********************************************
	Generates a texture from a buffer
**********************************************/

Texture *App::texGenerate(string name, byte *buffer, int buflen){
	
	LOG("Loading '%s' from buffer...", name.c_str());

	Texture *tex = genTexObj();
	
	if(!tex){
		ERR("Couldn't allocate a texture object!\n");
		return NULL;
	}
		
	ILuint devilID = makeImage();		
	ilBindImage(devilID);
	
	tex->iDevilID = devilID;	
	tex->mFilename = name;
	
	//Load the data	from the buf.
	if(!ilLoadL(0, buffer, buflen)){
		ERR("error!\n");
		return NULL;
	}
	
#ifndef _WINDOWS
	iluFlipImage();
#endif
	
	if(bTexFlip){
		iluFlipImage();
	}
					
	tex->iSizeX = ilGetInteger(IL_IMAGE_WIDTH);
	tex->iSizeY = ilGetInteger(IL_IMAGE_HEIGHT);
	
	tex->mData = ilGetData();
	tex->iGLID = ilutGLBindTexImage(); //ilutGLBindMipmaps();
	
	LOG("(got id %d)\n", tex->iGLID);	    
	
	return tex;
}

/*********************************************
		Starts up the texture sys
**********************************************/
bool App::texInit(){
	
	
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION || 
		iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION || 
		ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) 
	{
		LOG("DevIL version is different...exiting!\n");
		return false;
	}
	
	ilInit();
	ilutRenderer(ILUT_OPENGL);
	
	mLeftTex = mRightTex = NULL;
	
	//preload any common textures here
	mTextures.clear();
	
	/*
	string initialTextures[] = {"particle.bmp"};
	
	for(int i=0;i<1;i++){
		if(!texLoad(initialTextures[i], 0)){
			return false;
		}
	}
	*/
	
	if(mLeftTexName != ""){
		mLeftTex = texLoad(mLeftTexName, 0);
		if(!mLeftTex){
			return false;
		}
	}
	
	if(mRightTexName != ""){
		mRightTex = texLoad(mRightTexName, 0);
		if(!mRightTex){
			return false;
		}
	}
	
	mParticleTex = texLoad(mParticleTexName, 0);
	
	if(!mParticleTex){
		LOG("Couldn't load particle texture '%s'\n", mParticleTexName.c_str());
		return false;
	}
	
	texLoad("banner.png", 0);
	
#ifdef ENABLE_CGL_COMPAT
	//under CGL we want a mouse cursor
	texLoad("mouse.png", 0); 
#endif
		
	LOG("Finished loading initial textures!\n");
	
	return true;

}

/*********************************************
		Cleans up any images we've loaded
**********************************************/
void App::texShutdown(){

	for(int i=0;i<(int)mTextures.size();i++){	
		ILuint id = mTextures[i]->iDevilID;		
		
		ilDeleteImages(1, &id);
		
		glDeleteTextures(1, &mTextures[i]->iGLID);
						
		LOG("Freed '%s'\n", mTextures[i]->mFilename.c_str());
			
		delete mTextures[i];
	}
	
	ilShutDown();
}

/*********************************************
		Delete a specific texture
**********************************************/
void App::texDelete(Texture *tex){
	ILuint id =tex->iDevilID;			
	ilDeleteImages(1, &id);	
	glDeleteTextures(1, &tex->iGLID);
	
	for(int i=0;i<mTextures.size();i++){
		if(mTextures[i] == tex){
			free(tex->mData);
			mTextures[i] = mTextures[mTextures.size() - 1];
			mTextures.pop_back();
			break;
		}
	}
					
	LOG("Freed '%s'\n", tex->mFilename.c_str());
	
	delete tex;
}


/*********************************************
		Returns a texture by name
**********************************************/
Texture *App::texGet(string name){

	for(int i=0;i<(int)mTextures.size();i++){
		Texture *t = mTextures[i];		
		if(t && t->mFilename == name){
			return t;
		}
	}
	return NULL;
}


