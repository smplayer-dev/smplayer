dxlist is a small console application for Windows which can list the
directsound devices and directdraw display devices. This info is then used
by smplayer.

Usage: just run dxlist.exe with the -d or -s switches. -d shows the display
devices, -s the sound devices.

Compiling: in order to compile it you need the directx headers 
(you can get a package here: http://www.videolan.org/vlc/dx7headers.tgz), and
extract it in the directx folder. Then just compile it with the usual:

qmake
make

Installation: dxlist.exe has to be installed on the same folder as smplayer.exe.

Someone may be wondering... why don't you just simply call mplayer to get that 
info instead of writing this program?

Yes, mplayer can display that info, but there are several inconveniences:

1) it's necessary to use the -v switch. That makes mplayer to be much more 
verbose, that could even have an impact on performace (at least that was
a problem with old versions of Qt)

2) it's necessary to play a file. Yes, you can't get the info if you don't
play a file. That's a problem, smplayer wouldn't be able to get the info until
the user has at least played one file.

3) even worse, to get the info about the attached displays, you have to
use the -adapter option, but how can you know what value to use for 
-adapter if you don't know yet the list of valid values?

Because of those problems, I found easier to try to get that info using other
ways. This program just outputs the required info, no need to play anything,
smplayer can call it whenever it needs it.

Finally, the reason to implement all this in a separate program and not inside
smplayer itself (which would have been easier) is just simply to avoid it
to depend on directx, and require everyone who want to compile it to install 
the directx headers. After all this (dxlist) program is optional, smplayer 
can work without it.
