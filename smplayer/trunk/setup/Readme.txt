smplayer-installer.nsi is the script for NSIS (Nullsoft Scriptable Install System), which allows to create
the installer for Windows. This script requires requires NSIS Unicode 2.45 available freely from http://www.scratchpaper.com/home/downloads.
It is intended to be ran one directory up from 'smplayer-build'.

The SMPlayer NSIS script is set up to compile two different setups:
- SMPlayer including MPlayer files
- SMPlayer that downloads MPlayer from a remote server

Downloading MPlayer during install is the default mode. To create an installer with the MPlayer
files you need to define WITH_MPLAYER in the compiler. You need to have the smplayer-build\mplayer files present (not included in SVN):
makensis.exe /DWITH_MPLAYER smplayer-installer.nsi

MakeNSISW (GUI Compiler): Tools -> Settings. Add WITH_MPLAYER in Symbol Name and add to the list. Leave the Value field blank.

Other required files:
- 7za.exe (http://www.7zip.org)
- Unicode inetc plugin (http://nsis.sourceforge.net/Inetc_plug-in)
Also available in: ftp://ftp.berlios.de/pub/smplayer/tools/nsis/

Other required defines w/ values:
VER_MAJOR
VER_MINOR
VER_BUILD
VER_REVISION (only if non-zero)

To build v0.6.8 that includes MPlayer files:
C:\Program Files\NSIS\Unicode\makensis.exe /DWITH_MPLAYER /DVER_MAJOR=0 /DVER_MINOR=6 /DVER_BUILD=8 smplayer-installer.nsi