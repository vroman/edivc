# Microsoft Developer Studio Project File - Name="ediv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ediv - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ediv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ediv.mak" CFG="ediv - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ediv - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ediv - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ediv - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\inc" /I "..\..\shared" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zlib.lib ws2_32.lib /nologo /subsystem:console /machine:I386 /out:"..\..\..\bin\ediv.exe" /libpath:"..\..\..\lib"

!ELSEIF  "$(CFG)" == "ediv - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "..\..\..\inc" /I "..\..\shared" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sdlmain.lib sdl.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\bin\ediv.exe" /pdbtype:sept /libpath:"..\..\..\lib"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "ediv - Win32 Release"
# Name "ediv - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "edivcfg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ediv\edivcfg\dictionary.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\edivcfg\dictionary.h
# End Source File
# Begin Source File

SOURCE=..\..\ediv\edivcfg\e_error.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\edivcfg\e_error.h
# End Source File
# Begin Source File

SOURCE=..\..\ediv\edivcfg\edivcfg.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\edivcfg\iniparser.h
# End Source File
# Begin Source File

SOURCE=..\..\ediv\edivcfg\mainparse.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\edivcfg\strlib.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\edivcfg\strlib.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\ediv\compiler.c
# End Source File
# Begin Source File

SOURCE=..\..\shared\dll_load.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\ediv.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\ediv_export.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\encrypt.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\expresion.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\language.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\listados.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\lower.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\ltlex.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\modulos.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\parser.c
# End Source File
# Begin Source File

SOURCE=..\..\ediv\varindex.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\ediv\compiler.h
# End Source File
# Begin Source File

SOURCE=..\..\shared\dll_load.h
# End Source File
# Begin Source File

SOURCE=..\..\ediv\encrypt.h
# End Source File
# Begin Source File

SOURCE=..\..\ediv\expresion.h
# End Source File
# Begin Source File

SOURCE=..\..\shared\extern.h
# End Source File
# Begin Source File

SOURCE=..\..\shared\language.h
# End Source File
# Begin Source File

SOURCE=..\..\ediv\listados.h
# End Source File
# Begin Source File

SOURCE=..\..\ediv\lower.h
# End Source File
# Begin Source File

SOURCE=..\..\ediv\main.h
# End Source File
# Begin Source File

SOURCE=..\..\ediv\modulos.h
# End Source File
# Begin Source File

SOURCE=..\..\ediv\parser.h
# End Source File
# Begin Source File

SOURCE=..\..\shared\shared.h
# End Source File
# Begin Source File

SOURCE=..\..\shared\varindex.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
