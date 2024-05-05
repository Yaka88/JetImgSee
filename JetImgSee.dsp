# Microsoft Developer Studio Project File - Name="JetImgSee" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=JetImgSee - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JetImgSee.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JetImgSee.mak" CFG="JetImgSee - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JetImgSee - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JetImgSee - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JetImgSee - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"D:\VCProjects\JetImgCtrl\release\JetImgSee.dll"

!ELSEIF  "$(CFG)" == "JetImgSee - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"D:\VCProjects\JetImgCtrl\TestJetImgCtrl\debug\jetimgsee.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "JetImgSee - Win32 Release"
# Name "JetImgSee - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BMP.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportGIF.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportJPG.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportPCX.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportTGA.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportTiff.cpp
# End Source File
# Begin Source File

SOURCE=.\GIF_LZW.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportGIF.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportICO.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportJPG.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportPCX.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportTGA.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportTiff.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportWMF.cpp
# End Source File
# Begin Source File

SOURCE=.\JetImgSee.cpp
# End Source File
# Begin Source File

SOURCE=.\JetImgSee.def
# End Source File
# Begin Source File

SOURCE=.\JetImgSee.rc
# End Source File
# Begin Source File

SOURCE=.\JetImgSeeApi.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGCFdct.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGCHuff.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGCInit.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGCMark.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGCMcu.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGCSAMP.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGDExpa.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGDHuff.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGDIDct.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGDInit.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGDMark.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGDMcu.cpp
# End Source File
# Begin Source File

SOURCE=.\McsUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\PCXRLE.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TGA_RLE.cpp
# End Source File
# Begin Source File

SOURCE=.\TGARaw.cpp
# End Source File
# Begin Source File

SOURCE=.\TIF_LZW.cpp
# End Source File
# Begin Source File

SOURCE=.\TIFCCCITT.cpp
# End Source File
# Begin Source File

SOURCE=.\TIFDCCITT.cpp
# End Source File
# Begin Source File

SOURCE=.\TIFPackBits.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BMP.h
# End Source File
# Begin Source File

SOURCE=.\ExportGIF.h
# End Source File
# Begin Source File

SOURCE=.\ExportJPG.h
# End Source File
# Begin Source File

SOURCE=.\ExportPCX.h
# End Source File
# Begin Source File

SOURCE=.\ExportTGA.h
# End Source File
# Begin Source File

SOURCE=.\ExportTiff.h
# End Source File
# Begin Source File

SOURCE=.\GIF_LZW.h
# End Source File
# Begin Source File

SOURCE=.\ImportGIF.h
# End Source File
# Begin Source File

SOURCE=.\ImportICO.h
# End Source File
# Begin Source File

SOURCE=.\ImportJPG.h
# End Source File
# Begin Source File

SOURCE=.\ImportPCX.h
# End Source File
# Begin Source File

SOURCE=.\ImportTGA.h
# End Source File
# Begin Source File

SOURCE=.\ImportTiff.h
# End Source File
# Begin Source File

SOURCE=.\ImportWMF.h
# End Source File
# Begin Source File

SOURCE=.\JetImgSee.h
# End Source File
# Begin Source File

SOURCE=.\JetImgSeeApi.h
# End Source File
# Begin Source File

SOURCE=.\JPGCFdct.h
# End Source File
# Begin Source File

SOURCE=.\JPGCHuff.h
# End Source File
# Begin Source File

SOURCE=.\JPGCInit.h
# End Source File
# Begin Source File

SOURCE=.\JPGCMark.h
# End Source File
# Begin Source File

SOURCE=.\JPGCMcu.h
# End Source File
# Begin Source File

SOURCE=.\JPGCSAMP.h
# End Source File
# Begin Source File

SOURCE=.\JPGDExpa.h
# End Source File
# Begin Source File

SOURCE=.\JPGDHuff.h
# End Source File
# Begin Source File

SOURCE=.\JPGDIDct.h
# End Source File
# Begin Source File

SOURCE=.\JPGDInit.h
# End Source File
# Begin Source File

SOURCE=.\JPGDMark.h
# End Source File
# Begin Source File

SOURCE=.\JPGDMcu.h
# End Source File
# Begin Source File

SOURCE=.\mcsdef.h
# End Source File
# Begin Source File

SOURCE=.\mcsgif.h
# End Source File
# Begin Source File

SOURCE=.\mcsico.h
# End Source File
# Begin Source File

SOURCE=.\mcsjpeg.h
# End Source File
# Begin Source File

SOURCE=.\mcspcx.h
# End Source File
# Begin Source File

SOURCE=.\mcstga.h
# End Source File
# Begin Source File

SOURCE=.\mcstif.h
# End Source File
# Begin Source File

SOURCE=.\McsUtil.h
# End Source File
# Begin Source File

SOURCE=.\mcswmf.h
# End Source File
# Begin Source File

SOURCE=.\PCXRLE.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TGA_RLE.h
# End Source File
# Begin Source File

SOURCE=.\TGARaw.h
# End Source File
# Begin Source File

SOURCE=.\TIF_LZW.h
# End Source File
# Begin Source File

SOURCE=.\TIFCCCITT.h
# End Source File
# Begin Source File

SOURCE=.\TIFDCCITT.h
# End Source File
# Begin Source File

SOURCE=.\TIFPackBits.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
