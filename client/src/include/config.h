/*******************************************************************************
							BSOD2 Client - config.h
							
 This provides some #defines for compile-time options and defaults for config
 file options. 
*******************************************************************************/

#define DEFAULT_PORT 54567
#define CONFIG_FILE "bsod2.cfg"
#define PARTICLE_FPS fParticleFPS //hack!
#define MAX_FLOW_DESCRIPTORS 64
#define MAX_PARTICLES 1000000 //global cap of 1m particles for particle systems
							  //that have a limit

//screen width, height, and bit depth
//These used to be numbers, in order to keep
//compatibility, they changed to point to the vars in App 
#define SCREEN_WIDTH  App::S()->iScreenX
#define SCREEN_HEIGHT App::S()->iScreenY
#define SCREEN_BPP     32
#define SCREEN_FULLSCREEN App::S()->bFullscreen

#ifndef PI
	#define PI 3.14159265
#endif

#define SLAB_SIZE 40

#define GUI_HIDE_DELAY 5.0f

//Toggles between std::map and unordered_map
#define USE_TR1

#define CAMERA_SPEED 0.5f

const float infinity = std::numeric_limits<float>::infinity();
