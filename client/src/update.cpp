#include "main.h"





/*********************************************
		per-frame logic goes here
**********************************************/
void App::updateMain(){

	//Get the mouse pointer pos in screen space
	SDL_GetMouseState(&iMouseX, &iMouseY); 
	
	//Throw away any bad frames
	if(fTimeScale == infinity || fTimeScale == 0.0f){
		return;
	}
	
	float fCamSpeed = fTimeScale * 4.0f;
	float fDragScale = 0.05f;

	//Right button means we reset rotation and such
	//NOTE: I did have this as 'both left and right at once', which seemed
	//to make more sense, but it doesn't work great on systems that map this
	//to middle-mouse		
	if(mouseDown(3)){
		for(int i=0;i<3;i++){
			fRot[i] = 0.0f;
		}
		fRot[0] = 20;
		fZoom = 0.0f;
	}
	
	//If we're actively dragging with the mouse
	if(bDrag){
		
		//Figure out the drag vectors and stuff
		Vector2 drag = Vector2(iMouseX, iMouseY);
		Vector2 diff = (dragStart - drag) * fDragScale;
				
		//Left mouse button means we modify the rotation
		if(mouseDown(1)){
			fRot[1] -= diff.x;
			fRot[0] -= diff.y;
			
			//SDL_Delay(50);
		}
		
		//Middle mouse means we modify the zoom
		else if(mouseDown(2)){
			fZoom += diff.y;
		}
		
				
		dragStart = Vector2(iMouseX, iMouseY);
		dragVel = diff;
		
	}else{
		fRot[1] -= dragVel.x;
		fRot[0] -= dragVel.y;
	}
	

	updateSocket();
	
	if(!isConnected()){
		generateTestData();
	}
	
	mParticleSystem->update();		
	mFlowMgr->update(0.0f, PARTICLE_FPS);
	
	fNextFlowUpdate -= fTimeScale;
	if(fNextFlowUpdate <= 0.0f){
		mFlowMgr->update(0.0f, fTimeScale);
		fNextFlowUpdate = PARTICLE_FPS * 5;	
	}
}

/*********************************************
	Generate particles that are shown
	while not connected to a server
**********************************************/
float fTestTime = 0.0f;
int iTestRow = 0;
bool isRotating = false;

void App::generateTestData(){
	
	fTestTime += fTimeScale;
	
	Texture *tex = texGet("banner.png");
	byte *data = tex->mData;
	
	if(fTestTime > 0.02f){
		fTestTime = 0.0f;
		iTestRow++;
		
		if(iTestRow >= tex->iSizeX){
			iTestRow = 0;
			LOG("Ended\n");
			isRotating = true;
		}
		
		int x = iTestRow;
		
		//LOG("%d\n", x);
		
		for(int y=0;y<tex->iSizeY;y++){
			
			int index = (x * 4) + ((tex->iSizeY - y) * tex->iSizeX * 4);
			
			byte r = data[index + 0];
			byte g = data[index + 1];
			byte b = data[index + 2];
			
			//if(b == 255)
				//LOG("%d/%d: %d/%d/%d\n", x, y, r, g, b);
			
			if(r > 0 || g > 0 || b > 0){
				float v = 0.05f;
				Vector3 vel = Vector3(-10, 0, 0) + Vector3(randFloat(-v,v),randFloat(-v,v),randFloat(-v,v));
				Vector3 pos = Vector3(randFloat(-v,v),randFloat(-v,v),randFloat(-v,v) + randFloat(-1,1));
				ps()->add(pos + Vector3(60, (y / 3.0f) - 20, 0), vel, Color(r,g,b), 1.0f, 15.0f);
				//LOG("Added at %d/%d\n", x, y);
			}
			
		}
	}
	
	if(isRotating)
		fRot[2] += fTimeScale * 10; //rotate
	
}	


/*********************************************
		SDL event loop
**********************************************/
void App::utilEventLoop(){

	SDL_Event event;

    while ( !done ){
	   
	    while ( SDL_PollEvent( &event ) ){
	    
	    	//Hand the event off to the GUI first. If the GUI handles it, it's
	    	//done. 
	    	if(processGUIEvent(event)){
	    		continue;
	    	}
	    
		    switch( event.type ){
						      
			case SDL_VIDEORESIZE:
			    //handle resize event
			    surface = SDL_SetVideoMode( event.resize.w, event.resize.h, 16, videoFlags );
			    if ( !surface ){
				    fprintf( stderr, "Could not get a surface after resize: %s\n", SDL_GetError( ) );
				    utilShutdown( 1 );
				}
			    resizeWindow( event.resize.w, event.resize.h );
			    resizeGUI( event.resize.w, event.resize.h );
			    break;
			
			case SDL_QUIT:
			    /* handle quit requests */
			    done = true;
			    break;
			
			case SDL_MOUSEBUTTONDOWN:
								
				if(mFlowMgr->onClick(event.button.button, 
									fMouseX, fMouseY, fMouseZ)){
					break;
				}			
				
				onMouseEvent(event.button.button, SDL_MOUSEBUTTONDOWN); 
				beginDrag();
												
				break;	
				
			case SDL_MOUSEBUTTONUP:
				onMouseEvent(event.button.button, SDL_MOUSEBUTTONUP); 
				endDrag();
				break;	
			
			default:
			    break;
			}
		}

	    /* draw the scene */
	    if (!done){
	    	uint32_t startTime = SDL_GetTicks();
			renderMain();
			updateMain();
			uint32_t endTime = SDL_GetTicks();
			
			uint32_t diff = endTime - startTime;
			fTimeScale = (float)diff / 1000.0f;
			fParticleFPS = fTimeScale;
		}
	}
	
}

/*********************************************
		Camera rotate
**********************************************/
void App::beginDrag(){
	dragStart = Vector2(iMouseX, iMouseY);
	bDrag = true;
}

void App::endDrag(){
	dragStart = Vector2(0, 0);
	bDrag = false;
}



/*********************************************
		Camera movement
**********************************************/
void App::camMove(float x, float y, float z){
	fCameraX += x; 	fCameraY += y; 	fCameraZ += z;	
}
void App::camSetPos(float x, float y, float z){
	fCameraX = x; 	fCameraY = y; 	fCameraZ = z;	
}
void App::camLookAt(float x, float y, float z){
	fLookX = x; fLookY = y; fLookZ = z;
}

