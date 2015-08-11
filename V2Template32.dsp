# Microsoft Developer Studio Project File - Name="V2Template32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=V2Template32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "V2Template32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "V2Template32.mak" CFG="V2Template32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "V2Template32 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "V2Template32 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "V2Template32 - Win32 Run_Only" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "V2Template32 - Win32 Release_Small" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "V2Template32 - Win32 Run_Only_Small" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "V2Template32 - Win32 Vitalize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "V2Template32 - Win32 Vitalize_Small" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "V2Template32 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "V2TEMPLATE32_EXPORTS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "EDIT_MODE" /D "INCLUDE_DIALOG" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\inc" /d "NDEBUG" /d "EDITOR" /d "EDIT_MODE" /d "INCLUDE_DIALOG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 comctl32.lib shlwapi.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib ..\lib\mmfs2.lib /nologo /dll /machine:I386 /out:".\Release/INI++15.mfx"

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "V2TEMPLATE32_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "EDIT_MODE" /D "INCLUDE_DIALOG" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\inc" /d "_DEBUG" /d "EDITOR" /d "EDIT_MODE" /d "INCLUDE_DIALOG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 comctl32.lib shlwapi.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib ..\lib\mmfs2.lib /nologo /dll /debug /machine:I386 /out:"C:\Program Files\Multimedia Fusion Developer 2\Extensions\INI++.mfx"

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "V2Template32___Win32_Run_Only"
# PROP BASE Intermediate_Dir "V2Template32___Win32_Run_Only"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Run_Only"
# PROP Intermediate_Dir ".\Run_Only"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /D "NO_INCLUDE_DIALOG" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\inc" /d "NDEBUG" /d "RUN_ONLY" /d "NO_INCLUDE_DIALOG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\lib\cncs32.lib /nologo /dll /machine:I386
# ADD LINK32 comctl32.lib shlwapi.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib ..\lib\mmfs2.lib /nologo /dll /machine:I386 /out:".\Run_Only/INI++15.mfx"

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Release_Small"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "V2Template32___Win32_Release_Small"
# PROP BASE Intermediate_Dir "V2Template32___Win32_Release_Small"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release_Small"
# PROP Intermediate_Dir ".\Release_Small"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /YX /FD /c
# ADD CPP /nologo /W3 /O1 /I "..\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "COMPILE_SMALL" /D "EDIT_MODE" /D "INCLUDE_DIALOG" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\inc" /d "NDEBUG" /d "EDITOR" /d "EDIT_MODE" /d "INCLUDE_DIALOG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\lib\cncs32.lib /nologo /dll /machine:I386 /out:".\Release/V2Template32.cox"
# ADD LINK32 comctl32.lib shell32.lib shlwapi.lib kernel32.lib user32.lib ..\lib\mmfs2.lib /nologo /dll /machine:I386 /out:".\Release_Small/INI++15.mfx"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only_Small"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "V2Template32___Win32_Run_Only_Small"
# PROP BASE Intermediate_Dir "V2Template32___Win32_Run_Only_Small"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Run_Only_Small"
# PROP Intermediate_Dir ".\Run_Only_Small"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /YX /FD /c
# ADD CPP /nologo /W3 /O1 /I "..\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /D "COMPILE_SMALL" /D "INCLUDE_DIALOG" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "RUN_ONLY"
# ADD RSC /l 0x409 /i "..\inc" /d "NDEBUG" /d "RUN_ONLY" /d "INCLUDE_DIALOG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\lib\cncs32.lib /nologo /dll /machine:I386 /out:".\Run_Only/V2Template32.cox"
# ADD LINK32 comctl32.lib shell32.lib shlwapi.lib kernel32.lib user32.lib ..\lib\mmfs2.lib /nologo /dll /machine:I386 /out:".\Run_Only_Small/INI++15.mfx"
# SUBTRACT LINK32 /incremental:yes /nodefaultlib

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "V2Template32___Win32_Vitalize"
# PROP BASE Intermediate_Dir "V2Template32___Win32_Vitalize"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Vitalize"
# PROP Intermediate_Dir ".\Vitalize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /D "VITALIZE" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "RUN_ONLY"
# ADD RSC /l 0x409 /i "..\..\inc" /d "NDEBUG" /d "RUN_ONLY" /d "VITALIZE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\lib\cncs32.lib /nologo /dll /machine:I386 /out:".\Run_Only/V2Template32.cox"
# ADD LINK32 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib ..\..\lib\mmfs2.lib /nologo /dll /machine:I386 /out:".\Vitalize\V2Template32.mox"

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize_Small"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "V2Template32___Win32_Vitalize_Small"
# PROP BASE Intermediate_Dir "V2Template32___Win32_Vitalize_Small"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Vitalize_Small"
# PROP Intermediate_Dir ".\Vitalize_Small"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /D "COMPILE_SMALL" /YX /FD /c
# ADD CPP /nologo /W3 /O1 /I "..\..\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /D "COMPILE_SMALL" /D "VITALIZE" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "RUN_ONLY"
# ADD RSC /l 0x409 /i "..\..\inc" /d "NDEBUG" /d "RUN_ONLY" /d "VITALIZE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib msvcrt.lib ..\lib\cncs32.lib /nologo /dll /machine:I386 /nodefaultlib /out:".\Run_Only_Small/V2Template32.cox"
# ADD LINK32 msvcrt.lib kernel32.lib user32.lib ..\..\lib\mmfs2.lib /nologo /dll /machine:I386 /nodefaultlib /out:".\Vitalize_Small/V2Template32.mox"

!ENDIF 

# Begin Target

# Name "V2Template32 - Win32 Release"
# Name "V2Template32 - Win32 Debug"
# Name "V2Template32 - Win32 Run_Only"
# Name "V2Template32 - Win32 Release_Small"
# Name "V2Template32 - Win32 Run_Only_Small"
# Name "V2Template32 - Win32 Vitalize"
# Name "V2Template32 - Win32 Vitalize_Small"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "zlib"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=.\zlib\compress.c
# End Source File
# Begin Source File

SOURCE=.\zlib\crc32.c
# End Source File
# Begin Source File

SOURCE=.\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=.\zlib\example.c
# End Source File
# Begin Source File

SOURCE=.\zlib\gzclose.c
# End Source File
# Begin Source File

SOURCE=.\zlib\gzlib.c
# End Source File
# Begin Source File

SOURCE=.\zlib\gzread.c
# End Source File
# Begin Source File

SOURCE=.\zlib\gzwrite.c
# End Source File
# Begin Source File

SOURCE=.\zlib\infback.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=.\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.c
# End Source File
# End Group
# Begin Source File

SOURCE=".\common dialog.cpp"
# End Source File
# Begin Source File

SOURCE=.\Edittime.cpp
# End Source File
# Begin Source File

SOURCE=.\Extobj.rc

!IF  "$(CFG)" == "V2Template32 - Win32 Release"

# PROP Intermediate_Dir ".\Release"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Debug"

# PROP Intermediate_Dir ".\Debug"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only"

# PROP BASE Intermediate_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Release_Small"

# PROP BASE Intermediate_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only_Small"

# PROP BASE Intermediate_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize"

# PROP BASE Intermediate_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize_Small"

# PROP BASE Intermediate_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Extobj32.def
# End Source File
# Begin Source File

SOURCE=.\General.cpp
# End Source File
# Begin Source File

SOURCE=.\helper_to_main.cpp
# End Source File
# Begin Source File

SOURCE=.\Huffman.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\md5c.c
# End Source File
# Begin Source File

SOURCE=.\md5class.cpp
# End Source File
# Begin Source File

SOURCE=.\MMFArrayWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime.cpp
# End Source File
# Begin Source File

SOURCE=.\SuperINI.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\alphanum.hpp
# End Source File
# Begin Source File

SOURCE=.\ChartObject.h
# End Source File
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\common_dialog.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\helper_to_main.h
# End Source File
# Begin Source File

SOURCE=.\LoadSaveChartProperties.h
# End Source File
# Begin Source File

SOURCE=.\Main.h
# End Source File
# Begin Source File

SOURCE=.\MD5.h
# End Source File
# Begin Source File

SOURCE=.\md5class.h
# End Source File
# Begin Source File

SOURCE=.\MMFArrayWrapper.h
# End Source File
# Begin Source File

SOURCE=.\new_vector.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SuperINI.h
# End Source File
# Begin Source File

SOURCE=.\UndoInformation.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zlib.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\EXTICON.BMP
# End Source File
# Begin Source File

SOURCE=.\EXTIMG.BMP
# End Source File
# Begin Source File

SOURCE=.\EXTIMG2.BMP
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\icon2.ico
# End Source File
# End Group
# Begin Group "Information"

# PROP Default_Filter "txt"
# Begin Source File

SOURCE=..\Debug.txt

!IF  "$(CFG)" == "V2Template32 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Release_Small"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only_Small"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize_Small"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Extdev.hlp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\Global_Object_Code.zip
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\Graphic_Object_Ex.txt
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\Identifier.exe

!IF  "$(CFG)" == "V2Template32 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Release_Small"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only_Small"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize_Small"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\MMFSDK15.txt

!IF  "$(CFG)" == "V2Template32 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Release_Small"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only_Small"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize_Small"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\OEFLAGS.txt
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\SDK2_Changes.txt

!IF  "$(CFG)" == "V2Template32 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Release_Small"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only_Small"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize_Small"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Surfaces.txt

!IF  "$(CFG)" == "V2Template32 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Release_Small"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only_Small"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize_Small"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Vitalizing.txt

!IF  "$(CFG)" == "V2Template32 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Release_Small"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Run_Only_Small"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "V2Template32 - Win32 Vitalize_Small"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "MMF Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE="C:\Program Files\Microsoft Visual Studio\Vc98\Include\Basetsd.h"
# End Source File
# Begin Source File

SOURCE=..\Inc\Ccx.h
# End Source File
# Begin Source File

SOURCE=..\Inc\Ccxhdr.h
# End Source File
# Begin Source File

SOURCE=..\Inc\Mmf\Cncf.h
# End Source File
# Begin Source File

SOURCE=..\Inc\Mmf\Cncy.h
# End Source File
# Begin Source File

SOURCE=..\Inc\Cnpdll.h
# End Source File
# Begin Source File

SOURCE=..\Inc\colors.h
# End Source File
# Begin Source File

SOURCE=..\Inc\Mmf\Evtccx.h
# End Source File
# Begin Source File

SOURCE=..\Inc\Ptype.h
# End Source File
# Begin Source File

SOURCE=..\Inc\Surface.h
# End Source File
# Begin Source File

SOURCE=..\Inc\WinMacro.h
# End Source File
# End Group
# End Target
# End Project
