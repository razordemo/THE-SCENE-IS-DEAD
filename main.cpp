#define WIN32_LEAN_AND_MEAN
#define DEBUG 0					// debug flag //#if defined(_DEBUG)
#define SHADER 1				// shader flag
#define MUSIC DEBUG?0:1 // music flag
#define PI 3.1415926535f// pi
#define PID PI/180.0f		// pi ratio
#define CR 1.0f/256.0f	// color ratio
#define TIME 1.92f

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "glsl.h"
#include "resource.h"
#include "timer.h"
#if MUSIC
#include "v2mplayer.h"
#include "libv2.h"
#include "music.h"
#endif

bool done=false;

Timer *timer;
float timer_global=0;
float timer_global_previous=0;
float timer_fps=0;
float timer_fps_total=0;
float timer_fps_average=0;
float timer_fps_min=32768;
float timer_fps_max=0;
float timer_music=0;
float timer_buffer=0;
int frame_counter=0;

//char *param=GetCommandLine();

#if MUSIC
static V2MPlayer player;
#endif
bool music_play=false;	// flag
int music_order=0;			// pseudo protracker pattern order
int music_line=0;				// pseudo protracker pattern line
int music_line_buffer=0;// keep previous variable

int screen_n=0;					// screen current number
const int screen_max=21;// number of screen
int screen_i=(MUSIC==1)?0:20;// screen counter
int screen_start[]= {0,16,18,19, 1,7,12,2,3,8,10,13,14,4,15,6,20, 9,5,17,21};	// screen start
int screen_length[]={0, 1, 1, 1,24,4, 4,8,8,8, 4, 4, 2,8, 8,4, 4,16,7, 2, 8};	// screen length
float screen_timer[screen_max];	// screen timer

HDC				hDC=NULL;			// GDI device context
HGLRC			hRC=NULL;			// rendering context
HWND			hWnd=NULL;		// window handle
HINSTANCE	hInstance;		// instance application

int keys[256];					// keyboard array
int active=true;				// window active flag
bool fullscreen=DEBUG?false:true;	// fullscreen flag
bool pause=false;				// pause flag
float nearplane=0.1f;		// nearplane
float farplane=1000.0f;	// farplane
bool polygon=true;			// polygon mode
int polygon_fillmode=GL_FILL;// fill mode
int ratio_2d=2;					// 2d ratio
/* window variable			*/
int base_w=800;					// base width
int base_h=450;					// base height
int window_w=base_w;		// width
int window_h=base_h;		// height
int screen_w;						// screen width
int screen_h;						// screen height
int window_color=32;		// color depth
float screen_r;					// color red
float screen_g;					// color green
float screen_b;					// color blue
/* object variable			*/
float	main_angle;				// main angle
float	part_angle;				// part angle
float fx_angle;					// current angle
/* liner variable				*/
bool liner_flag=false;	// flag
bool liner_write=true;	// write flag
int car;								// ascii code
int car_cursor=126;			// cursor ascii code
int liner_length;				// text length
int liner_line_n;				// line number
int liner_line;					// line increment
int liner_max;					// line max length
int liner_i;						// char increment
int liner_count;				// counter
float liner_w;					// width
float liner_h;					// height
float liner_r;					// color red
float liner_g;					// color green
float liner_b;					// color blue
float liner_x;					// position x
float liner_y;					// position y
float liner_z;					// position z
float liner_p_x;				// relative x
float liner_p_y;				// relative y
float liner_p_z;				// relative z
float liner_a_x;				// angle x
float liner_a_y;				// angle y
float liner_a_z;				// angle z
float liner_angle;			// angle
float liner_vtx[]={1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f}; //,-1.0f,-1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f};
int greetings_i;				// greetings value
int greetings_n=16;			// greetings number
/* text variable				*/
char *name="Razor 1911 - The Scene Is Dead";
char const *txt_dos[]={
"\rCopyright \x05e\x031\x039\x038\x037 Commodore-Amiga, Inc.\rAll rights reserved.\rRelease 1.3\r@",
"\r\r                      .::::: .::::::::::.  .:::::  .:::::\r   .  . .. ............:::::.:::::..:::::...:::::...:::::........... .. .  .\r       _ __ ___________ __________ __________ __________ __________\r           \\\\_______  ¼\\_______  ¼\\_______  ¼\\__    _  ¼\\_______  ¼\\_ __ _\r             /    _  ___/    _     /    ______/     /    /    _  ___//\r            /     /   \\/     /    /     /    /     /    /     /   \\\r           /     /    /     /    /     /    /     /    /     /    /\r          /_____/    /_____/    /_____     /_____     /_____/    /\r              /_____/    /_____/    /_____/    /_____/    /_____/kRm\r   .  . .. ......................................................... .. .  .\r                       ::::: :::::  :::::   :::::   :::::\r                       ::::: `:::::::::::   :::::   :::::\r                       :::::        :::::   :::::   :::::\r                       ::::: :::::::::::'   :::::   :::::\r\r                  gives you \"THE SCENE IS DEAD\" A Non-AGA demo!\r                         Release Date 8th of April 2012\r\r       If you only have 1.0 MB memory then turn off all external drives.\r           If you have OS 2.x and only 1.0 MB the demo may not run!\r@",
"\r\r  /\\______  /\\______  ____/\\______     __\r  \\____   \\/  \\__   \\/  _ \\____   \\ __/  \\____\r   / _/  _/    \\/   /   /  / _/  _// / / / / /\r  /  \\   \\  /\\ /   /\\  /  /  \\   \\/ /\\  / / /\r  \\__/\\   \\/RTX______\\___/\\__/\\   \\/ / /_/_/\r  =====\\___)===\\__)============\\___)=\\/=======\r  Razor 1911 - Sharpening the blade since 1985\r\r@"};
//"\r\r                                      _____\r                     _ _ ___________ _\\_  /_ _______ _____ _\r                    .\\\\\\Y   _X___  \x07fY   _/ \x07fY   __ \x07fY   _///.\r              _ ___     |   |  \\|   |   |/  |   |/  |   |     ___ _\r              \\Y  /_  _ |   |   l   |   l   |   l   |   | _  _\\  Y/\r               l_/// (//l___l_______l_______l_______l___|\\\\) \\\\\\_|\r                \x07f       .  \x07f      _\x07f       \x07f       \x07f _,\x07f!       \x07f\r                        !      ___\\_        _____    \\__\r                       _____   \\__  \\_      \\__  \\_ ___/_\r                       \\__  \\_  \x07f/   /_  sK  \x07f/   /\\\\__  \\_\r                        \x07f/   /\\ /  _   \\     /   /  \\\x07f/   /\\\r                        /   /  /   /   /\\   /   /   //   /  \\\r                       /   /  /   /   /  \\ /   /   //   /   /\r                      /   /   \\__    /   //   /   //   /   /\r                     /   /   /  /   /   //   /   //   /   /\r                    /_  /__ /  /_  /__ //_  /__ //_  /__ /\r                     /____/\\   \\/____/\\ \\/____/\\ \\/____/\\\r                     \\    \\ \\   \\    \\ \\ \\    \\ \\ \\    \\ \\\r                      \\____\\/    \\____\\/  \\____\\/  \\____\\/\r                          \x07f          \x07f        \x07f        \x07f\r                        :                               ·\r              _ __      |   r  a  z  o  r  1  9  1  1   |      __ _\r              \\Y  \\ _   |                               |   _ /  Y/\r               |___\\\\\\. l_______________________________| .///___|\r                  \x07f                                    \x07f        \x07f\r@"};
char const *txt_skull={"\r we are back@"};
char const *txt_title={"\r   the\r  scene\r is dead@"};
char const *txt_razor={"\r     released\r        by\r\r**             **\r *             * \r\r        at\r     revision\r       2012@"};
char const *txt_credits[]={"\rprogram\r  rez @","\r music \rdubmood@"};
char const *txt_greetings[]={"\r greetings to elites \r fuckings to lamers!@","\r 3ln@","\r accession@","\r andromeda@","\r ate bit@","\r bomb@","\r ctrl-alt-test@","\r d-bug@","\r equinox@","\r f4cg@","\r frequency@","\r live!@","\r paradox@","\r photonstorm@","\r scenesat@","\r trsi@","\r vision factory@"};
char const *txt_end={"\r\r the scene is dead@"};
char const *txt="";
/* fs_quad variable			*/
float fs_quad_vtx[]={-1.0f,1.0f,0,1.0f,-1.0f,-1.0f,0,1.0f,1.0f,-1.0f,0,1.0f,1.0f,1.0f,0,1.0f};
float fs_quad_tex[]={0,1.0f,0,0,1.0f,0,1.0f,1.0f};
/* flash variable				*/
bool flash_flag=false;	// flag
float flash_angle=0;		// angle
float flash_value=0;		// value
float flash_speed;			// speed
/* fade variable				*/
bool fade_flag=false;		// flag
float fade_angle=0;			// angle
float fade_value=0;			// value
float fade_speed;				// speed
/* debug variable				*/
bool debug_flag=DEBUG;	// flag
int debug_x;						// position x
int debug_y;						// position y
int debug_w;						// width
int debug_h;						// height
int debug_vtx[8];				// vertex array
/* sync1 variable				*/
bool sync1_flag=false;	// flag
float sync1_angle=0;		// angle
float sync1_value=0;		// value
float sync1_speed;			// speed
/* sync2 variable				*/
bool sync2_flag=false;	// flag
float sync2_angle=0;		// angle
float sync2_value=0;		// value
float sync2_speed;			// speed
/* beat variable				*/
bool beat_flag=false;		// flag
float beat_angle=0;			// angle
float beat_value=0;			// value
float beat_speed;				// speed
/* corner variable			*/
const int corner_n=10;	// polygon number
int corner_w=24;				// radius
int corner_vtx[corner_n*6];// vertex array
/* dos variable					*/
bool dos_flag=false;		// flag
int dos_w;							// width
int dos_h;							// height
int dos_m;							// margin
int dos_tab;						// text tabulation
float dos_r;						// red value
float dos_g;						// green value
float dos_b;						// blue value
int dos_vtx[8];					// vertex array
int shell_vtx[8];				// vertex array
float shell_tex[]={1.0f,0.96875f,1.0f,1.0f,0.75f,1.0f,0.75f,0.96875f};
bool cursor_flag=false;	// flag
/* decrunch variable		*/
bool decrunch_flag=false;// flag
int decrunch_h=0;				// height
int decrunch_y=0;				// top
int decrunch_split=0;		// bar split random
int decrunch_split_w;		// bar split w
int decrunch_color=64;	// base color
/* cube variable				*/
float cube_vtx[144];		// vertex array
/* logo variable				*/
bool logo_flag=false;		// flag
int logo_x;							// position x
int logo_y;							// position y
int logo_w;							// width
int logo_h;							// height
int logo_vtx[16];				// vertex array
float logo_tex[]={0.1015625f,0,0,0,0,0.1796875f,0.1015625f,0.1796875f,0,0,0.1015625f,0,0.1015625f,0.1796875f,0,0.1796875f};
/* razor variable				*/
bool razor_flag=false;	// flag
int razor_x;						// position x
int razor_y;						// position y
int razor_w;						// width
int razor_h;						// height
float razor_alpha;			// color alpha
int razor_vtx[8];				// vertex array
float razor_tex[]={1.0f,0.13671875f,0.5859375f,0.13671875f,0.5859375f,0,1.0f,0};
/* bar variable					*/
const int bar_n=8;			// bar number
int bar_i=-1;						// counter
int bar_x;							// position x
int bar_y;							// position y
int bar_w;							// width
int bar_h;							// height
int bar_vtx[bar_n];			// vertex array
float bar_angle[bar_n];	// angle
float bar_speed[bar_n];	// speed
/* skull variable				*/
bool skull_flag=false;	// flag
int skull_x;						// position x
int skull_y;						// position y
int skull_w;						// width
int skull_h;						// height
int skull_vtx[8];				// vertex array
float skull_tex[]={0.046875f,1.0f,0,1.0f,0,0.953125f,0.046875f,0.953125f};
float skull_r=0.75f;		// color red
float skull_g=0.5f;			// color green
float skull_b=0.0f;			// color blue
/* triforce variable		*/
bool triforce_flag=false;// flag
float triforce_vtx[36];	// vertex array
int triforce_x;					// position x
int triforce_y;					// position y
/* common variable			*/
int i,j,k,l;
float x,y,z,w,h;
float r,g,b,c;
float value,angle,radius,scale,speed;
int tex_map;
/* shader variable			*/
bool shader_flag=SHADER?true:false;
bool shader_blur_flag=SHADER?true:false;
bool shader_effect_flag=SHADER?true:false;
bool shader_glow_flag=true;
bool shader_radial_flag=false;
bool shader_copper_flag=SHADER?true:false;
float copper_n;					// copper amount
float copper_alpha;			// color
float copper_speed;			// scroll speed
float copper_limit;			// y limit
bool shader_wave_flag=false;
bool shader_render_flag=false;
bool shader_cloud_flag=false;
bool shader_tunnel_flag=false;
bool shader_tube_flag=false;
bool shader_disc_flag=false;
int disc_type=0;
bool shader_title_flag=false;
bool shader_tower_flag=false;
bool shader_sphere_flag=false;
bool shader_dark_flag=false;
bool shader_land_flag=false;
bool shader_postfx_flag=SHADER?true:false;
int glow_fbo_size=3;		// glow fbo size
int glow_color;					// 0=color/1=alpha
float glow_smoothstep;	// glow smoothstep value
float glow_mix_ratio1;	// glow mixing ratio
float glow_mix_ratio2;	// source mixing ratio
float radial_value1;
float radial_value2;
float radial_color;			// color
bool postfx_scanline=true;
float postfx_deform;		// deformation ratio
#if SHADER
GLSL glsl;
Shader* shader_simple;
Shader* shader_blur_h;
Shader* shader_blur_v;
Shader* shader_glow;
Shader* shader_radial;
Shader* shader_copper;
Shader* shader_wave;
Shader* shader_render;
Shader* shader_cloud;
Shader* shader_tunnel;
Shader* shader_tube;
Shader* shader_disc;
Shader* shader_title;
Shader* shader_tower;
Shader* shader_sphere;
Shader* shader_dark;
Shader* shader_land;
Shader* shader_postfx;
// shader variables
VARID shader_simple_texture;
VARID shader_blur_h_texture;
VARID shader_blur_h_screen_w;
VARID shader_blur_h_screen_h;
VARID shader_blur_h_time;
VARID shader_blur_h_value;
VARID shader_blur_v_texture;
VARID shader_blur_v_screen_w;
VARID shader_blur_v_screen_h;
VARID shader_blur_v_time;
VARID shader_blur_v_value;
VARID shader_glow_texture;
VARID shader_glow_texture_prv;
VARID shader_glow_screen_w;
VARID shader_glow_screen_h;
VARID shader_glow_time;
VARID shader_glow_step;
VARID shader_glow_value1;
VARID shader_glow_value2;
VARID shader_radial_texture;
VARID shader_radial_screen_w;
VARID shader_radial_screen_h;
VARID shader_radial_time;
VARID shader_radial_value1;
VARID shader_radial_value2;
VARID shader_radial_color;
VARID shader_copper_texture;
VARID shader_copper_screen_w;
VARID shader_copper_screen_h;
VARID shader_copper_time;
VARID shader_copper_n;
VARID shader_copper_alpha;
VARID shader_copper_speed;
VARID shader_copper_limit;
VARID shader_wave_texture;
VARID shader_wave_screen_w;
VARID shader_wave_screen_h;
VARID shader_wave_time;
VARID shader_wave_value;
VARID shader_render_resolution;
VARID shader_render_time;
VARID shader_render_value;
VARID shader_cloud_resolution;
VARID shader_cloud_time;
VARID shader_cloud_value;
VARID shader_tunnel_resolution;
VARID shader_tunnel_time;
VARID shader_tunnel_value;
VARID shader_tube_resolution;
VARID shader_tube_time;
VARID shader_tube_value;
VARID shader_disc_resolution;
VARID shader_disc_time;
VARID shader_disc_type;
VARID shader_disc_value;
VARID shader_disc_sync;
VARID shader_title_resolution;
VARID shader_title_time;
VARID shader_title_value;
VARID shader_tower_resolution;
VARID shader_tower_time;
VARID shader_tower_value;
VARID shader_sphere_resolution;
VARID shader_sphere_time;
VARID shader_sphere_value;
VARID shader_dark_resolution;
VARID shader_dark_time;
VARID shader_dark_value;
VARID shader_land_resolution;
VARID shader_land_time;
VARID shader_land_value1;
VARID shader_land_value2;
VARID shader_postfx_texture;
VARID shader_postfx_texture_2d;
VARID shader_postfx_screen_w;
VARID shader_postfx_screen_h;
VARID shader_postfx_time;
VARID shader_postfx_flash;
VARID shader_postfx_value;
VARID shader_postfx_deform;
VARID shader_postfx_scanline;
VARID shader_postfx_sync;
FBO* fbo_back;
FBO* fbo_front;
FBO* fbo_render;
FBO* fbo_blur_h;
FBO* fbo_blur_v;
FBO* fbo_ping;
FBO* fbo_pong;
#endif

LRESULT	CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);	// wndProc declaration

static PIXELFORMATDESCRIPTOR pfd=
	{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,											// version number
	PFD_DRAW_TO_WINDOW|			// format must support window
	PFD_SUPPORT_OPENGL|			// format must support openGL
	PFD_DOUBLEBUFFER,				// must support double buffering
	PFD_TYPE_RGBA,					// request an RGBA format
	window_color,						// select our color depth
	0,0,0,0,0,0,						// color bits ignored
	0,											// no alpha buffer
	0,											// shift bit ignored
	0,											// no accumulation buffer
	0,0,0,0,								// accumulation bits ignored
	16,											// z-buffer (depth buffer)
	0,											// no stencil buffer
	0,											// no auxiliary buffer
	PFD_MAIN_PLANE,					// main drawing layer
	0,											// reserved
	0,0,0										// layer masks ignored
	};

int load_tex(WORD file,GLint clamp,GLint mipmap)
	{
	HBITMAP hBMP;	// bitmap handle
	BITMAP BMP;		// bitmap structure
	hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(file),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
	if(!hBMP) return 0;
	GetObject(hBMP,sizeof(BMP),&BMP);
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	GLuint TexID;
	glGenTextures(1,&TexID);
 	glBindTexture(GL_TEXTURE_2D,TexID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,clamp);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,clamp);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,mipmap);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,mipmap);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP,GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,BMP.bmWidth,BMP.bmHeight,0,GL_BGR,GL_UNSIGNED_BYTE,BMP.bmBits);
	DeleteObject(hBMP);
	return TexID;
	}

void init_viewport(int type)
	{
	glViewport(0,0,screen_w,screen_h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(type==0) gluPerspective(70,(float)((float)screen_w/(float)screen_h),nearplane,farplane); else gluOrtho2D(0,screen_w,screen_h,0);
	glMatrixMode(GL_MODELVIEW);
	}

#if SHADER
void init_shader()
	{
	glDisable(GL_BLEND);
	glVertexPointer(4,GL_FLOAT,0,fs_quad_vtx);
	glTexCoordPointer(2,GL_FLOAT,0,fs_quad_tex);
	}

void fs_quad()
	{
	glLoadIdentity();
	glDrawArrays(GL_QUADS,0,4);
	}

void draw_shader_simple(FBO* fbo_output,int n)
	{
	shader_simple->Use();
	shader_simple->SetTexture(shader_simple_texture,fbo_output->GetTextureID(),n);
	fs_quad();
	shader_simple->UnUse();
	}
#endif

int rand()
	{
	static int lastrand=3489715357;
	__asm
		{
		mov		eax,[lastrand]
		mov		edx,eax
		rcr		edx,11
		adc		eax,17
		xor		ax,dx
		imul	eax,edx
		xor		eax,edx
		mov		[lastrand],eax
		and		eax,0xffff
		}
	return (int)lastrand;
	}

void calc_txt()
	{
	liner_angle=main_angle;
	liner_length=strlen(txt);
	liner_count=0;
	liner_line_n=0;
	liner_max=0;
	liner_i=0;
	for(i=0;i<liner_length;i++)
		{
		if((unsigned char)txt[i]!=13)
			{
			liner_i++;
			}
		else
			{
			if(liner_i>liner_max) liner_max=liner_i;
			liner_line_n++;
			liner_i=0;
			}
		}
	if(liner_i>liner_max) liner_max=liner_i;
	}

void flash()
	{
	flash_flag=true;
	flash_angle=main_angle;
	}

void fade()
	{
	fade_flag=true;
	fade_angle=main_angle;
	}

void sync1()
	{
	sync1_flag=true;
	sync1_angle=main_angle;
	}

void sync2()
	{
	sync2_flag=true;
	sync2_angle=main_angle;
	}

void beat()
	{
	beat_flag=true;
	beat_angle=main_angle;
	}

void rectangle(int x,int y,int w,int h)
	{
	glLoadIdentity();
	glBegin(GL_QUADS);
		glVertex2i(x+w,y  );
		glVertex2i(x  ,y  );
		glVertex2i(x  ,y+h);
		glVertex2i(x+w,y+h);
	glEnd();
	}

void corner()
	{
	float vertex[2+corner_n*2];
	vertex[0]=0;
	vertex[1]=0;
	for(i=1;i<corner_n+1;i++)
		{
		j=i*2;
		float a=PID*90.0f/(corner_n-1)*(i-1);
		vertex[j  ]=corner_w-corner_w*cosf(a);
		vertex[j+1]=corner_w-corner_w*sinf(a);
		}
	for(i=0;i<corner_n;i++)
		{
		j=i*6;
		k=i*2;
		corner_vtx[j  ]=(int)vertex[0];
		corner_vtx[j+1]=(int)vertex[1];
		corner_vtx[j+2]=(int)vertex[2+k];
		corner_vtx[j+3]=(int)vertex[3+k];
		corner_vtx[j+4]=(int)vertex[4+k];
		corner_vtx[j+5]=(int)vertex[5+k];
		}
	}

void triforce(int radius2)
	{
	int radius1=radius2/2;
	float x1=radius1*cosf(PID*330.0f);
	float y1=radius1*sinf(PID*330.0f);
	float x2=radius1*cosf(PID*210.0f);
	float y2=radius1*sinf(PID*210.0f);
	float x3=radius1*cosf(PID* 90.0f);
	float y3=radius1*sinf(PID* 90.0f);
	float x4=radius2*cosf(PID*270.0f);
	float y4=radius2*sinf(PID*270.0f);
	float x5=radius2*cosf(PID*150.0f);
	float y5=radius2*sinf(PID*150.0f);
	float x6=radius2*cosf(PID* 30.0f);
	float y6=radius2*sinf(PID* 30.0f);
	float vertex[]={x1,y1,x4,y4,x2,y2,x2,y2,x5,y5,x3,y3,x3,y3,x6,y6,x1,y1,x2,y2,x4,y4,x1,y1,x3,y3,x5,y5,x2,y2,x1,y1,x6,y6,x3,y3};
	for(i=0;i<36;i++) triforce_vtx[i]=vertex[i];
	}

void bar(float speed)
	{
	bar_i++;
	if(bar_i>bar_n-1) bar_i=0;
	bar_angle[bar_i]=main_angle;
	bar_speed[bar_i]=speed;
	}

void greetings()
	{
	greetings_i++;
	if(greetings_i>greetings_n) greetings_i=0;
	txt=txt_greetings[greetings_i];
	calc_txt();
	sync2();
	}

void screen(int n)
	{
	screen_n=n;
	screen_r=0;
	screen_g=0;
	screen_b=0;
	flash_speed=1.5f;
	fade_speed=0.2f;
	sync1_flag=false;
	sync1_value=1.0f;
	sync1_speed=1.0f;
	sync2_flag=false;
	sync2_value=0;
	sync2_speed=1.0f;
	beat_speed=2.0f;
	decrunch_flag=false;
	liner_flag=false;
	liner_write=false;
	cursor_flag=false;
	liner_w=1.6f;
	liner_h=2.0f;
	liner_r=1.0f;
	liner_g=1.0f;
	liner_b=1.0f;
	liner_x=0;
	liner_y=0;
	liner_z=-16.0f;
	liner_p_x=0;
	liner_p_y=0;
	liner_p_z=0;
	liner_a_x=0;
	liner_a_y=0;
	liner_a_z=0;
	dos_flag=false;
	dos_tab=0;
	dos_r=CR*56;
	dos_g=CR*48;
	dos_b=CR*64;
	triforce_flag=false;
	logo_flag=false;
	razor_flag=false;
	razor_alpha=1.0f;
	skull_flag=false;
	shader_effect_flag=true;
	shader_glow_flag=true;
	glow_color=1;
	glow_smoothstep=0.025f;
	glow_mix_ratio1=0.5f;
	glow_mix_ratio2=0.5f;
	shader_radial_flag=false;
	radial_value1=2.0f;
	radial_value2=0.8f;
	radial_color=0;
	shader_copper_flag=false;
	copper_n=8.0f;
	copper_alpha=0.875f;
	copper_speed=0.25f;
	copper_limit=1.0f;
	shader_wave_flag=false;
	shader_render_flag=false;
	shader_cloud_flag=false;
	shader_tunnel_flag=false;
	shader_tube_flag=false;
	shader_disc_flag=false;
	disc_type=0;
	shader_title_flag=false;
	shader_tower_flag=false;
	shader_sphere_flag=false;
	shader_dark_flag=false;
	shader_land_flag=false;
	postfx_deform=0.5f;
	switch(n)
		{
		case 0: // loading
			break;
		case 16: case 18: case 19: // loading
			part_angle=main_angle;
			if(n==19) decrunch_flag=true;
			dos_flag=true;
			if(n==16) cursor_flag=true;
			txt=txt_dos[(n==16)?0:1];
			dos_tab=(n==18||n==19)?screen_w/2-2*ratio_2d-dos_m-83*dos_w:0;
			screen_r=dos_r;
			screen_g=dos_g;
			screen_b=dos_b;
			if(n==16) // music
				{
				#if MUSIC
				player.Stop();
				player.Play();
				while(dsGetCurSmp()<0.0f) {}
				#endif
				music_play=true;
				timer_buffer=timer_global;
				}
			break;
		case 1: // intro
			part_angle=main_angle;
			shader_copper_flag=true;
			shader_wave_flag=true;
			logo_flag=true;
			for(i=0;i<bar_n;i++)
				{
				bar_angle[i]=0;
				bar_speed[i]=1.0f;
				}
			flash();
			break;
		case 2: // render
			part_angle=main_angle;
			shader_render_flag=true;
			flash();
			sync1_speed=0.25f;
			sync1();
			break;
		case 3: // cloud
			part_angle=main_angle;
			shader_cloud_flag=true;
			flash();
			sync1_speed=0.25f;
			sync1();
			break;
		case 4: // tunnel
			part_angle=main_angle;
			shader_tunnel_flag=true;
			flash();
			sync1();
			break;
		case 5:	case 17: // end
			if(n==5) part_angle=main_angle;
			shader_copper_flag=true;
			copper_n=24.0f;
			copper_limit=0.525f;
			copper_speed=0;
			shader_land_flag=true;
			razor_flag=true;
			razor_y=(screen_h-razor_h)/2+8*ratio_2d;
			triforce_flag=true;
			triforce_x=screen_w/2;
			triforce_y=screen_h/2+2*ratio_2d;
			liner_flag=true;
			txt=txt_end;
			liner_p_y=-4.575f;
			liner_p_z=-7.0f;
			if(n==5)
				{
				flash();
				sync1_speed=0.25f;
				sync1();
				}
			else
				{
				fade();
				sync1_value=0;
				sync2_speed=fade_speed;
				sync2();
				}
			break;
		case 6:	case 20: // disc
			if(n==6) part_angle=main_angle;
			shader_disc_flag=true;
			shader_radial_flag=true;
			radial_value1=0.625f;
			radial_value2=0.9875f;
			if(n==6)
				{
				liner_flag=true;
				txt=txt_greetings[0];
				liner_p_y=4.0f;
				liner_p_z=-4.0f;
				liner_a_x=-45.0f;
				liner_r=0.875f;
				liner_g=0.625f;
				liner_b=0.375f;
				}
			else
				{
				radial_color=0.0625f;
				disc_type=1;
				}
			flash();
			sync1_value=0;
			//sync1_speed=1.5f;
			sync1();
			break;
		case 7: case 12: // title
			if(n==7) part_angle=main_angle;
			shader_title_flag=true;
			shader_copper_flag=true;
			if(n==7)
				{
				copper_n=12.0f;
				copper_speed=0.5f;
				txt=txt_title;
				liner_p_y=-3.0f;
				liner_p_z=3.0f;
				liner_a_x=-45.0f;
				}
			else
				{
				copper_n=24.0f;
				copper_speed=0.5f;
				triforce_flag=true;
				triforce_x=screen_w/2;
				triforce_y=screen_h/2-6*ratio_2d;
				razor_flag=true;
				razor_y=(screen_h-razor_h)/2;
				txt=txt_razor;
				liner_p_y=-1.0f;
				liner_p_z=-14.0f;
				}
			liner_flag=true;
			flash();
			sync1_value=0;
			break;
		case 8: // sphere
			part_angle=main_angle;
			shader_sphere_flag=true;
			shader_radial_flag=true;
			radial_value1=0.75f;
			radial_value2=1.0f;
			radial_color=0.0625f;
			sync1_speed=0.0625f;
			sync1();
			flash();
			break;
		case 10: case 13: case 14: // credits
			if(n==8) part_angle=main_angle;
			shader_tower_flag=true;
			shader_radial_flag=true;
			radial_value1=0.75f;
			radial_value2=1.0f;//0.9875f;
			if(n==10||n==13)
				{
				liner_flag=true;
				txt=txt_credits[(n==10)?0:1];
				sync1_speed=0.1275f;
				sync1();
				}
			if(n==14) { sync2_speed=0.1f; sync2(); }
			if(n!=14) flash();
			break;
		case 9: // dark
			part_angle=main_angle;
			shader_dark_flag=true;
			shader_radial_flag=true;
			radial_value1=0.625f;
			radial_value2=0.9875f;
			radial_color=0.0625f;
			sync1_speed=0.01875f;
			sync1();
			flash();
			break;
		case 11: // we are back
			part_angle=main_angle;
			skull_flag=true;
			shader_radial_flag=true;
			radial_value1=0.875f;
			radial_value2=0.9875f;
			radial_color=0.0625f;
			liner_flag=true;
			txt=txt_skull;
			liner_r=skull_r;
			liner_g=skull_g;
			liner_b=skull_b;
			liner_p_y=-1.0f;
			liner_p_z=-14.0f;
			//flash();
			break;
		case 15: // greetings
			part_angle=main_angle;
			shader_tube_flag=true;
			//shader_radial_flag=true;
			radial_value1=1.0f;
			radial_value2=0.9875f;
			liner_flag=true;
			greetings_i=0;
			sync2_speed=0.75f;
			txt="@";
			flash();
			sync1();
			break;
		case 21: // exit
			part_angle=main_angle;
			dos_flag=true;
			cursor_flag=true;
			txt=txt_dos[2];
			screen_r=dos_r;
			screen_g=dos_g;
			screen_b=dos_b;
			break;
		}
	glClearColor(screen_r,screen_g,screen_b,1.0f);
	calc_txt();
	}

int InitGL(void)
	{
	glClearDepth(1.0f);								// set depth buffer
	glDepthMask(GL_TRUE);							// do not write z-buffer
	glEnable(GL_CULL_FACE);						// disable cull face
	glCullFace(GL_BACK);							// don't draw front face
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	// load texture
 	tex_map=load_tex(IDB_MAP,GL_CLAMP,GL_NEAREST);//_MIPMAP_NEAREST);
	// initialize some variable
	timer=new Timer();
	for(i=0;i<screen_max;i++) screen_timer[i]=((i>0)?screen_timer[i-1]:0)+screen_length[i]*TIME;
	screen(screen_start[screen_i]);
	// initialize music
	#if MUSIC
	player.Init();
	player.Open(music);
	dsInit(player.RenderProxy,&player,GetForegroundWindow());
	#endif
	/*-PATACODE----------------------------------------------------------------*/
	#if SHADER
	if(shader_flag)
		{
		try
			{
			glsl.Init();
			fbo_back=glsl.CreateFBO(screen_w,screen_h);
			fbo_front=glsl.CreateFBO(screen_w,screen_h);
			fbo_render=glsl.CreateFBO(screen_w/2,screen_h/2);
			fbo_blur_h=glsl.CreateFBO(screen_w/glow_fbo_size,screen_h/glow_fbo_size);
			fbo_blur_v=glsl.CreateFBO(screen_w/glow_fbo_size,screen_h/glow_fbo_size);
			fbo_ping=glsl.CreateFBO(screen_w,screen_h);
			fbo_pong=glsl.CreateFBO(screen_w,screen_h);
			// shader simple
			shader_simple=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_SIMPLE,DEBUG);
			shader_simple_texture=shader_simple->GetVariableID("texture");
			// shader blur horizontal
			shader_blur_h=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_BLUR_H,DEBUG);
			shader_blur_h_texture=shader_blur_h->GetVariableID("texture");
			shader_blur_h_screen_w=shader_blur_h->GetVariableID("screen_w");
			shader_blur_h_screen_h=shader_blur_h->GetVariableID("screen_h");
			shader_blur_h_time=shader_blur_h->GetVariableID("time");
			shader_blur_h_value=shader_blur_h->GetVariableID("value");
			// shader blur vertical
			shader_blur_v=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_BLUR_V,DEBUG);
			shader_blur_v_texture=shader_blur_v->GetVariableID("texture");
			shader_blur_v_screen_w=shader_blur_v->GetVariableID("screen_w");
			shader_blur_v_screen_h=shader_blur_v->GetVariableID("screen_h");
			shader_blur_v_time=shader_blur_v->GetVariableID("time");
			shader_blur_v_value=shader_blur_v->GetVariableID("value");
			// shader glow
			shader_glow=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_GLOW,DEBUG);
			shader_glow_texture=shader_glow->GetVariableID("texture");
			shader_glow_texture_prv=shader_glow->GetVariableID("texture_prv");
			shader_glow_screen_w=shader_glow->GetVariableID("screen_w");
			shader_glow_screen_h=shader_glow->GetVariableID("screen_h");
			shader_glow_time=shader_glow->GetVariableID("time");
			shader_glow_step=shader_glow->GetVariableID("step");
			shader_glow_value1=shader_glow->GetVariableID("value1");
			shader_glow_value2=shader_glow->GetVariableID("value2");
			// shader radial
			shader_radial=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_RADIAL,DEBUG);
			shader_radial_texture=shader_radial->GetVariableID("texture");
			shader_radial_screen_w=shader_radial->GetVariableID("screen_w");
			shader_radial_screen_h=shader_radial->GetVariableID("screen_h");
			shader_radial_time=shader_radial->GetVariableID("time");
			shader_radial_value1=shader_radial->GetVariableID("value1");
			shader_radial_value2=shader_radial->GetVariableID("value2");
			shader_radial_color=shader_radial->GetVariableID("color");
			// shader copper
			shader_copper=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_COPPER,DEBUG);
			shader_copper_texture=shader_copper->GetVariableID("texture");
			shader_copper_screen_w=shader_copper->GetVariableID("screen_w");
			shader_copper_screen_h=shader_copper->GetVariableID("screen_h");
			shader_copper_time=shader_copper->GetVariableID("time");
			shader_copper_n=shader_copper->GetVariableID("number");
			shader_copper_alpha=shader_copper->GetVariableID("alpha");
			shader_copper_speed=shader_copper->GetVariableID("speed");
			shader_copper_limit=shader_copper->GetVariableID("limit");
			// shader wave
			shader_wave=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_WAVE,DEBUG);
			shader_wave_texture=shader_wave->GetVariableID("texture");
			shader_wave_screen_w=shader_wave->GetVariableID("screen_w");
			shader_wave_screen_h=shader_wave->GetVariableID("screen_h");
			shader_wave_time=shader_wave->GetVariableID("time");
			shader_wave_value=shader_wave->GetVariableID("value");
			// shader render
			shader_render=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_RENDER,DEBUG);
			shader_render_resolution=shader_render->GetVariableID("resolution");
			shader_render_time=shader_render->GetVariableID("time");
			shader_render_value=shader_render->GetVariableID("value");
			// shader cloud
			shader_cloud=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_CLOUD,DEBUG);
			shader_cloud_resolution=shader_cloud->GetVariableID("resolution");
			shader_cloud_time=shader_cloud->GetVariableID("time");
			shader_cloud_value=shader_cloud->GetVariableID("value");
			// shader tunnel
			shader_tunnel=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_TUNNEL,DEBUG);
			shader_tunnel_resolution=shader_tunnel->GetVariableID("resolution");
			shader_tunnel_time=shader_tunnel->GetVariableID("time");
			shader_tunnel_value=shader_tunnel->GetVariableID("value");
			// shader tube
			shader_tube=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_TUBE,DEBUG);
			shader_tube_resolution=shader_tube->GetVariableID("resolution");
			shader_tube_time=shader_tube->GetVariableID("time");
			shader_tube_value=shader_tube->GetVariableID("value");
			// shader disc
			shader_disc=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_DISC,DEBUG);
			shader_disc_resolution=shader_disc->GetVariableID("resolution");
			shader_disc_time=shader_disc->GetVariableID("time");
			shader_disc_type=shader_disc->GetVariableID("type");
			shader_disc_value=shader_disc->GetVariableID("value");
			shader_disc_sync=shader_disc->GetVariableID("sync");
			// shader title
			shader_title=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_TITLE,DEBUG);
			shader_title_resolution=shader_title->GetVariableID("resolution");
			shader_title_time=shader_title->GetVariableID("time");
			shader_title_value=shader_title->GetVariableID("value");
			// shader tower
			shader_tower=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_TOWER,DEBUG);
			shader_tower_resolution=shader_tower->GetVariableID("resolution");
			shader_tower_time=shader_tower->GetVariableID("time");
			shader_tower_value=shader_tower->GetVariableID("value");
			// shader sphere
			shader_sphere=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_SPHERE,DEBUG);
			shader_sphere_resolution=shader_sphere->GetVariableID("resolution");
			shader_sphere_time=shader_sphere->GetVariableID("time");
			shader_sphere_value=shader_sphere->GetVariableID("value");
			// shader dark
			shader_dark=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_DARK,DEBUG);
			shader_dark_resolution=shader_dark->GetVariableID("resolution");
			shader_dark_time=shader_dark->GetVariableID("time");
			shader_dark_value=shader_dark->GetVariableID("value");
			// shader land
			shader_land=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_LAND,DEBUG);
			shader_land_resolution=shader_land->GetVariableID("resolution");
			shader_land_time=shader_land->GetVariableID("time");
			shader_land_value1=shader_land->GetVariableID("value1");
			shader_land_value2=shader_land->GetVariableID("value2");
			// shader postfx
			shader_postfx=glsl.CreateShader(IDR_VS_SIMPLE,IDR_PS_POSTFX,DEBUG);
			shader_postfx_texture=shader_postfx->GetVariableID("texture");
			shader_postfx_texture_2d=shader_postfx->GetVariableID("texture_2d");
			shader_postfx_screen_w=shader_postfx->GetVariableID("screen_w");
			shader_postfx_screen_h=shader_postfx->GetVariableID("screen_h");
			shader_postfx_time=shader_postfx->GetVariableID("time");
			shader_postfx_flash=shader_postfx->GetVariableID("flash");
			shader_postfx_value=shader_postfx->GetVariableID("value");
			shader_postfx_deform=shader_postfx->GetVariableID("deform");
			shader_postfx_scanline=shader_postfx->GetVariableID("scanline");
			shader_postfx_sync=shader_postfx->GetVariableID("sync");
			}
		catch(const char* _pException)
			{
			char glsl_error[512];
			sprintf(glsl_error,"A bloody error occurred during GLSL init!\n\n%s",_pException);
			MessageBox(NULL,glsl_error,"GLSL ERROR!",MB_OK|MB_ICONERROR);
			return false;
			}
		}
	#endif
	/*-/PATACODE---------------------------------------------------------------*/
	return true;
	}

int DrawGLScene(void) // draw scene
	{
	frame_counter++;
	// timer
	timer->update();
	timer_global_previous=timer_global;
	timer_global=timer->elapsed;
	if(!pause) main_angle=timer_global*100.0f*PID;
	timer_fps=1.0f/(timer_global-timer_global_previous);
	if(timer_fps<0) timer_fps=0;
	if(timer_fps>1000) timer_fps=0;
	timer_fps_total+=timer_fps;
	timer_fps_average=timer_fps_total/frame_counter;
	if(timer_fps<timer_fps_min) timer_fps_min=timer_fps;
	if(timer_fps>timer_fps_max) timer_fps_max=timer_fps;
	// music
	if(music_play)
		{
		#if MUSIC
		timer_music=dsGetCurSmp()/176400.0f-timer_buffer;
		#else
		timer_music=timer_global-timer_buffer;
		#endif
		}
	else
		{
		timer_music=timer_global;
		}
	if(!DEBUG&&timer_music>=screen_timer[screen_i]&&screen_i<screen_max)
		{
		screen_i++;
		if(screen_i>screen_max-1) screen_i=0;
		screen(screen_start[screen_i]);
		}
	// synchro
	music_order=int(floor(timer_music/TIME));
	music_line=int(floor(float(timer_music-music_order*TIME)/float(TIME/64.0)));
	if(music_line!=music_line_buffer)
		{
		music_line_buffer=music_line;
		if(screen_n==1) // intro
			{
			switch(music_order)
				{
				case 3: case 4: case 5: case 6:
					if((music_line%32)==0) bar(1.5f);
					break;
				case 7: case 8: case 9: case 10:
					if((music_line%16)==0) bar(1.375f);
					break;
				case 11: case 12:
					if((music_line%11)==0) bar(1.25f);
					break;
				case 13: case 14:
					if((music_line%8)==0) bar(1.125f);
					break;
				case 15: case 16:
					if((music_line%5)==0) bar(1.0f);
					break;
				case 17: case 18:
					if((music_line%4)==0) bar(0.875f);
					break;
				case 19: case 20:
					if((music_line%3)==0) bar(0.75f);
					break;
				case 21: case 22:
					if((music_line%2)==0) bar(0.625f);
					break;
				case 23: case 24:
					if((music_line%1)==0) bar(0.5f);
					break;
				case 25: case 26:
					if((music_line%1)==0) bar(0.5f);
					if(music_order==26&&music_line==56) screen(11);	// we are back
					break;
				}
			}
		if(screen_n==3)	// cloud
			{
			if((music_line%32)==16) sync2();
			}
		if(screen_n==7||screen_n==12)	// title
			{
			if(music_order!=34&&music_line==56) sync1();
			if(music_order==30&&music_line==58&&music_line==60&&music_line==62) sync1();
			}
		if(screen_n==15&&(music_line==16||music_line==48)) greetings(); // greetings
		// default
		if(screen_n==2||screen_n==3||screen_n==4||screen_n==6||screen_n==7||screen_n==11||screen_n==12||screen_n==15||screen_n==20)
			{
			switch(music_line)
				{
				case 16: case 48:
					beat();
					if(screen_n==2) sync2(); // render
					if(screen_n==4||screen_n==15) sync1(); // greetings
					break;
				}
			}
		}
	if(sync1_flag)
		{
		angle=(main_angle-sync1_angle)*sync1_speed;
		sync1_value=1.0f-sinf(angle);
		if(angle>90.0f*PID)
			{
			sync1_value=0;
			sync1_flag=false;
			}
		}
	if(sync2_flag)
		{
		angle=(main_angle-sync2_angle)*sync2_speed;
		sync2_value=sinf(angle);
		if(angle>90.0f*PID)
			{
			sync2_value=1.0f;
			sync2_flag=false;
			}
		}
	if(beat_flag)
		{
		angle=(main_angle-beat_angle)*beat_speed;
		beat_value=1.0f-sinf(angle);
		if(angle>90.0f*PID)
			{
			beat_value=0;
			beat_flag=false;
			}
		}
	if(flash_flag)
		{
		angle=(main_angle-flash_angle)*flash_speed;
		flash_value=1.0f-sinf(angle);
		if(angle>90.0f*PID)
			{
			flash_value=0;
			flash_flag=false;
			}
		}
	if(fade_flag)
		{
		angle=(main_angle-fade_angle)*fade_speed;
		fade_value=1.0f-sinf(angle);
		if(angle>90.0f*PID)
			{
			fade_value=0;
			fade_flag=false;
			}
		}
	/*-PATACODE----------------------------------------------------------------*/
	#if SHADER
	if(shader_flag)
		{
		fbo_back->Bind();
		fbo_back->Clear(screen_r,screen_g,screen_b,0.0f,1.0f);
		}
	else
		{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		}
	#else
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	#endif
	/*-/PATACODE---------------------------------------------------------------*/
	glEnable(GL_BLEND);
	//if(polygon) glEnable(GL_LINE_SMOOTH); else glDisable(GL_LINE_SMOOTH);
	glLineWidth((polygon)?2.0f:1.0f);
	fx_angle=main_angle-part_angle;
	if(polygon) glEnable(GL_TEXTURE_2D);
	/* .-------------------. */
	/* | DRAW SOME 2D HERE | */
	/* °-------------------° */
	init_viewport(1);
	// draw logo
	glBindTexture(GL_TEXTURE_2D,tex_map);
	if(logo_flag)
		{
		glBlendFunc(GL_SRC_COLOR,GL_SRC_ALPHA);
		glLoadIdentity();
		glColor3f(1.0f,1.0f,1.0f);
		glVertexPointer(2,GL_INT,0,logo_vtx);
		glTexCoordPointer(2,GL_FLOAT,0,logo_tex);
		glTranslated(logo_x,logo_y,0);
		glDrawArrays(GL_QUADS,0,8);
		glTranslated(-logo_w/2,logo_h,0);
		glDrawArrays(GL_QUADS,0,8);
		glTranslated(logo_w,0,0);
		glDrawArrays(GL_QUADS,0,8);
		if(polygon) glEnable(GL_TEXTURE_2D);
		}
	// draw bar
	if(logo_flag)
		{
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor3f(0,0,0);
		glVertexPointer(2,GL_INT,0,bar_vtx);
		for(i=0;i<bar_n;i++)
			{
			angle=(main_angle-bar_angle[i])*bar_speed[i];
			value=1.0f-sinf(angle);
			if(angle>90.0f*PID) value=0;
			glLoadIdentity();
			glTranslated(bar_x+bar_w*i,bar_y,0);//+bar_w/2*value
			glScalef(1.0f-value,1.0f,1.0f);
			glDrawArrays(GL_QUADS,0,4);
			}
		if(polygon) glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		}
	// draw skull
	if(skull_flag)
		{
		glLoadIdentity();
		glBlendFunc(GL_SRC_COLOR,GL_SRC_ALPHA);
		glTranslated(skull_x,skull_y,0);
		glColor3f(skull_r,skull_g,skull_b);
		glVertexPointer(2,GL_INT,0,skull_vtx);
		glTexCoordPointer(2,GL_FLOAT,0,skull_tex);
		glDrawArrays(GL_QUADS,0,4);
		}
	/* .-------------------. */
	/* | DRAW SOME 3D HERE | */
	/* °-------------------° */
	init_viewport(0);
	/*-PATACODE----------------------------------------------------------------*/
	#if SHADER
	if(shader_flag)
		{
		init_shader();
		// shader render
		if(shader_render_flag)
			{
			fbo_back->UnBind();
			fbo_render->Bind();
			shader_render->Use();
			shader_render->SetVar(shader_render_resolution,0.5f,0.5f*float(screen_w/screen_h));
			shader_render->SetVar(shader_render_time,fx_angle);
			shader_render->SetVar(shader_render_value,sync1_value*cosf((main_angle-sync1_angle)*1.0f));
			fs_quad();
			shader_render->UnUse();
			fbo_render->UnBind();
			fbo_back->Bind();
			// shader simple
			draw_shader_simple(fbo_render,0);
			}
		// shader cloud
		if(shader_cloud_flag)
			{
			fbo_back->UnBind();
			fbo_render->Bind();
			shader_cloud->Use();
			shader_cloud->SetVar(shader_cloud_resolution,0.5f,0.5f);
			shader_cloud->SetVar(shader_cloud_time,fx_angle);
			shader_cloud->SetVar(shader_cloud_value,sync1_value*cosf((main_angle-sync1_angle)*1.0f));
			fs_quad();
			shader_cloud->UnUse();
			fbo_render->UnBind();
			fbo_back->Bind();
			// shader simple
			draw_shader_simple(fbo_render,0);
			}
		// shader tunnel
		if(shader_tunnel_flag)
			{
			fbo_back->UnBind();
			fbo_render->Bind();
			shader_tunnel->Use();
			shader_tunnel->SetVar(shader_tunnel_resolution,0.5f,0.5f);
			shader_tunnel->SetVar(shader_tunnel_time,fx_angle);
			shader_tunnel->SetVar(shader_tunnel_value,sync1_value);
			fs_quad();
			shader_tunnel->UnUse();
			fbo_render->UnBind();
			fbo_back->Bind();
			// shader simple
			draw_shader_simple(fbo_render,0);
			}
		// shader tube
		if(shader_tube_flag)
			{
			fbo_back->UnBind();
			fbo_render->Bind();
			shader_tube->Use();
			shader_tube->SetVar(shader_tube_resolution,0.5f,0.5f);
			shader_tube->SetVar(shader_tube_time,fx_angle);
			shader_tube->SetVar(shader_tube_value,sync2_value);
			fs_quad();
			shader_tube->UnUse();
			fbo_render->UnBind();
			fbo_back->Bind();
			// shader simple
			draw_shader_simple(fbo_render,0);
			}
		// shader disc
		if(shader_disc_flag)
			{
			fbo_back->UnBind();
			fbo_render->Bind();
			shader_disc->Use();
			shader_disc->SetVar(shader_disc_resolution,0.5f,0.5f);
			shader_disc->SetVar(shader_disc_time,fx_angle);
			shader_disc->SetVar(shader_disc_type,disc_type);
			shader_disc->SetVar(shader_disc_value,sync1_value);
			shader_disc->SetVar(shader_disc_sync,beat_value);
			fs_quad();
			shader_disc->UnUse();
			fbo_render->UnBind();
			fbo_back->Bind();
			// shader simple
			draw_shader_simple(fbo_render,0);
			}
		// shader title
		if(shader_title_flag)
			{
			fbo_back->UnBind();
			fbo_render->Bind();
			shader_title->Use();
			shader_title->SetVar(shader_title_resolution,0.5f,0.5f);
			shader_title->SetVar(shader_title_time,fx_angle);
			shader_title->SetVar(shader_title_value,-sync1_value*8.0f);
			fs_quad();
			shader_title->UnUse();
			fbo_render->UnBind();
			fbo_back->Bind();
			// shader simple
			draw_shader_simple(fbo_render,0);
			}
		// shader sphere
		if(shader_sphere_flag)
			{
			fbo_back->UnBind();
			fbo_render->Bind();
			shader_sphere->Use();
			shader_sphere->SetVar(shader_sphere_resolution,0.5f,0.5f);
			shader_sphere->SetVar(shader_sphere_time,fx_angle);
			shader_sphere->SetVar(shader_sphere_value,sync1_value);
			fs_quad();
			shader_sphere->UnUse();
			fbo_render->UnBind();
			fbo_back->Bind();
			// shader simple
			draw_shader_simple(fbo_render,0);
			}
		// shader tower
		if(shader_tower_flag)
			{
			fbo_back->UnBind();
			fbo_render->Bind();
			shader_tower->Use();
			shader_tower->SetVar(shader_tower_resolution,0.5f,0.5f);
			shader_tower->SetVar(shader_tower_time,fx_angle);
			shader_tower->SetVar(shader_tower_value,sync2_value);
			fs_quad();
			shader_tower->UnUse();
			fbo_render->UnBind();
			fbo_back->Bind();
			// shader simple
			draw_shader_simple(fbo_render,0);
			}
		// shader dark
		if(shader_dark_flag)
			{
			fbo_back->UnBind();
			fbo_render->Bind();
			shader_dark->Use();
			shader_dark->SetVar(shader_dark_resolution,0.5f,0.5f);
			shader_dark->SetVar(shader_dark_time,fx_angle);
			shader_dark->SetVar(shader_dark_value,sync1_value);
			fs_quad();
			shader_dark->UnUse();
			fbo_render->UnBind();
			fbo_back->Bind();
			// shader simple
			draw_shader_simple(fbo_render,0);
			}
		// shader land
		if(shader_land_flag)
			{
			fbo_back->UnBind();
			fbo_render->Bind();
			shader_land->Use();
			shader_land->SetVar(shader_land_resolution,0.5f,0.5f);
			shader_land->SetVar(shader_land_time,fx_angle);
			shader_land->SetVar(shader_land_value1,sync1_value);
			shader_land->SetVar(shader_land_value2,sync2_value);
			fs_quad();
			shader_land->UnUse();
			fbo_render->UnBind();
			fbo_back->Bind();
			// shader simple
			draw_shader_simple(fbo_render,0);
			}
		// shader radial
		if(shader_effect_flag&&shader_radial_flag)
			{
			fbo_back->UnBind();
			fbo_render->Bind();
			shader_radial->Use();
			shader_radial->SetTexture(shader_radial_texture,fbo_back->GetTextureID(),0);
			shader_radial->SetVar(shader_radial_screen_w,1.0f);
			shader_radial->SetVar(shader_radial_screen_h,1.0f);
			shader_radial->SetVar(shader_radial_time,fx_angle);
			shader_radial->SetVar(shader_radial_value1,radial_value1);
			shader_radial->SetVar(shader_radial_value2,radial_value2);
			shader_radial->SetVar(shader_radial_color,radial_color);
			fs_quad();
			shader_radial->UnUse();
			fbo_render->UnBind();
			fbo_back->Bind();
			// shader simple
			draw_shader_simple(fbo_render,0);
			}
		glEnable(GL_BLEND);
		}
	#endif
	/*-/PATACODE---------------------------------------------------------------*/
	// draw liner
	if(liner_flag)
		{
		switch(screen_n)
			{
			case 10: case 13: // credits
				liner_p_z=-8.0f;
				value=sync1_value*12.0f;
				liner_r=(value>1.0f)?0.875f:0.875f*value;
				liner_g=(value>1.0f)?0.625f:0.625f*value;
				liner_b=(value>1.0f)?0.375f:0.375f*value;
				break;
			case 15: // greetings
				liner_p_z=-8.0f;
				value=(1.0f-sync2_value)*2.0f;
				liner_r=(value>1.0f)?0.875f:0.875f*value;
				liner_g=(value>1.0f)?0.625f:0.625f*value;
				liner_b=(value>1.0f)?0.375f:0.375f*value;
				break;
			case 17: // end
				copper_alpha=fade_value;
				break;
			}
		glBindTexture(GL_TEXTURE_2D,tex_map);
		glVertexPointer(2,GL_FLOAT,0,liner_vtx);
		glColor3f(liner_r,liner_g,liner_b);
		glBlendFunc(GL_SRC_COLOR,GL_SRC_ALPHA);
		j=0;
		liner_line=-1;
		float l_v=0.0390625f;
		float l_j=0.001953125f;
		liner_count=liner_write?(int)((main_angle-liner_angle)*12.0f):liner_length;
		if(liner_count>liner_length) liner_count=liner_length;
		for(i=0;i<liner_count;i++)
			{
			j++;
			car=(unsigned char)txt[i-1];
			if(car>13)
				{
				if(car!=32)
					{
					glLoadIdentity();
					glTranslatef(liner_x,liner_y,liner_z);
					glRotatef(liner_a_x,1.0f,0,0);
					glRotatef(liner_a_y,0,1.0f,0);
					glRotatef(liner_a_z,0,0,1.0f);
					glTranslatef(liner_p_x-x+j*liner_w,liner_p_y+y,liner_p_z);
					float l_w=(car%16)*0.03515625f;
					float l_h=(car-car%16)*0.00244140625f;
					float liner_tex[]={l_w+l_v-l_j,1.0f-l_h-l_v,l_w+l_v-l_j,1.0f-l_h,l_w-l_j,1.0f-l_h,l_w-l_j,1.0f-l_h-l_v};//,l_w-l_j,1.0f-l_h-l_v,l_w-l_j,1.0f-l_h,l_w+l_v-l_j,1.0f-l_h,l_w+l_v-l_j,1.0f-l_h-l_v};
					glTexCoordPointer(2,GL_FLOAT,0,liner_tex);
					glDrawArrays(GL_QUADS,0,4);
					}
				}
			else
				{
				j=0;
				x=(liner_max+1)*0.5f*liner_w;
				y=((liner_line_n-1)*0.5f-liner_line)*liner_h;
				liner_line++;
				}
			}
		}
	/*-PATACODE----------------------------------------------------------------*/
	#if SHADER
	if(shader_flag)
		{
		init_shader();
		fbo_back->UnBind();
		fbo_ping->Bind();
		// shader copper
		if(shader_copper_flag)
			{
			shader_copper->Use();
			shader_copper->SetTexture(shader_copper_texture,fbo_back->GetTextureID(),0);
			shader_copper->SetVar(shader_copper_screen_w,screen_w);
			shader_copper->SetVar(shader_copper_screen_h,screen_h);
			shader_copper->SetVar(shader_copper_time,fx_angle);
			shader_copper->SetVar(shader_copper_n,copper_n);
			shader_copper->SetVar(shader_copper_alpha,copper_alpha);
			shader_copper->SetVar(shader_copper_speed,copper_speed);
			shader_copper->SetVar(shader_copper_limit,copper_limit);
			fs_quad();
			shader_copper->UnUse();
			}
		else
			{
			// shader simple
			draw_shader_simple(fbo_back,0);
			}
		fbo_ping->UnBind();
		fbo_back->Bind();
		// shader wave
		if(shader_wave_flag)
			{
			shader_wave->Use();
			shader_wave->SetTexture(shader_wave_texture,fbo_ping->GetTextureID(),0);
			shader_wave->SetVar(shader_wave_screen_w,screen_w);
			shader_wave->SetVar(shader_wave_screen_h,screen_h);
			shader_wave->SetVar(shader_wave_time,fx_angle);
			shader_wave->SetVar(shader_wave_value,0.3625f);
			fs_quad();
			shader_wave->UnUse();
			}
		else
			{
			// shader simple
			draw_shader_simple(fbo_ping,0);
			}
		}
	#endif
	/*-/PATACODE---------------------------------------------------------------*/
	/* .-------------------. */
	/* | DRAW SOME 2D HERE | */
	/* °-------------------° */
	init_viewport(1);
	glBindTexture(GL_TEXTURE_2D,tex_map);
	glEnable(GL_BLEND);
	// draw razor
	if(razor_flag)
		{
		if(screen_n==17) razor_alpha=1.0f-sync2_value;
		glLoadIdentity();
		glBlendFunc(GL_SRC_COLOR,GL_SRC_ALPHA);
		glTranslated(razor_x,razor_y,0);
		glColor3f(0.75f*razor_alpha,0.625f*razor_alpha,0.375f*razor_alpha);
		glVertexPointer(2,GL_INT,0,razor_vtx);
		glTexCoordPointer(2,GL_FLOAT,0,razor_tex);
		glDrawArrays(GL_QUADS,0,4);
		}
	// draw triforce
	if(triforce_flag)
		{
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_ONE,GL_SRC_ALPHA);
		glVertexPointer(2,GL_FLOAT,0,triforce_vtx);
		glLoadIdentity();
		glColor3f(0.125f*razor_alpha,0.0625f*razor_alpha,0);
		glTranslated(triforce_x,triforce_y,0);
		//glRotatef(flash_value*360.0f,0,0,1.0f);
		glDrawArrays(GL_TRIANGLES,0,18);
		// wireframe
		glPolygonMode(GL_FRONT,GL_LINE);
		glDrawArrays(GL_TRIANGLES,0,18);
		glPolygonMode(GL_FRONT,polygon_fillmode);
		if(polygon) glEnable(GL_TEXTURE_2D);
		}
	// draw decrunch
	if(decrunch_flag)
		{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		for(i=0;i<screen_h/decrunch_h+2;i++)
			{
			decrunch_y=i*decrunch_h;
			decrunch_split=rand()%8;
			if(decrunch_split==0)
				{
				decrunch_split_w=rand()%(screen_w/4)*4;
				if(rand()%4==0) glColor3f((float)(CR*(decrunch_color+rand()%decrunch_color)),(float)(CR*(decrunch_color+rand()%decrunch_color)),(float)(CR*(decrunch_color+rand()%decrunch_color)));
				rectangle(0,decrunch_y-decrunch_h,decrunch_split_w,decrunch_h);
				if(rand()%128==0) glColor3f((float)(CR*(decrunch_color+rand()%decrunch_color)),(float)(CR*(decrunch_color+rand()%decrunch_color)),(float)(CR*(decrunch_color+rand()%decrunch_color)));
				rectangle(decrunch_split_w,decrunch_y-decrunch_h,screen_w-decrunch_split_w,decrunch_h);
				}
			else
				{
				rectangle(0,decrunch_y-decrunch_h,screen_w,decrunch_h);
				}
			}
		glEnable(GL_BLEND);
		if(polygon) glEnable(GL_TEXTURE_2D);
		}
	// draw dos
	if(dos_flag)
		{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glColor3f(1.0f,1.0f,1.0f);
		rectangle(dos_m,dos_m,screen_w-(int)(26.5f*ratio_2d)-dos_m*2,10*ratio_2d);
		rectangle(screen_w-(int)(25.5f*ratio_2d)-dos_m,dos_m,11*ratio_2d,10*ratio_2d);
		rectangle(screen_w-(int)(13.5f*ratio_2d)-dos_m,dos_m,11*ratio_2d,10*ratio_2d);
		rectangle(screen_w-(int)(1.5f*ratio_2d)-dos_m,dos_m,(int)(1.5f*ratio_2d),10*ratio_2d);
		rectangle(dos_m,dos_m+10*ratio_2d,1*ratio_2d,screen_h-10*ratio_2d-dos_m*2);
		rectangle(screen_w-1*ratio_2d-dos_m,dos_m+10*ratio_2d,1*ratio_2d,screen_h-10*ratio_2d-dos_m*2);
		rectangle(dos_m+1*ratio_2d,screen_h-1*ratio_2d-dos_m,screen_w-2*ratio_2d-dos_m*2,1*ratio_2d);
		glColor3f(dos_r,dos_g,dos_b);
		rectangle(dos_m+1*ratio_2d,dos_m+1*ratio_2d,32*ratio_2d,8*ratio_2d);
		rectangle(dos_m+(int)(34.5f*ratio_2d),dos_m+2*ratio_2d,screen_w-(int)(62.5f*ratio_2d)-dos_m*2,2*ratio_2d);
		rectangle(dos_m+(int)(34.5f*ratio_2d),dos_m+6*ratio_2d,screen_w-(int)(62.5f*ratio_2d)-dos_m*2,2*ratio_2d);
		rectangle(screen_w-(int)(24.5f*ratio_2d)-dos_m,dos_m+1*ratio_2d,7*ratio_2d,6*ratio_2d);
		glColor3f(1.0f,1.0f,1.0f);
		rectangle(screen_w-(int)(23.5f*ratio_2d)-dos_m,dos_m+2*ratio_2d,5*ratio_2d,4*ratio_2d);
		glColor3f(0,0,0);
		rectangle(screen_w-(int)(22.5f*ratio_2d)-dos_m,dos_m+3*ratio_2d,7*ratio_2d,6*ratio_2d);
		rectangle(screen_w-(int)(12.5f*ratio_2d)-dos_m,dos_m+1*ratio_2d,7*ratio_2d,6*ratio_2d);
		glColor3f(dos_r,dos_g,dos_b);
		rectangle(screen_w-(int)(10.5f*ratio_2d)-dos_m,dos_m+2*ratio_2d,7*ratio_2d,7*ratio_2d);
		glColor3f(1.0f,1.0f,1.0f);
		rectangle(screen_w-(int)(9.5f*ratio_2d)-dos_m,dos_m+3*ratio_2d,5*ratio_2d,5*ratio_2d);
		rectangle(screen_w-8*ratio_2d-dos_m,screen_h-9*ratio_2d-dos_m,7*ratio_2d,8*ratio_2d);
		glColor3f(dos_r,dos_g,dos_b);
		rectangle(screen_w-7*ratio_2d-dos_m,screen_h-8*ratio_2d-dos_m,6*ratio_2d,7*ratio_2d);
		glColor3f(1.0f,1.0f,1.0f);
		rectangle(screen_w-4*ratio_2d-dos_m,screen_h-8*ratio_2d-dos_m,4*ratio_2d,2*ratio_2d);
		rectangle(screen_w-7*ratio_2d-dos_m,screen_h-5*ratio_2d-dos_m,2*ratio_2d,5*ratio_2d);
		rectangle(screen_w-6*ratio_2d-dos_m,screen_h-7*ratio_2d-dos_m,1*ratio_2d,1*ratio_2d);
		rectangle(screen_w-4*ratio_2d-dos_m,screen_h-5*ratio_2d-dos_m,2*ratio_2d,3*ratio_2d);
		glEnable(GL_BLEND);
		if(polygon) glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_ONE,GL_ONE);
		glVertexPointer(2,GL_INT,0,shell_vtx);
		glTexCoordPointer(2,GL_FLOAT,0,shell_tex);
		glDrawArrays(GL_QUADS,0,4);
		// draw dos text
		glVertexPointer(2,GL_INT,0,dos_vtx);
		j=0;
		liner_line=-1;
		for(i=1;i<liner_length+1;i++)
			{
			j++;
			car=(unsigned char)txt[i-1];
			if(i==liner_length) car=(cursor_flag&&music_line<32)?car_cursor:0;
			if(car>32)
				{
				glLoadIdentity();
				glTranslated(x+j*(dos_w+2*ratio_2d),y,0);
				float l_w=(car%16)*0.03125f;
				float l_h=(car-car%16)*0.001953125f;
				float dos_tex[]={l_w+0.03125f,0.5f-l_h-0.03125f,l_w+0.03125f,0.5f-l_h,l_w,0.5f-l_h,l_w,0.5f-l_h-0.03125f};
				glTexCoordPointer(2,GL_FLOAT,0,dos_tex);
				if(car==car_cursor) glColor3f(CR*(dos_r+64),CR*(dos_g+48),CR*(dos_b+48));
				glDrawArrays(GL_QUADS,0,4);
				if(car==car_cursor) glColor3f(1.0f,1.0f,1.0f);
				}
			if(car==13)
				{
				j=0;
				liner_line++;
				x=dos_m+(float)(2*ratio_2d)+dos_tab;
				y=dos_m+(float)(15*ratio_2d+liner_line*(dos_h+4*ratio_2d));
				if(y+dos_h>screen_h-dos_m) break;
				}
			}
		}
	// draw debug
	//#if DEBUG
	if(debug_flag)
		{
		char debug[512];
		sprintf(debug,"screen=%02d:%02d\rwidth=%d\rheight=%d\rratio=%dx\rfps=%3.1f\raverage=%3.1f\rfps min=%3.1f\rfps max=%3.1f\rtimer=%3.1f\rgap=%1.3f\rorder=%002d-%02d\rbeat=%-02.1f\rsync1=%-02.1f\rsync2=%-02.1f\r",screen_i,screen_n,screen_w,screen_h,ratio_2d,timer_fps,timer_fps_average,timer_fps_min,timer_fps_max,timer_music,timer_global-timer_music,music_order,music_line,beat_value,sync1_value,sync2_value);
		float l_v=0.0390625f;
		float l_j=0.001953125f;
		glVertexPointer(2,GL_INT,0,debug_vtx);
		glLoadIdentity();
		glBlendFunc(GL_ONE,GL_ONE);
		glColor3f(0.5f,0.5f,0.5f);
		glTranslated(debug_x,debug_y,0);
		j=0;
		for(i=0;i<(int)strlen(debug);i++)
			{
			j++;
			car=(unsigned char)debug[i];
			if(car==13)
				{
				glTranslated(-j*(debug_w-ratio_2d)*2,(debug_h-ratio_2d)*2,0);
				j=0;
				}
			glTranslated((debug_w-ratio_2d)*2,0,0);
			if(car>32)
				{
				float l_w=(car%16)*0.03515625f;
				float l_h=(car-car%16)*0.00244140625f;
				float debug_tex[]={l_w+l_v-l_j,1.0f-l_h-l_v,l_w+l_v-l_j,1.0f-l_h,l_w-l_j,1.0f-l_h,l_w-l_j,1.0f-l_h-l_v};
				glTexCoordPointer(2,GL_FLOAT,0,debug_tex);
				glDrawArrays(GL_QUADS,0,4);
				}
			}
		}
	//#endif
	// draw corner
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glVertexPointer(2,GL_INT,0,corner_vtx);
	glLoadIdentity();
	glColor3f(0,0,0);
	glDrawArrays(GL_TRIANGLES,0,corner_n*3);
	glTranslated(screen_w,0,0);
	glRotated(90.0f,0,0,1.0f);
	glDrawArrays(GL_TRIANGLES,0,corner_n*3);
	glTranslated(screen_h,0,0);
	glRotated(90.0f,0,0,1.0f);
	glDrawArrays(GL_TRIANGLES,0,corner_n*3);
	glTranslated(screen_w,0,0);
	glRotated(90.0f,0,0,1.0f);
	glDrawArrays(GL_TRIANGLES,0,corner_n*3);
	glEnable(GL_BLEND);
	/* .-------------------. */
	/* | DRAW SOME 3D HERE | */
	/* °-------------------° */
	//init_viewport(0);
	/*-PATACODE----------------------------------------------------------------*/
	#if SHADER
	if(shader_flag)
		{
		init_shader();
		fbo_back->UnBind();
		if(shader_effect_flag&&shader_blur_flag)
			{
			// shader blur horizontal
			fbo_ping->Bind();
			shader_blur_h->Use();
			shader_blur_h->SetTexture(shader_blur_h_texture,fbo_back->GetTextureID(),0);
			shader_blur_h->SetVar(shader_blur_h_screen_w,1.0f);
			shader_blur_h->SetVar(shader_blur_h_screen_h,1.0f);
			shader_blur_h->SetVar(shader_blur_h_time,fx_angle);
			shader_blur_h->SetVar(shader_blur_h_value,0.625f/screen_w);
			fs_quad();
			shader_blur_h->UnUse();
			fbo_ping->UnBind();
			// shader blur vertical
			fbo_front->Bind();
			shader_blur_v->Use();
			shader_blur_v->SetTexture(shader_blur_v_texture,fbo_ping->GetTextureID(),0);
			shader_blur_v->SetVar(shader_blur_v_screen_w,1.0f);
			shader_blur_v->SetVar(shader_blur_v_screen_h,1.0f);
			shader_blur_v->SetVar(shader_blur_v_time,fx_angle);
			shader_blur_v->SetVar(shader_blur_v_value,0.625f/screen_h);
			fs_quad();
			shader_blur_v->UnUse();
			}
		else
			{
			// shader simple
			fbo_front->Bind();
			draw_shader_simple(fbo_back,0);
			}
		fbo_front->UnBind();
		// shader glow
		if(shader_effect_flag&&shader_glow_flag)
			{
			// shader blur horizontal
			fbo_blur_h->Bind();
			shader_blur_h->Use();
			shader_blur_h->SetTexture(shader_blur_h_texture,fbo_ping->GetTextureID(),0);
			shader_blur_h->SetVar(shader_blur_h_screen_w,1.0f/glow_fbo_size);
			shader_blur_h->SetVar(shader_blur_h_screen_h,1.0f/glow_fbo_size);
			shader_blur_h->SetVar(shader_blur_h_time,fx_angle);
			shader_blur_h->SetVar(shader_blur_h_value,4.0f/screen_w);
			fs_quad();
			shader_blur_h->UnUse();
			fbo_blur_h->UnBind();
			// shader blur vertical
			fbo_blur_v->Bind();
			shader_blur_v->Use();
			shader_blur_v->SetTexture(shader_blur_v_texture,fbo_blur_h->GetTextureID(),0);
			shader_blur_v->SetVar(shader_blur_v_screen_w,1.0f/glow_fbo_size);
			shader_blur_v->SetVar(shader_blur_v_screen_h,1.0f/glow_fbo_size);
			shader_blur_v->SetVar(shader_blur_v_time,fx_angle);
			shader_blur_v->SetVar(shader_blur_h_value,3.0f/screen_h);
			fs_quad();
			shader_blur_v->UnUse();
			fbo_blur_v->UnBind();
			// shader blur horizontal
			fbo_blur_h->Bind();
			shader_blur_h->Use();
			shader_blur_h->SetTexture(shader_blur_h_texture,fbo_blur_v->GetTextureID(),0);
			shader_blur_h->SetVar(shader_blur_h_screen_w,1.0f/glow_fbo_size);
			shader_blur_h->SetVar(shader_blur_h_screen_h,1.0f/glow_fbo_size);
			shader_blur_h->SetVar(shader_blur_h_time,fx_angle);
			shader_blur_h->SetVar(shader_blur_h_value,2.0f/screen_w);
			fs_quad();
			shader_blur_h->UnUse();
			fbo_blur_h->UnBind();
			// shader blur vertical
			fbo_blur_v->Bind();
			shader_blur_v->Use();
			shader_blur_v->SetTexture(shader_blur_v_texture,fbo_blur_h->GetTextureID(),0);
			shader_blur_v->SetVar(shader_blur_v_screen_w,1.0f/glow_fbo_size);
			shader_blur_v->SetVar(shader_blur_v_screen_h,1.0f/glow_fbo_size);
			shader_blur_v->SetVar(shader_blur_v_time,fx_angle);
			shader_blur_v->SetVar(shader_blur_h_value,1.5f/screen_h);
			fs_quad();
			shader_blur_v->UnUse();
			fbo_blur_v->UnBind();
			// shader glow
			fbo_pong->Bind();
			shader_glow->Use();
			shader_glow->SetTexture(shader_glow_texture,fbo_blur_v->GetTextureID(),0);
			shader_glow->SetTexture(shader_glow_texture_prv,fbo_front->GetTextureID(),1);
			shader_glow->SetVar(shader_glow_screen_w,1.0f);
			shader_glow->SetVar(shader_glow_screen_h,1.0f);
			shader_glow->SetVar(shader_glow_time,fx_angle);
			shader_glow->SetVar(shader_glow_step,glow_smoothstep);
			shader_glow->SetVar(shader_glow_value1,glow_mix_ratio1);
			shader_glow->SetVar(shader_glow_value2,glow_mix_ratio2);
			fs_quad();
			shader_glow->UnUse();
			}
		if(!shader_effect_flag)
			{
			// shader simple
			fbo_pong->Bind();
			draw_shader_simple(fbo_front,0);
			}
		fbo_pong->UnBind();
		if(shader_postfx_flag)
			{
			// shader postfx
			shader_postfx->Use();
			shader_postfx->SetTexture(shader_postfx_texture,fbo_pong->GetTextureID(),0);
			shader_postfx->SetVar(shader_postfx_screen_w,screen_w);
			shader_postfx->SetVar(shader_postfx_screen_h,screen_h);
			shader_postfx->SetVar(shader_postfx_time,fx_angle);
			shader_postfx->SetVar(shader_postfx_flash,flash_value);
			shader_postfx->SetVar(shader_postfx_value,4.0f/screen_w);
			shader_postfx->SetVar(shader_postfx_deform,postfx_deform);
			shader_postfx->SetVar(shader_postfx_scanline,postfx_scanline);
			shader_postfx->SetVar(shader_postfx_sync,(float)fabs(beat_value*cosf((main_angle-beat_angle)*8.0f)));
			fs_quad();
			shader_postfx->UnUse();
			}
		else
			{
			// shader simple
			draw_shader_simple(fbo_pong,0);
			}
		}
	#endif
	/*-/PATACODE---------------------------------------------------------------*/
	return true;
	}

void KillGLWindow(void)							// kill window
	{
	if(fullscreen)
		{
		window_w=base_w;
		window_h=base_h;
		ChangeDisplaySettings(NULL,0);	// switch back to desktop
		ShowCursor(true);								// show mouse pointer
		}
	if(hRC)
		{
		if(!wglMakeCurrent(NULL,NULL)) MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK|MB_ICONINFORMATION);
		if(!wglDeleteContext(hRC)) MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK|MB_ICONINFORMATION);
		hRC=NULL;
		}
	if(hDC&&!ReleaseDC(hWnd,hDC)) { MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK|MB_ICONINFORMATION); hDC=NULL; }
	if(hWnd&&!DestroyWindow(hWnd)) { MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK|MB_ICONINFORMATION); hWnd=NULL; }
	if(!UnregisterClass("razor1911",hInstance)) { MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK|MB_ICONINFORMATION); hInstance=NULL; }
	delete timer;
	}

int CreateGLWindow(char* title)
	{
	GLuint pixelFormat;	// pixel format result
	WNDCLASS wc;				// windows class structure
	DWORD dwExStyle;		// window extended style
	DWORD dwStyle;			// window style
	RECT WindowRect;		// upper_left/lower_right values
	int w=GetSystemMetrics(SM_CXSCREEN);
	int h=GetSystemMetrics(SM_CYSCREEN);
	screen_w=fullscreen?w:window_w;
	screen_h=fullscreen?h:window_h;
	timer_fps_min=32768;
	timer_fps_max=0;
	ratio_2d=(int)(screen_w/400); if(ratio_2d<2) ratio_2d=2;
	decrunch_h=(int)(screen_h*0.01f);
	razor_w=106*ratio_2d;
	razor_h=35*ratio_2d;
	logo_w=52*ratio_2d;
	logo_h=46*ratio_2d;
	logo_x=(screen_w-logo_w)/2;
	logo_y=(screen_h-logo_h*2)/2-18*ratio_2d;
	logo_vtx[ 0]=logo_w/2;
	logo_vtx[ 1]=logo_h;
	logo_vtx[ 2]=logo_w;
	logo_vtx[ 3]=logo_h;
	logo_vtx[ 4]=logo_w;
	logo_vtx[ 5]=0;
	logo_vtx[ 6]=logo_w/2;
	logo_vtx[ 7]=0;
	logo_vtx[ 8]=0;
	logo_vtx[ 9]=logo_h;
	logo_vtx[10]=logo_w/2;
	logo_vtx[11]=logo_h;
	logo_vtx[12]=logo_w/2;
	logo_vtx[13]=0;
	logo_vtx[14]=0;
	logo_vtx[15]=0;
	razor_x=(screen_w-razor_w)/2;
	razor_vtx[0]=razor_w;
	razor_vtx[1]=0;
	razor_vtx[2]=0;
	razor_vtx[3]=0;
	razor_vtx[4]=0;
	razor_vtx[5]=razor_h;
	razor_vtx[6]=razor_w;
	razor_vtx[7]=razor_h;
	bar_w=logo_w*2/bar_n;
	bar_h=logo_h*2+2*ratio_2d;
	bar_x=(screen_w-logo_w*2)/2;
	bar_y=logo_y;
	bar_vtx[0]=0;
	bar_vtx[1]=bar_h;
	bar_vtx[2]=bar_w;
	bar_vtx[3]=bar_h;
	bar_vtx[4]=bar_w;
	bar_vtx[5]=0;
	bar_vtx[6]=0;
	bar_vtx[7]=0;
	skull_w=24*ratio_2d;
	skull_h=24*ratio_2d;
	skull_x=(screen_w-skull_w)/2;
	skull_y=(screen_h-skull_h)/2-12*ratio_2d;
	skull_vtx[0]=skull_w;
	skull_vtx[1]=0;
	skull_vtx[2]=0;
	skull_vtx[3]=0;
	skull_vtx[4]=0;
	skull_vtx[5]=skull_h;
	skull_vtx[6]=skull_w;
	skull_vtx[7]=skull_h;
	corner_w=24*ratio_2d;
	corner();
	triforce(52*ratio_2d);
	dos_w=2*ratio_2d;
	dos_h=4*ratio_2d;
	dos_m=20*ratio_2d;
	dos_vtx[0]=dos_w;
	dos_vtx[1]=dos_h;
	dos_vtx[2]=dos_w;
	dos_vtx[3]=-dos_h;
	dos_vtx[4]=-dos_w;
	dos_vtx[5]=-dos_h;
	dos_vtx[6]=-dos_w;
	dos_vtx[7]=dos_h;
	shell_vtx[0]=dos_m+33*ratio_2d;
	shell_vtx[1]=dos_m+9*ratio_2d;
	shell_vtx[2]=dos_m+33*ratio_2d;
	shell_vtx[3]=dos_m+1*ratio_2d;
	shell_vtx[4]=dos_m+1*ratio_2d;
	shell_vtx[5]=dos_m+1*ratio_2d;
	shell_vtx[6]=dos_m+1*ratio_2d;
	shell_vtx[7]=dos_m+9*ratio_2d;
	debug_w=5*ratio_2d;
	debug_h=5*ratio_2d;
	debug_x=12*ratio_2d;
	debug_y=20*ratio_2d;
	debug_vtx[0]=debug_w;
	debug_vtx[1]=debug_h;
	debug_vtx[2]=debug_w;
	debug_vtx[3]=-debug_h;
	debug_vtx[4]=-debug_w;
	debug_vtx[5]=-debug_h;
	debug_vtx[6]=-debug_w;
	debug_vtx[7]=debug_h;
	WindowRect.left=0;												// set left value
	WindowRect.right=(long)screen_w;					// set right value
	WindowRect.top=0;													// set top value
	WindowRect.bottom=(long)screen_h;					// set bottom value
	pfd.cColorBits=window_color;							// set color depth
	hInstance=GetModuleHandle(NULL);					// window instance
	wc.style=CS_HREDRAW|CS_VREDRAW|CS_OWNDC;	// redraw on size,own DC for window
	wc.lpfnWndProc=(WNDPROC) WndProc;					// WndProc handles messages
	wc.cbClsExtra=0;													// no extra window data
	wc.cbWndExtra=0;													// no extra window data
	wc.hInstance=hInstance;										// set the instance
	wc.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(ICON_32));	// load default icon
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);		// load arrow pointer
	wc.hbrBackground=CreateSolidBrush(RGB(0,0,0));// background color
	wc.lpszMenuName=NULL;											// no menu
	wc.lpszClassName="razor1911";							// set class name
	if(!RegisterClass(&wc)) return false;			// register window class
	if(fullscreen)
		{
		DEVMODE dmScreenSettings;															// device mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// is memory cleared?
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);			// devmode structure size
		dmScreenSettings.dmPelsWidth=screen_w;								// screen width
		dmScreenSettings.dmPelsHeight=screen_h;								// screen height
		dmScreenSettings.dmBitsPerPel=window_color;						// bits per pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		// set selected mode
		if(ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL) fullscreen=false;
		}
	if(fullscreen)
		{
		dwExStyle=WS_EX_APPWINDOW;							// window extended style
		dwStyle=WS_POPUP;												// windows style
		ShowCursor(false);											// hide cursor
		}
	else
		{
		dwExStyle=WS_EX_APPWINDOW|WS_EX_CLIENTEDGE;	// window extended style
		dwStyle=WS_OVERLAPPEDWINDOW;								// windows style
		}
	AdjustWindowRectEx(&WindowRect,dwStyle,false,dwExStyle);	// adjust window to requested size
	// create window
	if(!(hWnd=CreateWindowEx(dwExStyle,		// extended style for window
		"razor1911",												// class name
		title,															// window title
		dwStyle															// required window style
		&~WS_THICKFRAME 										// window style (no-resize)
		//&~WS_SYSMENU											// window style (system menu)
		//&WS_DISABLED											// disable window
		&~WS_MAXIMIZEBOX 										// window style (maximize)
		&~WS_MINIMIZEBOX,										// window style (minimize)
		(int)((w-screen_w)/2),							// window position x
		(int)((h-screen_h)/2),							// window position y
		(WindowRect.right-WindowRect.left),	// window width
		(WindowRect.bottom-WindowRect.top),	// window height
		NULL,																// no parent window
		NULL,																// no menu
		hInstance,													// instance
		NULL)))															// don't pass anything to WM_CREATE!
		{
		KillGLWindow();
		return false;
		}
	if(!(hDC=GetDC(hWnd))) { KillGLWindow(); return false; }
	if(!(pixelFormat=ChoosePixelFormat(hDC,&pfd))) { KillGLWindow(); return false; }
	if(!SetPixelFormat(hDC,pixelFormat,&pfd)) { KillGLWindow(); return false; }
	if(!(hRC=wglCreateContext(hDC))) { KillGLWindow(); return false; }
	if(!wglMakeCurrent(hDC,hRC)) { KillGLWindow(); return false; }
	ShowWindow(hWnd,SW_SHOW);		// show window
	SetForegroundWindow(hWnd);	// set higher priority
	SetFocus(hWnd);							// set keyboard focus to window
	if(!InitGL()) { KillGLWindow(); return false; }
	return true;
	}

// window handle,window message,additional message,additional message
LRESULT CALLBACK WndProc(HWND	hWnd,UINT	uMsg,WPARAM	wParam,LPARAM	lParam)
	{
	switch(uMsg)								// check windows messages
		{
		case WM_ACTIVATE:					// watch for window activate message
			{
			active=!HIWORD(wParam)?true:false;	// check minimization state
			return 0;								// return to the message loop
			}
		case WM_SYSCOMMAND:				// intercept system commands
			{
			switch(wParam)					// check system calls
				{
				case SC_SCREENSAVE:		// screensaver trying to start?
				case SC_MONITORPOWER:	// monitor trying to enter powersave?
					return 0;						// prevent from happening
				}
			break;									// exit
			}
		case WM_CLOSE:						// close message?
			{
			PostQuitMessage(0);			// post quit message
			return 0;
			}
		case WM_KEYDOWN:					// key down?
			{
			keys[wParam]=true;			// mark key as true
			return 0;
			}
		case WM_KEYUP:						// key released?
			{
			keys[wParam]=false;			// mark key as false
			return 0;
			}
	  case WM_SIZE:							// resize openGL window
			{
			window_w=LOWORD(lParam);
			window_h=HIWORD(lParam);
			screen_w=window_w;
			screen_h=window_h;
			return 0;
			}
		}
	return DefWindowProc(hWnd,uMsg,wParam,lParam); // pass all unhandled messages to DefWindowProc
	}

// instance,previous instance,command line parameters,window show state
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
	{
	MSG msg;																		// windows message structure
	done=false;																	// exit loop
	// ask for fullscreen mode
	#if !DEBUG
	fullscreen=(MessageBox(NULL,"fullscreen mode?",name,MB_YESNO|MB_ICONQUESTION)==IDYES)?true:false;
	#endif
	// create openGL window
	if(!CreateGLWindow(name)) return 0;					// quit if window not created
	// display a first frame
	DrawGLScene();
	SwapBuffers(hDC);
	// main loop
	while(!done)
		{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// a message is waiting?
			{
			if(msg.message==WM_QUIT)								// a quit message?
				{
				done=true;														// quit window
				}
			else																		// a window message?
				{
				TranslateMessage(&msg);								// translate message
				DispatchMessage(&msg);								// dispatch message
				}
			}
		else
			{
			if((active&&!DrawGLScene())||keys[VK_ESCAPE]) done=true; else SwapBuffers(hDC);	// exit or swap buffers
			if(keys[VK_F1])
				{
				debug_flag=!debug_flag;
				keys[VK_F1]=false;
				}
			#if DEBUG
			if(keys[VK_F2])
				{
				polygon=!polygon;
				polygon_fillmode=(polygon)?GL_FILL:GL_LINE;
				glPolygonMode(GL_FRONT,polygon_fillmode);
				keys[VK_F2]=false;
				}
			if(keys[VK_F3])
				{
				shader_flag=!shader_flag;
				keys[VK_F3]=false;
				}
			if(keys[VK_F4])
				{
				shader_postfx_flag=!shader_postfx_flag;
				keys[VK_F4]=false;
				}
			if(keys[VK_TAB])
				{
				shader_effect_flag=!shader_effect_flag;
				keys[VK_TAB]=false;
				}
			if(keys[VK_RETURN])
				{
				timer_fps_total=0;
				timer_fps_min=32768;
				timer_fps_max=0;
				frame_counter=0;
				bar(1.0f);
				greetings();
				keys[VK_RETURN]=false;
				}
			if(keys[VK_SPACE])
				{
				KillGLWindow();
				fullscreen=!fullscreen;
				if(!CreateGLWindow(name)) return 0;		// quit if window not created
				keys[VK_SPACE]=false;
				}
			if(keys[VK_F5])
				{
				screen_i--;
				if(screen_i<0) screen_i=screen_max-1;
				screen(screen_start[screen_i]);
				keys[VK_F5]=false;
				}
			if(keys[VK_F6])
				{
				screen_i++;
				if(screen_i>screen_max-1) screen_i=0;
				screen(screen_start[screen_i]);
				keys[VK_F6]=false;
				}
			if(keys[VK_F7])
				{
				beat();
				sync1();
				sync2();
				keys[VK_F7]=false;
				}
			if(keys[VK_F12])
				{
				postfx_scanline=!postfx_scanline;
				keys[VK_F12]=false;
				}
			if(keys[VK_BACK])
				{
				pause=!pause;
				keys[VK_BACK]=false;
				}
			#endif
			}
		}
	// shutdown
	#if MUSIC
	dsClose();
	player.Close();
	#endif
	KillGLWindow();
	return(msg.wParam);
	}