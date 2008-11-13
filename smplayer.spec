%define name    smplayer
%define version 0.6.5.1
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
Summary:        A great front-end for MPlayer
License:        GPL
Group:          Applications/Multimedia
URL:            http://smplayer.sourceforge.net/
Version:        %{version}
Release:        %{release}

Source0:        %{name}-%{version}.tar.bz2

Packager:       Ricardo Villalba <rvm@escomposlinux.org>
Distribution:   %{distr}
BuildRoot:      %{_tmppath}/%{name}-buildroot
Autoreqprov:    On


%description
SMPlayer intends to be a complete front-end for MPlayer, from basic features 
like playing videos, DVDs, and VCDs to more advanced features like support 
for Mplayer filters and more. One of the main features is the ability to 
remember the state of a played file, so when you play it later it will resume 
at the same point and with the same settings. smplayer is developed with 
the Qt toolkit, so it's multi-platform.

%prep
%setup -q

%build
make PREFIX=/usr QMAKE=%{qmake} LRELEASE=%{lrelease}

%install
make PREFIX=/usr DESTDIR=%{?buildroot:%{buildroot}} install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%attr (-,root,root) /*

%changelog
* Wed May 7 2008 iSage
 - adapted for some distros
* Sun May 20 2007 Ricardo Villalba <rvm@escomposlinux.org>
  - use DESTDIR in make install
* Sat May 5 2007 Ricardo Villalba <rvm@escomposlinux.org>
  - fixed some typos
* Mon Feb 12 2007 Ricardo Villalba <rvm@escomposlinux.org>
  - first spec file
