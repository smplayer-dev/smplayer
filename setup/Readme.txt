smplayer-installer.nsi is the script for NSIS (Nullsoft Scriptable Install System), which allows to create
the installer for Windows. This script requires requires NSIS 2.x available freely from http://nsis.sourceforge.net.
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
- inetc plugin (http://nsis.sourceforge.net/Inetc_plug-in)

Other required defines w/ values:
VER_MAJOR
VER_MINOR
VER_BUILD
VER_REVISION (only if non-zero)

To build v0.6.8 that includes MPlayer files:
C:\Program Files\NSIS\makensis.exe /DWITH_MPLAYER /DVER_MAJOR=0 /DVER_MINOR=6 /DVER_BUILD=8 smplayer-installer.nsi