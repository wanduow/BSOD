#ifndef __GL_DISPLAY_MANAGER__
#define __GL_DISPLAY_MANAGER__

#include "../display_manager.h"

struct CTexture;
class CFont;

class CGLDisplayManager : public CDisplayManager
{
private:
	CTexture		*loadingScreenTex; // should prolly get rid of this...

	// Though OpenGL can be queried for options such as whether GL_TEXTURE2D
	// is enabled, we keep our own tabs on these options.  Apparently it is
	// quicker to do this.  Source: one of the NVidia essays available at
	// nvidia.com/developer
	bool			textureOn;
	bool			blendOn;
	BlendMode		blendMode;

	//
	int				width;
	int				height;
	unsigned int	tri_indices[20000];

public:
	static int meshs_drawn, triangles_drawn;

	virtual int GetNumTrianglesDrawn() { return triangles_drawn; };
	virtual int GetNumMeshesDrawn() { return meshs_drawn; };

	CGLDisplayManager() {
		CDisplayManager::SetWireframe(false);
		CDisplayManager::SetBackfaceCull(true);

		textureOn = true;
		blendOn = false;

		// The following should be setup right at the start by the
		// System Driver anyway.
		width = 800;
		height = 600;
	}

	virtual void Initialise();

	// Texture management
	virtual CDisplayManager::ImageType RequestSupportedImageType(CDisplayManager::ImageType type);
	virtual int  LoadTexture(void *bytes, CDisplayManager::ImageType type, int width, int height);
	virtual void UnloadTexture(string texName);
	virtual void UnloadTexture(CTexture *tex);
	
	// Primitive drawing
	virtual void DrawSphere(Vector3f offset, float width);
	virtual void DrawBox(Vector3f offset, float width);
	virtual void DrawBox(Vector3f offset, Vector3f dims);

	// TEMPORARY
	virtual void DrawActor(CActor *act);
	virtual void DrawTriangleFan(CTriangleFan *fan);

	// NEW INTERFACE BELOW:
	virtual void BeginFrame2();
	virtual void EndFrame2();
        virtual void SetCameraTransform(CCamera &cam);
	virtual void PushMatrix();
	virtual void Translate(Vector3f position);
	virtual void MultMatrix(CMatrix4f &mat);
	virtual void PopMatrix();
	virtual void Rotate(float amount, Vector3f axis);
	/** Bind texture tex to be the texture used for subsequent drawing calls. A NULL 
	 * parameter indicates that texturing should be turned off. */
	virtual void BindTexture(CTexture *tex);
	/** Sets up the colour subsequent primitives are drawn in.  An optional alpha value
	 * allows setting up a default blend value (only used if blending is turned on). */
	virtual void SetColour(float red, float green, float blue, float alpha = 1.0f);
	/** Sets whether subsequent primitives will be rendered with blending enabled. */
	virtual void SetBlend(bool on);
	virtual void SetBlendMode(BlendMode bm);
	virtual BlendMode GetBlendMode();

	// 2D graphics functions
	/** Sets the render state to 2D-mode without depth testing or backface culling.
	 * This mode is suitable for drawing text and user interfaces, for example. */
	virtual void Begin2D();
	/** Draws string str at location (x,y) on the screen.  This function assumes
	 * Begin2D() has been called before it to setup 2D mode.  This modifies:
	 * current colour, current texture. */
	virtual void DrawString2(int x, int y, const string &str);
	virtual void DrawString3(const CFont &fnt, const int x, const int y, const string &str);
	/** Draws a 2D quad on the screen stretching the currently-bound texture over
	 * the entire image, if a texture is bound.  If no texture is bound it will
	 * draw a flat shaded quad in the current colour.  This function assumes
	 * Begin2D() has been called before it to setup 2D mode. */
	virtual void Draw2DQuad(int left, int top, int right, int bottom);
	/** Restored the render state (generally back to 3D mode). */
	virtual void End2D();
	
	// 3D graphics functions
	// 3D mode is the default; there is no needed Begin3D()..End3D() like 2D
	/** Draws triangles in vertex array vertices in order specified by indices.  */
	virtual void DrawIndexedTriangles2(float *vertices, float *tex_coords, unsigned int *indices, int num_triangles);
	/** Draws triangles in vertex array vertices in sequential order. */
	virtual void DrawTriangles2(float *vertices, float *tex_coords, int num_triangles);
	/** Draws triangles in vertex array vertices with colours in sequential order. */
	virtual void DrawTriangles2(float *vertices, float *tex_coords, byte *colours, 
		int num_triangles);
	/** Draws triangles in vertex array vertices in sequential fan order. */
	virtual void DrawTriangleFan2(float *vertices, float *tex_coords, int num_triangles);
	/** Draws triangles in vertex array vertices in sequential strip order. */
	virtual void DrawTriangleStrip(float *vertices, float *tex_coords, int num_triangles);
	//virtual void DrawSphere(Vector3f offset, float radius);
	//virtual void DrawBox(Vector3f offset, float ?);

	// Other misc functions
	/** Called by the System Driver whenever the user resizes the window. This
	 * func just resets the viewport and matrices for a larger window and updates
	 * the internal width and height. */
	virtual void WindowResized(int new_width, int new_height);
	virtual int GetWidth() { return width; }
	virtual int GetHeight() { return height; }

	virtual void SetWireframe(bool w);
	virtual void SetBackfaceCull(bool b);
};

#endif

