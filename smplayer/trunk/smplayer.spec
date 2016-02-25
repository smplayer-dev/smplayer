Name:           smplayer
Version:        16.1.0
%global smtube_ver %{version}
Release:        1%{?dist}
Summary:        A graphical frontend for mplayer

Group:          Applications/Multimedia
License:        GPLv2+
URL:            http://smplayer.sourceforge.net/
Source0:        http://downloads.sourceforge.net/smplayer/smplayer-%{version}.tar.bz2
# Add a servicemenu to enqeue files in smplayer's playlist. 
# see also: 
# https://sourceforge.net/tracker/?func=detail&atid=913576&aid=2052905&group_id=185512
Source1:        smplayer_enqueue_kde4.desktop
Source3:        http://downloads.sourceforge.net/smtube/smtube-%{smtube_ver}.tar.bz2
# Fix regression in Thunar (TODO: re-check in upcoming versions!)
# https://bugzilla.rpmfusion.org/show_bug.cgi?id=1217
Patch0:         smplayer-0.8.3-desktop-files.patch
Patch2:         smplayer-14.9.0.6966-system-qtsingleapplication.patch
Patch3:         smtube-15.5.10-system-qtsingleapplication.patch

BuildRequires:  desktop-file-utils
BuildRequires:  qt4-devel
BuildRequires:  quazip-devel
BuildRequires:  qtsingleapplication-devel
BuildRequires:  qtwebkit-devel
# smplayer without mplayer is quite useless
Requires:       mplayer
Requires:       kde-filesystem
%{?_qt4_version:Requires: qt4%{?_isa} >= %{_qt4_version}}

%description
smplayer intends to be a complete front-end for Mplayer, from basic features
like playing videos, DVDs, and VCDs to more advanced features like support
for Mplayer filters and more. One of the main features is the ability to
remember the state of a played file, so when you play it later it will resume
at the same point and with the same settings. smplayer is developed with
the Qt toolkit, so it's multi-platform.

%prep
%setup -a3 -qn %{name}-%{version}
#remove some bundle sources
rm -rf zlib
rm -rf src/qtsingleapplication/
rm -rf smtube-%{smtube_ver}/src/qtsingleapplication/
#TODO unbundle libmaia
#rm -rf src/findsubtitles/libmaia

%patch0 -p0 -b .desktop-files
%patch2 -p1 -b .qtsingleapplication
pushd smtube-%{smtube_ver}
%patch3 -p1 -b .qtsingleapplication
popd

# correction for wrong-file-end-of-line-encoding
%{__sed} -i 's/\r//' *.txt
# fix files which are not UTF-8 
iconv -f Latin1 -t UTF-8 -o Changelog.utf8 Changelog 
mv Changelog.utf8 Changelog

# fix path of docs
sed -i 's|DOC_PATH=$(PREFIX)/share/doc/packages/smplayer|DOC_PATH=$(PREFIX)/share/doc/smplayer|' Makefile

# use %{?_smp_mflags}
sed -i '/cd src && $(QMAKE) $(QMAKE_OPTS) && $(DEFS) make/s!$! %{?_smp_mflags}!' Makefile

# don't show smplayer_enqueue.desktop in KDE and use servicemenus instead
echo "NotShowIn=KDE;" >> smplayer_enqueue.desktop

%build
make QMAKE=%{_qt4_qmake} PREFIX=%{_prefix} LRELEASE=%{_bindir}/lrelease-qt4

pushd smtube-%{smtube_ver}
sed -i 's|/usr/local|%{_prefix}|' Makefile
sed -i 's|doc/smtube|doc/%{name}/smtube|' Makefile
sed -i 's|smtube/translations|smplayer/translations|' Makefile
make QMAKE=%{_qt4_qmake} PREFIX=%{_prefix} LRELEASE=%{_bindir}/lrelease-qt4
popd


%install
make QMAKE=%{_qt4_qmake} PREFIX=%{_prefix} DESTDIR=%{buildroot}/ install
pushd smtube-%{smtube_ver}
make install DESTDIR=%{buildroot}
popd

desktop-file-install --delete-original                   \
        --vendor "rpmfusion"                             \
        --dir %{buildroot}%{_datadir}/applications/      \
        %{buildroot}%{_datadir}/applications/%{name}.desktop


desktop-file-install --delete-original                   \
        --vendor "rpmfusion"                             \
        --dir %{buildroot}%{_datadir}/applications/      \
        %{buildroot}%{_datadir}/applications/%{name}_enqueue.desktop

desktop-file-validate %{buildroot}%{_datadir}/applications/smtube.desktop

# Add servicemenus dependend on the version of KDE:
# https://sourceforge.net/tracker/index.php?func=detail&aid=2052905&group_id=185512&atid=913576
install -Dpm 0644 %{SOURCE1} %{buildroot}%{_datadir}/kde4/services/ServiceMenus/smplayer_enqueue.desktop

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
%{_bindir}/smplayer
%{_bindir}/smtube
%{_datadir}/applications/rpmfusion-smplayer*.desktop
%{_datadir}/applications/smtube.desktop
%{_datadir}/icons/hicolor/*/apps/smplayer.png
%{_datadir}/icons/hicolor/*/apps/smplayer.svg
%{_datadir}/icons/hicolor/*/apps/smtube.png
%{_datadir}/smplayer/
%dir %{_datadir}/kde4/services/ServiceMenus/
%{_datadir}/kde4/services/ServiceMenus/smplayer_enqueue.desktop
%{_mandir}/man1/smplayer.1.gz
%{_docdir}/%{name}/

%changelog
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
