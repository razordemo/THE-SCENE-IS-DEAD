#include <windows.h>
#include <stdio.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "glsl.h"

#define DEBUG 0

GLSL::GLSL():m_pRootShader(NULL),m_pRootFBO(NULL)
	{
	// initialize the error strings
	ms_pShaderCompilationErrorsVS=new char[MAX_ERROR_STRING_LENGTH];
	ms_pShaderCompilationErrorsPS=new char[MAX_ERROR_STRING_LENGTH];
	ms_pShaderLinkErrors=new char[MAX_ERROR_STRING_LENGTH];
	#if DEBUG
	sprintf(ms_pShaderCompilationErrorsVS,"Vertex Shader Error:\n");
	sprintf(ms_pShaderCompilationErrorsPS,"Pixel Shader Error:\n");
	sprintf(ms_pShaderLinkErrors,"Link Error:\n");
	#endif
	}

GLSL::~GLSL()
	{
	// destroy all remaining shaders
	while (m_pRootShader!=NULL) DestroyShader(*m_pRootShader);
	// destroy all remaining FBOs
	while(m_pRootFBO!=NULL) DestroyFBO(*m_pRootFBO);
	// delete compilation/link error strings
	delete[] ms_pShaderCompilationErrorsVS;
	delete[] ms_pShaderCompilationErrorsPS;
	delete[] ms_pShaderLinkErrors;
	}

Shader*	GLSL::CreateShader(WORD _VSResourceID,WORD _PSResourceID,bool _bThrowOnError)
	{
	// load vertex & pixel shader source codes
	char* pVertexShaderSource=LoadShaderResource(_VSResourceID);
	char* pPixelShaderSource=LoadShaderResource(_PSResourceID);
	// create the shader
	Shader*	pResult=CreateShader(pVertexShaderSource,pPixelShaderSource,_bThrowOnError);
	// release the source codes
	delete[] pVertexShaderSource;
	delete[] pPixelShaderSource;
	return pResult;
	}

Shader*	GLSL::CreateShader(const char* _pVertexShaderCode,const char* _pPixelShaderCode,bool _bThrowOnError)
	{
	// create the shader object
	Shader*	pResult=new Shader(_pVertexShaderCode,_pPixelShaderCode,_bThrowOnError);
	// link it to the list of existing shaders
	if(m_pRootShader!=NULL) m_pRootShader->m_pPrevious=pResult;
	pResult->m_pNext=m_pRootShader;
	m_pRootShader=pResult;
	return pResult;
	}

void GLSL::DestroyShader(Shader&_Shader)
	{
	// remove that shader from the list of valid shaders
	if(_Shader.m_pPrevious!=NULL)
		_Shader.m_pPrevious->m_pNext=_Shader.m_pNext;
	else
		m_pRootShader=_Shader.m_pNext;
	if(_Shader.m_pNext!=NULL) _Shader.m_pNext->m_pPrevious=_Shader.m_pPrevious;
	delete &_Shader;
	}

FBO* GLSL::CreateFBO(int _Width,int _Height,GLenum _Format,GLenum _WrapMode,GLenum _FilteringMode)
	{
	return CreateFBOWithDepthBuffer(_Width,_Height,_Format,GL_DEPTH_COMPONENT,_WrapMode,_FilteringMode); //GL_INVALID_INDEX / GL_DEPTH_COMPONENT
	}

FBO* GLSL::CreateFBOWithDepthBuffer(int _Width,int _Height,GLenum _Format,GLenum _DepthFormat,GLenum _WrapMode,GLenum _FilteringMode)
	{
	FBO* pResult=new FBO(_Width,_Height,_Format,_DepthFormat,_WrapMode,_FilteringMode);	// create the FBO object
	if(m_pRootFBO!= NULL) m_pRootFBO->m_pPrevious=pResult;	// link it to the list of existing FBOs
	pResult->m_pNext=m_pRootFBO;
	m_pRootFBO=pResult;
	return pResult;
	}

void GLSL::DestroyFBO(FBO& _FBO)
	{
	if(_FBO.m_pPrevious!=NULL)
		_FBO.m_pPrevious->m_pNext=_FBO.m_pNext;
	else
		m_pRootFBO=_FBO.m_pNext;
	if(_FBO.m_pNext!=NULL) _FBO.m_pNext->m_pPrevious=_FBO.m_pPrevious;
	delete &_FBO;
	}

char*	GLSL::LoadShaderResource(WORD _ResourceID)
	{
	HRSRC hResource=FindResource(NULL,MAKEINTRESOURCE(_ResourceID),"SHADER");
	// load and lock the resource
	HGLOBAL	hBinary=LoadResource(NULL,hResource);
	LPVOID	pData=LockResource(hBinary);
	// copy it and append the missing NULL character terminator
	DWORD	dwSize=SizeofResource(NULL,hResource);
	char*	pShaderSource=new char[dwSize+1];
	memcpy(pShaderSource,pData,dwSize);
	pShaderSource[dwSize]='\0';	// add the terminator
	return pShaderSource;
	}

GLchar*	GLSL::ms_pShaderCompilationErrorsVS=NULL;
GLchar*	GLSL::ms_pShaderCompilationErrorsPS=NULL;
GLchar*	GLSL::ms_pShaderLinkErrors=NULL;

PFNGLCREATESHADERPROC				GLSL::glCreateShader=NULL;
PFNGLSHADERSOURCEPROC				GLSL::glShaderSource=NULL;
PFNGLCOMPILESHADERPROC			GLSL::glCompileShader=NULL;
PFNGLGETSHADERIVPROC				GLSL::glGetShaderiv=NULL;
PFNGLGETSHADERINFOLOGPROC		GLSL::glGetShaderInfoLog=NULL;
PFNGLCREATEPROGRAMPROC			GLSL::glCreateProgram=NULL;
PFNGLATTACHSHADERPROC				GLSL::glAttachShader=NULL;
PFNGLLINKPROGRAMPROC				GLSL::glLinkProgram=NULL;
PFNGLGETPROGRAMIVPROC				GLSL::glGetProgramiv=NULL;
PFNGLGETPROGRAMINFOLOGPROC	GLSL::glGetProgramInfoLog=NULL;
PFNGLDETACHSHADERPROC				GLSL::glDetachShader=NULL;
PFNGLDELETESHADERPROC				GLSL::glDeleteShader=NULL;
PFNGLDELETEPROGRAMPROC			GLSL::glDeleteProgram=NULL;
PFNGLUSEPROGRAMPROC					GLSL::glUseProgram=NULL;
PFNGLGETUNIFORMLOCATIONPROC GLSL::glGetUniformLocation=NULL;
PFNGLUNIFORM1FPROC					GLSL::glUniform1f=NULL;
PFNGLUNIFORM2FPROC					GLSL::glUniform2f=NULL;
PFNGLUNIFORM3FPROC					GLSL::glUniform3f=NULL;
PFNGLUNIFORM4FPROC					GLSL::glUniform4f=NULL;
PFNGLUNIFORM1IPROC					GLSL::glUniform1i=NULL;
PFNGLUNIFORM2IPROC					GLSL::glUniform2i=NULL;
PFNGLUNIFORM3IPROC					GLSL::glUniform3i=NULL;
PFNGLUNIFORM4IPROC					GLSL::glUniform4i=NULL;
PFNGLUNIFORMMATRIX4FVPROC		GLSL::glUniformMatrix4fv=NULL;
PFNGLGETATTRIBLOCATIONPROC	GLSL::glGetAttribLocation=NULL;
PFNGLVERTEXATTRIB1FPROC			GLSL::glVertexAttrib1f=NULL;
PFNGLVERTEXATTRIB2FPROC			GLSL::glVertexAttrib2f=NULL;
PFNGLVERTEXATTRIB3FPROC			GLSL::glVertexAttrib3f=NULL;
PFNGLVERTEXATTRIB4FPROC			GLSL::glVertexAttrib4f=NULL;

PFNGLGENFRAMEBUFFERSPROC					GLSL::glGenFramebuffers=NULL;
PFNGLGENRENDERBUFFERSPROC					GLSL::glGenRenderbuffers=NULL;
PFNGLBINDFRAMEBUFFERPROC					GLSL::glBindFramebuffer=NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC			GLSL::glFramebufferTexture2D=NULL;
PFNGLBINDRENDERBUFFERPROC					GLSL::glBindRenderbuffer=NULL;
PFNGLRENDERBUFFERSTORAGEPROC			GLSL::glRenderbufferStorage=NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC	GLSL::glFramebufferRenderbuffer=NULL;
//PFNGLCLEARDEPTHFPROC							GLSL::glClearDepthf=NULL;
PFNGLDELETEFRAMEBUFFERSPROC				GLSL::glDeleteFramebuffers=NULL;
PFNGLDELETERENDERBUFFERSPROC			GLSL::glDeleteRenderbuffers=NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC		GLSL::glCheckFramebufferStatus=NULL;
PFNGLACTIVETEXTUREPROC						GLSL::glActiveTexture=NULL;
PFNGLDRAWBUFFERSPROC							GLSL::glDrawBuffers=NULL;

void GLSL::HookGLEXTFunctions()
	{
	// shader creation/deletion
	glCreateShader=(PFNGLCREATESHADERPROC) wglGetProcAddress("glCreateShader");
	if(glCreateShader==NULL) throw "Failed to hook OpenGL 2.0 functions.\nIs OpenGL 2.0 available on this machine?\n";
	glDeleteShader=(PFNGLDELETESHADERPROC) wglGetProcAddress("glDeleteShader");
	glAttachShader=(PFNGLATTACHSHADERPROC) wglGetProcAddress("glAttachShader");
	glDetachShader=(PFNGLDETACHSHADERPROC) wglGetProcAddress("glDetachShader");
	glCompileShader=(PFNGLCOMPILESHADERPROC) wglGetProcAddress("glCompileShader");
	glGetShaderiv=(PFNGLGETSHADERIVPROC) wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog=(PFNGLGETSHADERINFOLOGPROC) wglGetProcAddress("glGetShaderInfoLog");
	glShaderSource=(PFNGLSHADERSOURCEPROC) wglGetProcAddress("glShaderSource");
	glCreateProgram=(PFNGLCREATEPROGRAMPROC) wglGetProcAddress("glCreateProgram");
	glDeleteProgram=(PFNGLDELETEPROGRAMPROC) wglGetProcAddress("glDeleteProgram");
	glLinkProgram=(PFNGLLINKPROGRAMPROC) wglGetProcAddress("glLinkProgram");
	glUseProgram=(PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram");
	glGetProgramiv=(PFNGLGETPROGRAMIVPROC) wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog=(PFNGLGETPROGRAMINFOLOGPROC) wglGetProcAddress("glGetProgramInfoLog");
	// uniforms access
	glGetUniformLocation=(PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress("glGetUniformLocationARB");
	glUniform1f=(PFNGLUNIFORM1FPROC) wglGetProcAddress("glUniform1f");
	glUniform2f=(PFNGLUNIFORM2FPROC) wglGetProcAddress("glUniform2f");
	glUniform3f=(PFNGLUNIFORM3FPROC) wglGetProcAddress("glUniform3f");
	glUniform4f=(PFNGLUNIFORM4FPROC) wglGetProcAddress("glUniform4f");
	glUniform1i=(PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
	glUniform2i=(PFNGLUNIFORM2IPROC) wglGetProcAddress("glUniform2i");
	glUniform3i=(PFNGLUNIFORM3IPROC) wglGetProcAddress("glUniform3i");
	glUniform4i=(PFNGLUNIFORM4IPROC) wglGetProcAddress("glUniform4i");
	glUniformMatrix4fv=(PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress("glUniformMatrix4fv");
	// vertex attributes access
	glGetAttribLocation=(PFNGLGETATTRIBLOCATIONPROC) wglGetProcAddress("glGetAttribLocation");
	glVertexAttrib1f=(PFNGLVERTEXATTRIB1FPROC) wglGetProcAddress("glVertexAttrib1f");
	glVertexAttrib2f=(PFNGLVERTEXATTRIB2FPROC) wglGetProcAddress("glVertexAttrib2f");
	glVertexAttrib3f=(PFNGLVERTEXATTRIB3FPROC) wglGetProcAddress("glVertexAttrib3f");
	glVertexAttrib4f=(PFNGLVERTEXATTRIB4FPROC) wglGetProcAddress("glVertexAttrib4f");
	// FBO management
	glGenFramebuffers=(PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffersEXT");
	glBindFramebuffer=(PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebufferEXT");
	glDeleteFramebuffers=(PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffersEXT");
	glGenRenderbuffers=(PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffersEXT");
	glBindRenderbuffer=(PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbufferEXT");
	glDeleteRenderbuffers=(PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffersEXT");
	glFramebufferTexture2D=(PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2DEXT");
	glFramebufferRenderbuffer=(PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbufferEXT");
	glCheckFramebufferStatus=(PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress("glCheckFramebufferStatusEXT");
	glRenderbufferStorage=(PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorageEXT");
	//glClearDepthf=(PFNGLCLEARDEPTHFPROC) wglGetProcAddress("glClearDepthf");
	glActiveTexture=(PFNGLACTIVETEXTUREPROC) wglGetProcAddress("glActiveTextureARB");
	glDrawBuffers=(PFNGLDRAWBUFFERSPROC) wglGetProcAddress("glDrawBuffers");
	}

// shader class implementation
Shader::Shader(const char* _pVertexShaderCode,const char* _pPixelShaderCode,bool _bThrowOnError):m_pPrevious(NULL),m_pNext(NULL),m_bHasErrors(false)
	{
	int	status;
	int log_size=0;
	// compile vertex shader
	m_hVS=GLSL::glCreateShader(GL_VERTEX_SHADER);
	GLSL::glShaderSource(m_hVS,1,&_pVertexShaderCode,NULL);
	GLSL::glCompileShader(m_hVS);
	GLSL::glGetShaderiv(m_hVS,GL_COMPILE_STATUS,&status);
	GLSL::glGetShaderiv(m_hVS,GL_INFO_LOG_LENGTH,&log_size);
	#if DEBUG
	if(log_size>1)
		{
		char *log=(char*)malloc(log_size+1);
		GLSL::glGetShaderInfoLog(m_hVS,log_size,&log_size,log);
		MessageBox(NULL,log,"Vertex shader log",MB_OK|MB_ICONERROR);
		free(log);
		if(_bThrowOnError) throw GLSL::ms_pShaderCompilationErrorsVS;
		}
	#endif
	// compile pixel shader
	m_hPS=GLSL::glCreateShader(GL_FRAGMENT_SHADER);
	GLSL::glShaderSource(m_hPS,1,&_pPixelShaderCode,NULL);
	GLSL::glCompileShader(m_hPS);
	GLSL::glGetShaderiv(m_hPS,GL_COMPILE_STATUS,&status);
	GLSL::glGetShaderiv(m_hPS,GL_INFO_LOG_LENGTH,&log_size);
	#if DEBUG
	if(log_size>1)
		{
		char *log=(char*)malloc(log_size+1);
		GLSL::glGetShaderInfoLog(m_hPS,log_size,&log_size,log);
		MessageBox(NULL,log,"Fragment shader log",MB_OK|MB_ICONERROR);
		free(log);
		if(_bThrowOnError) throw GLSL::ms_pShaderCompilationErrorsVS;
		}
	#endif
	// bind into a single program & link
	m_hProgram=GLSL::glCreateProgram();
	GLSL::glAttachShader(m_hProgram,m_hVS);	// attach vertex shader
	GLSL::glAttachShader(m_hProgram,m_hPS);	// attach pixel shader
	GLSL::glLinkProgram(m_hProgram);				// link program
	GLSL::glGetProgramiv(m_hProgram,GL_LINK_STATUS,&status);
	#if DEBUG
	if(m_bHasErrors|=(status!=GL_TRUE))
		{
	  GLsizei	link_error;
	  GLSL::glGetProgramInfoLog(m_hProgram,MAX_ERROR_STRING_LENGTH-23,&link_error,GLSL::ms_pShaderLinkErrors+23);
	  if(_bThrowOnError) throw GLSL::ms_pShaderLinkErrors;
		}
	#endif
	}

Shader::~Shader()
	{
	// detach shaders
	GLSL::glDetachShader(m_hProgram,m_hVS);
	GLSL::glDetachShader(m_hProgram,m_hPS);
	// destroy everything
	GLSL::glDeleteShader(m_hVS);
	GLSL::glDeleteShader(m_hPS);
	GLSL::glDeleteProgram(m_hProgram);
	}

FBO::FBO(int _Width,int _Height,GLenum _Format,GLenum _DepthFormat,GLenum _WrapMode,GLenum _FilteringMode):m_pPrevious(NULL),m_pNext(NULL)
	{
	m_Width=_Width;
	m_Height=_Height;
	// create FBO
	GLSL::glGenFramebuffers(1,&m_hFBO);
	GLSL::glBindFramebuffer(GL_FRAMEBUFFER,m_hFBO);
	// create color texture
	glGenTextures(1,&m_hTexture);
	GLSL::glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_hTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,_WrapMode);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,_WrapMode);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,_FilteringMode);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,_FilteringMode);
	glTexImage2D(GL_TEXTURE_2D,0,_Format,_Width,_Height,0,GL_BGRA,GL_UNSIGNED_BYTE,NULL);
	// attach 2d texture to this FBO's mip level #0
	GLSL::glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_hTexture,0);
	m_hDepth=GL_INVALID_INDEX;
	if(_DepthFormat!=GL_INVALID_INDEX)
		{
		// create depth render buffer
		GLSL::glGenRenderbuffers(1,&m_hDepth);
		GLSL::glBindRenderbuffer(GL_RENDERBUFFER,m_hDepth);
		GLSL::glRenderbufferStorage(GL_RENDERBUFFER,_DepthFormat,_Width,_Height);
		// attach depth buffer to FBO
		GLSL::glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,m_hDepth);
		}
	// check if FBO is complete
	GLenum status=GLSL::glCheckFramebufferStatus(GL_FRAMEBUFFER);
	// unbind the FBO to return to default back buffer rendering
	UnBind();
	}

FBO::~FBO()
	{
	GLSL::glDeleteFramebuffers(1,&m_hFBO);
	glDeleteTextures(1,&m_hTexture);
	if(m_hDepth!=GL_INVALID_INDEX) GLSL::glDeleteRenderbuffers(1,&m_hDepth);
	}

void FBO::Bind()
	{
	GLSL::glBindFramebuffer(GL_FRAMEBUFFER,m_hFBO);
	}

void FBO::Clear(float _R,float _G,float _B,float _A)
	{
	glClearColor(_R,_G,_B,_A);
	glClear(GL_COLOR_BUFFER_BIT);
	}

void FBO::Clear(float _R,float _G,float _B,float _A,float _Depth)
	{
	glClearColor(_R,_G,_B,_A);
	//GLSL::glClearDepthf(_Depth);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}

void	FBO::UnBind()
	{
	GLSL::glBindFramebuffer(GL_FRAMEBUFFER,NULL);
	}