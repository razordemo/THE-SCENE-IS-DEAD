..\kkrunchy.exe --best --refsize 64 --out .\rzr-intro_kkr.exe .\release\rzr-intro.exe
..\kkrunchy_k7.exe --best --refsize 64 --out .\rzr-intro_kkr_k7.exe .\release\rzr-intro.exe
..\kkrunchy_023a4_asm07.exe --best --refsize 64 --out .\rzr-intro_kkr_asm07.exe .\release\rzr-intro.exe
..\beroexepacker.exe .\release\rzr-intro.exe .\rzr-intro_bero.exe +CA=LZMAU
rem ..\apk2.exe .\release\rzr-intro.exe -o .\rzr-intro_apk2.exe -s -p0
rem ..\upx.exe --best .\release\rzr-intro.exe
rem ..\upx.exe --ultra-brute --best .\release\rzr-intro.exe
rem ..\crinkler.exe /OUT:.\release\rzr-intro.exe /SUBSYSTEM:WINDOWS /RANGE:opengl32 /COMPMODE:SLOW /ORDERTRIES:1000 /PROGRESSGUI
