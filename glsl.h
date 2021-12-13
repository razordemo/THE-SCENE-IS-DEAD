//////////////////////////////////////////////////////////////////////////
// <PATACODE>
// This file contains the GLSL singleton from which you can :
//	* create or destroy shaders.
//	* create or destroy FBOs (Frame Buffer Objects).
//
// From the shader,you can then setup its variables and use it for drawing.
// From the FBO,you can then bind it or unbind it for rendering
//
// NOTES:
// ------
// * You need at least OpenGL 2.0 to use this class
// * You don't have to worry about memory management as the GLSL singleton will take care of
//	everything for you and won't leave anything behind when exiting the application.
//
#ifndef GLSL_H
#define GLSL_H

#define MAX_ERROR_STRING_LENGTH	1024

// includes for OpenGL 2.0 extensions
#include "glext.h"
#include "wglext.h"

typedef GLint VARID;
typedef GLint ATTRID;
class Shader;
class FBO;

// This little helper will trigger a breakpoint if a GL error occurs...
// Make sure to call BreakOnGLError() to check your GL code...
//
extern void	BreakOnGLError();

// Mini GLSL support class
// From there,you can create shaders & FBOs
class	GLSL
{
protected:	// FIELDS

	// The root element of the linked list of shaders
	Shader* m_pRootShader;

	// The root element of the linked list of FBOs
	FBO* m_pRootFBO;

	// Some useful strings about last shader's compilation errors
	static GLchar* ms_pShaderCompilationErrorsVS;
	static GLchar* ms_pShaderCompilationErrorsPS;
	static GLchar* ms_pShaderLinkErrors;

public:		// METHODS

	GLSL();
	~GLSL();

	void	Init()	{ HookGLEXTFunctions(); }

	// Creates a new GLSL vertex & pixel shader from 2 embeded text resources
	//	_VSResourceID,ID of the vertex shader resource file
	//	_PSResourceID,ID of the pixel shader resource file
	//	_bThrowOnError,true to throw an exception if the shader fails to compile (default). If set to false,then you should check for errors with "Shader.HasErrors()" before using a shader !
	//
	Shader*	CreateShader(WORD _VSResourceID,WORD _PSResourceID,bool _bThrowOnError=true);

	// Creates a new GLSL vertex & pixel shader from 2 GLSL source codes
	//	_pVertexShaderCode,a pointer to the vertex shader source code
	//	_pPixelShaderCode,a pointer to the pixel shader source code
	//	_bThrowOnError,true to throw an exception if the shader fails to compile (default). If set to false,then you should check for errors with "Shader.HasErrors()" before using a shader !
	//
	Shader*	CreateShader(const char* _pVertexShaderCode,const char* _pPixelShaderCode,bool _bThrowOnError=true);

	// Destroys an existing shader
	//
	void	DestroyShader(Shader& _Shader);

	// Creates a new FBO (Frame Buffer Object)
	//	_Width,_Height,the dimensions of the FBO
	//	_Format,the format of the color texture
	//	_WrapMode,the texture addressing mode (e.g. REPEAT,CLAMP,MIRROR)
	//	_FilteringMode,the texture filtering mode (e.g. NEAREST,LINEAR)
	//
	FBO*	CreateFBO(int _Width,int _Height,GLenum _Format=GL_RGBA8,GLenum _WrapMode=GL_REPEAT,GLenum _FilteringMode=GL_NEAREST);

	// Creates a new FBO (Frame Buffer Object) with an associated depth buffer
	//	_Width,_Height,the dimensions of the FBO
	//	_Format,the format of the color texture
	//	_DepthFormat,the format of the depth buffer associated to the FBO
	//	_WrapMode,the texture addressing mode (e.g. REPEAT,CLAMP,MIRROR)
	//	_FilteringMode,the texture filtering mode (e.g. NEAREST,LINEAR)
	//
	FBO*	CreateFBOWithDepthBuffer(int _Width,int _Height,GLenum _Format=GL_RGBA8,GLenum _DepthFormat=GL_DEPTH_COMPONENT24,GLenum _WrapMode=GL_REPEAT,GLenum _FilteringMode=GL_NEAREST);

	// Destroys an existing FBO
	//
	void	DestroyFBO(FBO& _FBO);


protected:

	char*	LoadShaderResource(WORD _ResourceID);
	void	HookGLEXTFunctions();
	// shaders management
	static PFNGLCREATESHADERPROC				glCreateShader;
	static PFNGLSHADERSOURCEPROC				glShaderSource;
	static PFNGLCOMPILESHADERPROC				glCompileShader;
	static PFNGLGETSHADERIVPROC					glGetShaderiv;
	static PFNGLGETSHADERINFOLOGPROC		glGetShaderInfoLog;
	static PFNGLCREATEPROGRAMPROC				glCreateProgram;
	static PFNGLATTACHSHADERPROC				glAttachShader;
	static PFNGLLINKPROGRAMPROC					glLinkProgram;
	static PFNGLGETPROGRAMIVPROC				glGetProgramiv;
	static PFNGLGETPROGRAMINFOLOGPROC		glGetProgramInfoLog;
	static PFNGLDETACHSHADERPROC				glDetachShader;
	static PFNGLDELETESHADERPROC				glDeleteShader;
	static PFNGLDELETEPROGRAMPROC				glDeleteProgram;
	static PFNGLUSEPROGRAMPROC					glUseProgram;
	// uniforms
	static PFNGLGETUNIFORMLOCATIONPROC	glGetUniformLocation;
	static PFNGLUNIFORM1FPROC						glUniform1f;
	static PFNGLUNIFORM2FPROC						glUniform2f;
	static PFNGLUNIFORM3FPROC						glUniform3f;
	static PFNGLUNIFORM4FPROC						glUniform4f;
	static PFNGLUNIFORM1IPROC						glUniform1i;
	static PFNGLUNIFORM2IPROC						glUniform2i;
	static PFNGLUNIFORM3IPROC						glUniform3i;
	static PFNGLUNIFORM4IPROC						glUniform4i;
	static PFNGLUNIFORMMATRIX4FVPROC		glUniformMatrix4fv;
	// vertex attributes
	static PFNGLGETATTRIBLOCATIONPROC		glGetAttribLocation;
	static PFNGLVERTEXATTRIB1FPROC			glVertexAttrib1f;
	static PFNGLVERTEXATTRIB2FPROC			glVertexAttrib2f;
	static PFNGLVERTEXATTRIB3FPROC			glVertexAttrib3f;
	static PFNGLVERTEXATTRIB4FPROC			glVertexAttrib4f;
	// fbo management
	static PFNGLGENFRAMEBUFFERSPROC					glGenFramebuffers;
	static PFNGLGENRENDERBUFFERSPROC				glGenRenderbuffers;
	static PFNGLBINDFRAMEBUFFERPROC					glBindFramebuffer;
	static PFNGLFRAMEBUFFERTEXTURE2DPROC		glFramebufferTexture2D;
	static PFNGLBINDRENDERBUFFERPROC				glBindRenderbuffer;
	static PFNGLRENDERBUFFERSTORAGEPROC			glRenderbufferStorage;
	static PFNGLFRAMEBUFFERRENDERBUFFERPROC	glFramebufferRenderbuffer;
	//static PFNGLCLEARDEPTHFPROC							glClearDepthf;
	static PFNGLDELETEFRAMEBUFFERSPROC			glDeleteFramebuffers;
	static PFNGLDELETERENDERBUFFERSPROC			glDeleteRenderbuffers;
	static PFNGLCHECKFRAMEBUFFERSTATUSPROC	glCheckFramebufferStatus;
	static PFNGLACTIVETEXTUREPROC						glActiveTexture;
	static PFNGLDRAWBUFFERSPROC							glDrawBuffers;

	friend class Shader;
	friend class FBO;
};

// Mini shader class
class	Shader
{
protected:	// FIELDS

	// Internal linked list of shaders
	Shader*		m_pPrevious;
	Shader*		m_pNext;

	GLuint		m_hVS;			// Handle to the vertex shader
	GLuint		m_hPS;			// Handle to the pixel/fragment shader
	GLuint		m_hProgram;	// Handle to the program binding the 2 shaders together
	bool		m_bHasErrors;	// True if the compilation generated errors


public:		// METHODS

	// Checks if the shader had compilation/link errors
	// If you created the shader using _bThrowOnError=false then you should check this to know if you can use a shader.
	// NOTE: Using a shader that has errors will throw an exception !
	bool HasErrors() const { return m_bHasErrors; }

	// Starts using the shader for drawing
	// NOTE: Using a shader that has errors will throw an exception !
	void Use()
		{
		GLSL::glUseProgram(m_hProgram);
		}

	// Stops using the shader for drawing
	// NOTE: You don't have to call Use()/UnUse() in sequences like :
	//	Shader1.Use()/Shader1.UnUse()/Shader2.Use()/Shader2.UnUse()
	// You can simply call :
	//	Shader1.Use()/Shader2.Use()/Shader3.Use()/etc.
	// and only call UnUse() when you want to stop using shaders altogether...
	void UnUse()
		{
		GLSL::glUseProgram(0);
		}

	// Gets a variable's identifier
	// NOTE: A shader variable uses the keyword "uniform" (e.g. uniform vec3 MyVariable;)
	VARID	GetVariableID(const char* _pVariableName)
		{
		VARID	Result=GLSL::glGetUniformLocation(m_hProgram,_pVariableName);
		return Result;
		}

	// Gets a vertex attribute's identifier
	// NOTE: A vertex attribute uses the keyword "attribute" (e.g. attribute vec3 MyVertexAttribute;)
	ATTRID GetAttributeID(const char* _pAttributeName)
		{
		ATTRID Result=GLSL::glGetAttribLocation(m_hProgram,_pAttributeName);
		return Result;
		}

	//////////////////////////////////////////////////////////////////////////
	// Functions to set a uniform variable (cf. http://www.lighthouse3d.com/opengl/glsl/index.php?data)
	// Shader uniforms are "per shader variables" declared like this in the vertex or pixel shader :
	//	uniform vec3	SomeVariable;
	//
	// In C++,you can retrieve its location using :
	//	VARID	vSomeVariable = myShader.GetVariableID("SomeVariable");
	//
	// And set it BEFORE a glBegin() doing something like :
	// 	myShader.SetVar(vSomeVariable,4.0f,2.0f,7.0f);
	//
	// An important note : the values that are set with these functions will keep their values until the shader is destroyed
	// It's no use to set them every frame if they are constants during the entire demo. You only need to assign them once
	//	after the shader is created.
	// Also,the shader must be in use to set a variable !

	// 1,2,3 or 4 floats
	void SetVar(VARID _ID,float v0) { if ( _ID != -1 ) GLSL::glUniform1f(_ID,v0); }
	void SetVar(VARID _ID,float v0,float v1) { if ( _ID != -1 ) GLSL::glUniform2f(_ID,v0,v1); }
	void SetVar(VARID _ID,float v0,float v1,float v2) { if ( _ID != -1 ) GLSL::glUniform3f(_ID,v0,v1,v2); }
	void SetVar(VARID _ID,float v0,float v1,float v2,float v3) { if ( _ID != -1 ) GLSL::glUniform4f(_ID,v0,v1,v2,v3); }

	// 1,2,3 or 4 integers
	void SetVar(VARID _ID,int v0) { if ( _ID != -1 ) GLSL::glUniform1i(_ID,v0); }
	void SetVar(VARID _ID,int v0,int v1) { if ( _ID != -1 ) GLSL::glUniform2i(_ID,v0,v1); }
	void SetVar(VARID _ID,int v0,int v1,int v2) { if ( _ID != -1 ) GLSL::glUniform3i(_ID,v0,v1,v2); }
	void SetVar(VARID _ID,int v0,int v1,int v2,int v3) { if ( _ID != -1 ) GLSL::glUniform4i(_ID,v0,v1,v2,v3); }

	// 4x4 Matrix
	//	_pMatrix is a pointer to an array of 16 floats organized like this :
	//
	//	|M00 M01 M02 M03|   |F00 F01 F02 F03|
	//	|M10 M11 M12 M13|   |F04 F05 F06 F07|
	//	|M20 M21 M22 M23| = |F08 F09 F10 F11|
	//	|M30 M31 M32 M33|   |F12 F13 F14 F15|
	//
	// M are matrix coefficients followed by their ROW and COLUMN 0-based index
	// F are floats from the array,followed by their index in the array
	//
	void SetVar(VARID _ID,float* _pMatrix) { if ( _ID != -1 ) GLSL::glUniformMatrix4fv(_ID,1,GL_FALSE,_pMatrix); }

	// Texture sampler
	//	_ID,the location of the texture sampler
	//	_TextureID,a texture ID resulting from a call to glBindTexture()
	//	_TextureIndex,the index of the texture sampler to assign the texture to (from 0 to 31)
	void SetTexture(VARID _ID,GLuint _TextureID,int _TextureIndex)
		{
		if(_ID==-1) return;
		GLSL::glActiveTexture(GL_TEXTURE0+_TextureIndex);
		//glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,_TextureID);
		GLSL::glUniform1i(_ID,_TextureIndex);
		}

	//////////////////////////////////////////////////////////////////////////
	// Functions to set a vertex attribute (cf. http://www.lighthouse3d.com/opengl/glsl/index.php?data)
	// Vertex attributes are additional "per vertex infos" declared like this in the vertex shader :
	//	attribute vec3	SomeAdditionalVertexInfo;
	//
	// In C++,you can retrieve its location using :
	//	ATTRID	attAdditionalVertexInfo = GetAttributeID("SomeAdditionalVertexInfo");
	//
	// And set it for each vertex doing something like :
	// 	glBegin(GL_TRIANGLES);
	// 		myShader.SetAttrib(attAdditionalVertexInfo,4.0f,2.0f,7.0f);
	// 		glVertex3f(1.0f,1.0f,1.0f);
	// 
	// 		myShader.SetAttrib(attAdditionalVertexInfo,2.0f,9.0f,2.0f);
	// 		glVertex3f(-1.0f,1.0f,1.0f);
	// 
	// 		myShader.SetAttrib(attAdditionalVertexInfo,1.0f,0.0f,5.0f);
	// 		glVertex3f(1.0f,-1.0f,1.0f);
	// 	glEnd();
	//

	// 1,2,3 or 4 floats
	void SetAttrib(ATTRID _ID,float v0)								{ if ( _ID != -1 ) GLSL::glVertexAttrib1f(_ID,v0); }
	void SetAttrib(ATTRID _ID,float v0,float v1)					{ if ( _ID != -1 ) GLSL::glVertexAttrib2f(_ID,v0,v1); }
	void SetAttrib(ATTRID _ID,float v0,float v1,float v2)			{ if ( _ID != -1 ) GLSL::glVertexAttrib3f(_ID,v0,v1,v2); }
	void SetAttrib(ATTRID _ID,float v0,float v1,float v2,float v3)	{ if ( _ID != -1 ) GLSL::glVertexAttrib4f(_ID,v0,v1,v2,v3); }

protected:	// INTERNAL (don't touch !)

	Shader(const char* _pVertexShaderCode,const char* _pPixelShaderCode,bool _bThrowOnError=true);
	~Shader();

	friend class GLSL;
};

// Mini FBO class
class	FBO
{
protected:	// FIELDS

	// Internal linked list of FBOs
	FBO* m_pPrevious;
	FBO* m_pNext;

	// FBO's dimensions
	int m_Width;
	int m_Height;

	GLuint m_hFBO;			// Handle to the FBO
	GLuint m_hTexture;	// Handle to the color buffer
	GLuint m_hDepth;		// Handle to the depth buffer (can be NULL if no depth buffer was requested)

public:		// METHODS

	// Gets the texture ID you can use in shaders to read from a previously rendered to FBO
	// NOTE: The FBO must be unbound using UnBind() before using it as a texture !
	GLuint GetTextureID() const	{ return m_hTexture; }

	// Binds the FBO for rendering
	void Bind();

	// Clears the color FBO
	// NOTE: The FBO must first be bound using Bind() before calling Clear() !
	void Clear(float _R,float _G,float _B,float _A);

	// Clears the color & depth FBO
	// NOTE: The FBO must first be bound using Bind() before calling Clear() !
	void Clear(float _R,float _G,float _B,float _A,float _Depth=1.0f);

	// UnBinds the FBO from rendering (i.e. return to rendering to back buffer)
	void UnBind();


protected:	// INTERNAL (don't touch !)

	FBO(int _Width,int _Height,GLenum _Format=GL_RGBA8,GLenum _DepthFormat=GL_DEPTH_COMPONENT24,GLenum _WrapMode=GL_REPEAT,GLenum _FilteringMode=GL_NEAREST);
	~FBO();

	friend class GLSL;
};

#endif
