%define name    smplayer
%define version 16.1.0+r7427
%define release rvm

%define is_mandrake %(test -e /etc/mandrake-release && echo 1 || echo 0)
%define is_suse %(test -e /etc/SuSE-release && echo 1 || echo 0)
%define is_fedora %(test -e /etc/fedora-release && echo 1 || echo 0)
%define qmake qmake
%define lrelease lrelease

%if %is_fedora
%define distr %(cat /etc/fedora-release)
%define qmake qmake-qt4
%define lrelease lrelease-qt4
%endif
%if %is_suse
%define distr %(head -1 /etc/SuSE-release)
%endif
%if %is_mandrake
%define distr %(cat /etc/mandrake-release)
%endif

Name:           %{name}
Summary:        A great media player
License:        GPL
Group:          Applications/Multimedia
URL:            http://smplayer.sourceforge.net/
Version:        %{version}
Release:        %{release}

Source0:        %{name}-%{version}.tar.bz2

Packager:       Ricardo Villalba <rvm@users.sourceforge.net>
Distribution:   %{distr}
BuildRoot:      %{_tmppath}/%{name}-buildroot
Autoreqprov:    On
Requires:       mpv


%description
SMPlayer is a graphical user interface (GUI) for the award-winning mplayer
and also for mpv. But apart from providing access for the most common
and useful options of mplayer and mpv, SMPlayer adds other interesting features
like the possibility to play Youtube videos or search and download subtitles.
One of the main features is the ability to remember the state of a
played file, so when you play it later it will be resumed at the same point
and with the same settings.

%prep
%setup -q

%build
make PREFIX=/usr QMAKE=%{qmake} LRELEASE=%{lrelease}

%install
make PREFIX=/usr DESTDIR=%{?buildroot:%{buildroot}} install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc %dir %{_docdir}/packages/%{name}
%doc %{_docdir}/packages/%{name}/Changelog
%doc %{_docdir}/packages/%{name}/*.txt
%doc %{_docdir}/packages/%{name}/*/*
%{_bindir}/smplayer
%{_datadir}/applications/smplayer.desktop
%{_datadir}/applications/smplayer_enqueue.desktop
%{_datadir}/icons/*/*/apps/smplayer.*
%dir %{_datadir}/smplayer
%config %{_datadir}/smplayer/input.conf
%dir %{_datadir}/smplayer/shortcuts
%{_datadir}/smplayer/shortcuts/*.keys
%dir %{_datadir}/smplayer/translations
%{_datadir}/smplayer/translations/*.qm
%doc %{_mandir}/man1/smplayer.1.gz


%changelog
* Wed Feb 24 2016 Ricardo Villalba <rvm@users.sourceforge.net>
 - update
* Wed May 7 2008 iSage
 - adapted for some distros
* Sun May 20 2007 Ricardo Villalba <rvm@users.sourceforge.net>
  - use DESTDIR in make install
* Sat May 5 2007 Ricardo Villalba <rvm@users.sourceforge.net>
  - fixed some typos
* Mon Feb 12 2007 Ricardo Villalba <rvm@users.sourceforge.net>
  - first spec file
