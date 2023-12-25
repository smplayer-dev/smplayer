# Changelog

## [23.12] - 2023-12-26
 * Implemented support for the new Opensubtitles API.
 * Fix play/pause with mpv 0.37.
 * Fix loading playlists with extension m3u8.
 * Fix video jitter/shudder when clicking on timeline bar.
 * Other small fixes.

## [23.6] - 2023-06-30
 * Fix the issue with YouTube playback limited to 720p.
 * Update mpv and mplayer.
 * New option in preferences to keep the playback speed for all videos.
 * Fix the AppImage package.
 * Other bug fixes.

## [22.7] - 2022-07-10
 * Better statistic info in the OSD (Shift + I).
 * Bug fixes regarding MPRIS2.
 * MPRIS2 is compiled (but disabled by default) on Windows and Mac OS.

## [22.2] - 2022-02-27
### Linux and Mac OS
 * Fix the option to send video to another screen.
 * Fix the option to choose the audio output device.
### Flatpak package
 * Add support for the AV1 codec.
### Mac OS
 * Fix colors in HDR videos.
 * SMPlayer is now also compatible with Mac OS 10.14 (Mojave).
### Other changes
 * Fix: the playlist is shuffled after adding files if the shuffle button is on.
 * Possibility to choose whether to show the media title or the filename
   in the playlist.

## [21.10] - 2021-10-29
 * The support for YouTube has been improved by using yt-dlp. Fixes
   buffering and playback of age-restricted videos.
 * Possibility to use yt-dlp instead of youtube-dl.
 * Option to use the AV1 codec with YouTube videos.
 * Improve support for FreeBSD.
 * Auto-rotate videos that have been recorded in vertical mode or upside down.
 * Some bug fixes.
### Linux
 * Ignore KDE's "single click". It causes unexpected behaviour.
### Windows
 * It's easier to update the YouTube code.
 * The installer is digitally signed.
 * SMPlayer is now also available in the Windows Store.
### Mac OS
 * Fix playback with mplayer when screenshots are enabled.
### Removed
 * Support for the obsolete mplayer2.

## [21.8] - 2021-08-16
 * A version for Mac OS is now available.
 * Better automatic resizing of the main window, trying to prevent
   black borders.
 * Fix loading YouTube playlists.
 * Add an option to rotate the video by 180 degrees.
 * Playlist: try to prevent a one second delay when using play prev/next.
 * Add some predefined speeds (0.25x, 0.5x, 1.25x, 1.5x, 1.75x).
 * Fix audio CD playback with mpv.
 * Fix audio channels.
### Windows
 * The installation of YouTube support is now optional.
 * Using Qt 5.15.2.
### Linux
 * Disable power saving on Wayland during playback.
 * Remove KDE from the categories in the *.desktop files.
 * SMPlayer is now also available as appimage, flatpak and snap.

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
