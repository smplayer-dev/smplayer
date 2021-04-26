# Changelog

## [Unreleased]
 * Better automatic resizing of the main window, trying to prevent
   black borders.
 * Fix loading YouTube playlists.
 * Add an option to rotate the video by 180 degrees.
 * Playlist: try to prevent a one second delay when using play prev/next.
### Windows
 * Using Qt 5.15.2.
### Linux
 * Disable power saving on Wayland during playback.
 * Remove KDE from the categories in the *.desktop files.

## [21.1] - 2021-01-06
 * Initial support for Wayland.
 * Some fixes and improvements in the playlist.
 * Show cache info in the OSD (Shift + I).

## [20.6] - 2020-06-17
 * The internal YouTube code has been removed. Now SMPlayer uses
   an external application to get the video URLs.
 * The shuffle option in the playlist does shuffle the items of
   the playlist.

## [20.4.2] - 2020-04-13
 * Fix for YouTube.

## [20.4.0] - 2020-04-09
 * Possibility to enter the opensubtitles.org credentials,
   in order to prevent a login error.
 * Now it's possible to search for shortcuts in Preferences -> Keyboard.
 * Support for mpv 0.32.
