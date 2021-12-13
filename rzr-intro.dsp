# Microsoft Developer Studio Project File - Name="intro" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=intro - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rzr-intro.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rzr-intro.mak" CFG="intro - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "intro - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "intro - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "intro - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /GX /Ox /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FD /QIfist /Zm1000 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib glu32.lib winmm.lib user32.lib gdi32.lib libv2.lib /nologo /base:"0x6000000" /subsystem:windows /pdb:none /machine:I386 /nodefaultlib:"msvcrtd.lib" /nodefaultlib:"LIBCMT"

!ELSEIF  "$(CFG)" == "intro - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /ML /W3 /Gm /Gi /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib winmm.lib user32.lib gdi32.lib kernel32.lib libv2.lib /nologo /base:"0x420000" /subsystem:windows /debug /machine:I386 /nodefaultlib:"msvcrtd.lib" /nodefaultlib:"LIBCMT"
# SUBTRACT LINK32 /pdb:none /incremental:no /nodefaultlib /force

!ENDIF 

# Begin Target

# Name "intro - Win32 Release"
# Name "intro - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\data.rc
# End Source File
# Begin Source File

SOURCE=.\glsl.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\timer.cpp
# End Source File
# Begin Source File

SOURCE=.\v2mplayer.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\glext.h
# End Source File
# Begin Source File

SOURCE=.\glsl.h
# End Source File
# Begin Source File

SOURCE=.\libv2.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\timer.h
# End Source File
# Begin Source File

SOURCE=.\v2mplayer.h
# End Source File
# Begin Source File

SOURCE=.\wglext.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\resource\icon.ico
# End Source File
# Begin Source File

SOURCE=.\resource\map.bmp
# End Source File
# Begin Source File

SOURCE=.\music.h
# End Source File
# Begin Source File

SOURCE=.\resource\ps_blur_h.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_blur_v.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_cloud.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_copper.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_dark.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_disc.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_glow.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_land.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_radial.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_render.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_scanline.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_simple.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_sphere.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_title.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_tower.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_tube.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_tunnel.ps
# End Source File
# Begin Source File

SOURCE=.\resource\ps_wave.ps
# End Source File
# Begin Source File

SOURCE=.\resource\texture.bmp
# End Source File
# Begin Source File

SOURCE=.\resource\vs_simple.vs
# End Source File
# End Group
# End Target
# End Project
