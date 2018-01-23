Name:           smplayer
Version:        18.2.0
%global smplayer_themes_ver 17.3.0
%global smplayer_skins_ver 15.2.0
#%global webfs_ver 1.21
Release:        1%{?dist}
Summary:        A great media player

Group:          Applications/Multimedia
License:        GPL-2.0+
URL:            http://smplayer.sourceforge.net/
Source0:        http://downloads.sourceforge.net/smplayer/smplayer-%{version}.tar.bz2
Source3:        http://downloads.sourceforge.net/smplayer/smplayer-themes-%{smplayer_themes_ver}.tar.bz2
Source4:        http://downloads.sourceforge.net/smplayer/smplayer-skins-%{smplayer_skins_ver}.tar.bz2
#Source5:        https://www.kraxel.org/releases/webfs/webfs-%{webfs_ver}.tar.gz

%if 0%{?suse_version}
BuildRequires:  hicolor-icon-theme
BuildRequires:  libqt5-qttools-devel
BuildRequires:  libQt5Gui-private-headers-devel
%else
BuildRequires:  qt5-qtbase-devel
BuildRequires:  qt5-qttools-devel
%endif
BuildRequires:  desktop-file-utils
BuildRequires:  gcc-c++
#BuildRequires:  pkgconfig(libpulse) >= 0.9
BuildRequires:  pkgconfig(Qt5Concurrent)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  pkgconfig(Qt5PrintSupport)
BuildRequires:  pkgconfig(Qt5Script)
BuildRequires:  pkgconfig(Qt5Sql)
BuildRequires:  pkgconfig(Qt5WebKitWidgets)
BuildRequires:  pkgconfig(Qt5Widgets)
BuildRequires:  pkgconfig(Qt5Xml)

Requires:       mpv
%{?_qt4_version:Requires: qt4%{?_isa} >= %{_qt4_version}}

%description
SMPlayer is a graphical user interface (GUI) for the award-winning mplayer
and also for mpv. But apart from providing access for the most common
and useful options of mplayer and mpv, SMPlayer adds other interesting features
like the possibility to play Youtube videos or search and download subtitles.
One of the main features is the ability to remember the state of a
played file, so when you play it later it will be resumed at the same point
and with the same settings.

%global debug_package %{nil}

%prep
#%setup -a3 -a4 -a5 -qn %{name}-%{version}
%setup -a3 -a4 -qn %{name}-%{version}

# correction for wrong-file-end-of-line-encoding
%{__sed} -i 's/\r//' *.txt
# fix files which are not UTF-8 
iconv -f Latin1 -t UTF-8 -o Changelog.utf8 Changelog
mv Changelog.utf8 Changelog

# change rcc binary
%{__sed} -e 's/rcc -binary/rcc-qt5 -binary/' -i smplayer-themes-%{smplayer_themes_ver}/themes/Makefile
%{__sed} -e 's/rcc -binary/rcc-qt5 -binary/' -i smplayer-skins-%{smplayer_skins_ver}/themes/Makefile

%build
make \
	QMAKE=%{_bindir}/qmake-qt5 \
	LRELEASE=%{_bindir}/lrelease-qt5 \
	PREFIX=%{_prefix} \
	DOC_PATH="\\\"%{_docdir}/%{name}/\\\"" \
	QMAKE_OPTS=DEFINES+=NO_DEBUG_ON_CONSOLE

#touch src/smplayer
#touch src/translations/smplayer_es.qm

pushd smplayer-themes-%{smplayer_themes_ver}
make
popd

pushd smplayer-skins-%{smplayer_skins_ver}
make
popd

#pushd webfs-%{webfs_ver}
#make
#popd

%install
make PREFIX=%{_prefix} DESTDIR=%{buildroot}/ DOC_PATH=%{_docdir}/%{name}/ install

pushd smplayer-themes-%{smplayer_themes_ver}
make install PREFIX=%{_prefix} DESTDIR=%{buildroot}
popd

pushd smplayer-skins-%{smplayer_skins_ver}
make install PREFIX=%{_prefix} DESTDIR=%{buildroot}
popd

#pushd webfs-%{webfs_ver}
#cp webfsd %{buildroot}/usr/bin/webfsd
#popd

%post
touch --no-create %{_datadir}/icons/hicolor
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
  %{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :
fi
update-desktop-database &> /dev/null || :

%postun
touch --no-create %{_datadir}/icons/hicolor
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
  %{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :
fi
update-desktop-database &> /dev/null || :

%files
%defattr(-,root,root)
%{_bindir}/smplayer
%{_datadir}/applications/*.desktop
%dir %{_datadir}/icons/hicolor/*/
%dir %{_datadir}/icons/hicolor/*/apps/
%{_datadir}/icons/hicolor/*/apps/%{name}.*
%{_datadir}/smplayer/
%{_mandir}/man1/smplayer.1.gz
%{_docdir}/%{name}/
#%{_bindir}/webfsd
%{_bindir}/simple_web_server

%changelog
* Thu Feb 25 2016 Ricardo Villalba <rvm@users.sourceforge.net> - 16.1.0
- Remove smtube
- Remove some patches
- Install smplayer-themes and smplayer-skins
- Build with opensuse too

* Sun Jan 17 2016 Sérgio Basto <sergio@serjux.com> - 16.1.0-1
- Update 16.1.0

* Sun Dec 06 2015 Sérgio Basto <sergio@serjux.com> - 15.11.0-1
- Update smplayer and smtube 15.11.0

* Fri Oct 02 2015 Sérgio Basto <sergio@serjux.com> - 15.9.0-1
- Update smplayer to 15.9.0 and smtube to 15.9.0 .

* Thu Aug 20 2015 Sérgio Basto <sergio@serjux.com> - 14.9.0.6994-2
- Update smtube to 15.8.0 .
- Removed version of package from _docdir directory (following the guidelines).

* Wed Jun 17 2015 Sérgio Basto <sergio@serjux.com> - 14.9.0.6994-1
- Update to 4.9.0.6994 .
- Drop smplayer-14.9.0-get_svn_revision.patch .

* Mon Jun 08 2015 Sérgio Basto <sergio@serjux.com> - 14.9.0.6966-3
- Added smplayer-14.9.0-get_svn_revision.patch, I think is better have a
  hardcore version than (svn r0UNKNOWN)

* Sun Jun 07 2015 Sérgio Basto <sergio@serjux.com> - 14.9.0.6966-2
- Update to smtube-15.5.17

* Sat Jun 06 2015 Sérgio Basto <sergio@serjux.com> - 14.9.0.6966-1
- Update to smplayer-14.9.0.6966 and smtube-15.5.10
- Fix warning "The desktop entry file "ServiceMenus/smplayer_enqueue.desktop
  has an empty mimetype! " .
- Rebase patches 2 and 3 .

* Wed Mar 25 2015 Sérgio Basto <sergio@serjux.com> - 14.9.0.6690-1
- Update smplayer to smplayer-14.9.0.6690 and smtube to smtube-15.1.26

* Mon Sep 15 2014 Sérgio Basto <sergio@serjux.com> - 14.9.0-1
- New upstream releases smplayer 14.9.0 and smtube 14.8.0
- Rebase patches 1 and 3 .

* Mon Sep 01 2014 Sérgio Basto <sergio@serjux.com> - 14.3.0-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_21_22_Mass_Rebuild

* Fri Apr 04 2014 Sérgio Basto <sergio@serjux.com> - 14.3.0-1
- New upstream release, Previous version was 0.8.6, this new release is 14.3...
  What happened? Now the version is just the year and month of the release.
- Patches refactor.

* Tue Oct 01 2013 Sérgio Basto <sergio@serjux.com> - 0.8.6-1
- Update smplayer to 0.8.6 and smtube to 1.8

* Mon May 27 2013 Nicolas Chauvet <kwizart@gmail.com> - 0.8.5-2
- Rebuilt for x264/FFmpeg

* Sat May 11 2013 Sérgio Basto <sergio@serjux.com> - 0.8.5-1
- Update smplayer to 0.8.5 and smtube to 1.7
- Fix patches smplayer-0.8.3-smtube-system-qtsingleapplication and
  smplayer-0.8.1-system-qtsingleapplication.patch for 0.8.5 and smtube 1.7

* Mon Mar 25 2013 Sérgio Basto <sergio@serjux.com> - 0.8.4-2
- New tag 

* Mon Mar 25 2013 Sérgio Basto <sergio@serjux.com> - 0.8.4-1
- New upsteam release.
- Drop "updates *.desktop with video/webm;" on patch smplayer-0.8.3-desktop-files.patch.
- Fix patch smplayer-0.8.3-smtube-system-qtsingleapplication.patch 
- Fix dates on changelog specs.

* Thu Jan 10 2013 Sérgio Basto <sergio@serjux.com> - 0.8.3-2
- bug #2635, Update *.desktop with video/webm; mimetype support, as upstream do in svn r5005.

* Mon Dec 24 2012 Sérgio Basto <sergio@serjux.com> - 0.8.3-1
- New updates to smplayer-0.8.3 and smtube-1.5 . Fix for Youtube playback.

* Mon Dec 17 2012 Sérgio Basto <sergio@serjux.com> - 0.8.2.1-1
- New updates to smplayer-0.8.2.1 and smtube-1.4 .

* Sun Nov 25 2012 Sérgio Basto <sergio@serjux.com> - 0.8.2-3
- now smtube new source b372bd396c068aa28798bf2b5385bf59  smtube-1.3.tar.bz2 .

* Sun Nov 25 2012 Sérgio Basto <sergio@serjux.com> - 0.8.2-2
- 0.8.2 new source 0dee3f9a4f0d87d37455efc800f9bba7 smplayer-0.8.2.tar.bz2 this one has some minor
  fixes ... , smplayer-0.8.2.tar.bz2 was announced at 2012-11-24. 

* Thu Nov 22 2012 Sérgio Basto <sergio@serjux.com> - 0.8.2-1
- New upsteam release.

* Thu Sep 27 2012 Sérgio Basto <sergio@serjux.com> - 0.8.1-2
- fix rfbz #2488

* Thu Sep 20 2012 Sérgio Basto <sergio@serjux.com> - 0.8.1-1
- New upsteam release.
- rfbz #2113, all done by Nucleo.

* Sat Apr 28 2012 Sérgio Basto <sergio@serjux.com> - 0.8.0-2
- fix smtube translations.
- drop support for Fedora < 9 and EPEL 5, since we need kde4.

* Sat Apr 28 2012 Sérgio Basto <sergio@serjux.com> - 0.8.0-1 
- New release
- add smtube support
- use system qtsingleapplication
- a little review with: fedora-review -n smplayer --mock-config fedora-16-i386

* Sat Mar 24 2012 Sérgio Basto <sergio@serjux.com> - 0.7.1-1
- New upstream version: 0.7.1, changelog says "This version includes some bug fixes, 
  some of them important. It's highly recommended to update." 
- Remove some bundle sources.
- Small fixes in patches to fit on 0.7.1.

* Sat Mar 24 2012 Sérgio Basto <sergio@serjux.com> - 0.7.0-3
- Add a patch to remove bundled quazip shlibs and Requires kde-filesystem, bug rfbz #1164
- Removed tag BuildRoot.

* Fri Mar 02 2012 Nicolas Chauvet <kwizart@gmail.com> - 0.7.0-2
- Rebuilt for c++ ABI breakage

* Tue Feb 7 2012 Sérgio Basto <sergio@serjux.com> - 0.7.0-1
- new upstream version: 0.7.0

* Mon May 24 2010 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.9-2
- #1217: fix regression in Thunar

* Sat Apr 24 2010 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.9-1
- new upstream version: 0.6.9

* Sun Jun 28 2009 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.8-1
- new upstream version: 0.6.8

* Sun Mar 29 2009 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.7-1
- new upstream version: 0.6.7

* Sun Mar 29 2009 Thorsten Leemhuis <fedora [AT] leemhuis [DOT] info> - 0.6.6-2
- rebuild for new F11 features

* Sat Jan 10 2009 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.6-1
- new upstream version: 0.6.6

* Thu Nov 13 2008 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.5.1-1
- new upstream version: 0.6.5.1

* Wed Oct 29 2008 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.4-1
- new upstream version: 0.6.4

* Mon Sep 29 2008 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.3-1
- new upstream version: 0.6.3

* Fri Aug 15 2008 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.2-1
- new upstream version: 0.6.2
- add servicemenus depending on the KDE version

* Wed Jul 30 2008 Thorsten Leemhuis <fedora [AT] leemhuis [DOT] info - 0.6.1-4
- rebuild for buildsys cflags issue

* Tue Jul 22 2008 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.1-3
- import into rpmfusion

* Tue Jul 08 2008 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.1-2
- fix packaging of FAQs

* Tue Jun 17 2008 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.1-1
- update to latest upstream version

* Sun Feb 24 2008 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.0-0.3.rc2
- add %%{?_smp_mflags} in Makefile to really use it
- finally fix usage of macros
- mode 0644 for desktop-file isn't needed anymore

* Sat Feb 23 2008 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.0-0.2.rc2
- Update %%post and %%postun scriplets
- use %%{?_smp_mflags} in make
- change vendor to rpmfusion in desktop-file-install
- some minor spec cleanups

* Thu Feb 14 2008 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.0-0.1.rc2
- new upstream version: 0.6.0rc2

* Tue Feb 12 2008 Sebastian Vahl <fedora@deadbabylon.de> - 0.6.0-0.1.rc1
- new upstream version: 0.6.0rc1
- added docs: Changelog Copying.txt Readme.txt Release_notes.txt
- fix path of %%docdir in Makefile

* Tue Dec 18 2007 Sebastian Vahl <fedora@deadbabylon.de> - 0.5.62-1
- new version: 0.5.62
- specify license as GPLv2+

* Thu Sep 20 2007 Sebastian Vahl <fedora@deadbabylon.de> - 0.5.60-1
- Update to development version of qt4

* Thu Sep 20 2007 Sebastian Vahl <fedora@deadbabylon.de> - 0.5.21-1
- new upstream version: 0.5.21
- don't add category "Multimedia" to desktop-file
- correct url of Source0

* Sun Jul 29 2007 Sebastian Vahl <fedora@deadbabylon.de> - 0.5.20-1
- new upstream version: 0.5.20

* Mon Jun 18 2007 Sebastian Vahl <fedora@deadbabylon.de> - 0.5.14-1
- new upstream version: 0.5.14

* Thu Jun 14 2007 Sebastian Vahl <fedora@deadbabylon.de> - 0.5.7-1
- Initial Release
