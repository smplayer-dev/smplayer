/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2008 Ricardo Villalba <rvm@escomposlinux.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#define DIRECTSOUND_VERSION 5
#include <stdio.h>
#include <dsound.h>
#include <ddraw.h>

int sound_devices;
int display_devices;

BOOL CALLBACK DirectSoundEnum(LPGUID guid, LPCSTR desc, LPCSTR module, LPVOID context)
{		
	printf("%i: %s\n", sound_devices, desc);
	
	sound_devices++;
	
	return TRUE;
}

BOOL WINAPI DirectDrawEnum(GUID FAR *lpGUID, LPSTR lpDriverDescription, LPSTR lpDriverName, LPVOID lpContext, HMONITOR  hm)
{
	if (!lpGUID) 
		printf("%i: %s\n", display_devices, "Primary Display Adapter");
	else
		printf("%i: %s\n", display_devices, lpDriverDescription);

	display_devices++;
	
	return TRUE;
}

int main(int argc, char* argv[])
{
	if (argc != 2){
		printf("Usage: dxlist.exe [-sound] [-s] [-display] [-d]\n");
		return 1;
	}

	if ((strcmp(argv[1],"-sound")==0) || (strcmp(argv[1],"-s")==0)) {
		sound_devices = 0;
		printf("Sound devices:\n");
		if (DirectSoundEnumerateA(DirectSoundEnum, NULL) != DS_OK){
			printf("Error: can't list the audio devices\n");
		}
	}
	else
	if ((strcmp(argv[1],"-display")==0) || (strcmp(argv[1],"-d")==0)) {	
		display_devices = 0;
		printf("Display devices:\n");
		if (DirectDrawEnumerateExA(DirectDrawEnum, NULL, DDENUM_ATTACHEDSECONDARYDEVICES) != DS_OK) {
			printf("Error: can't list the display devices\n");
		}
	}
	else
	printf("Unknown option %s\n", argv[1]);

	return 0;
}

