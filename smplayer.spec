%define name    smplayer
%define version 0.5.57
%define release rvm

Name:           %{name}
Summary:        A great front-end for MPlayer
License:        GPL
Group:          Applications/Multimedia
URL:            http://smplayer.sourceforge.net/
Version:        %{version}
Release:        %{release}

Source0:        %{name}-%{version}.tar.bz2

Packager:       Ricardo Villalba <rvm@escomposlinux.org>
Distribution:   SUSE Linux 9.2 (i586)
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
make PREFIX=/usr

%install
make PREFIX=/usr DESTDIR=%{?buildroot:%{buildroot}} install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%attr (-,root,root) /*

%changelog
* Sun May 20 2007 Ricardo Villalba <rvm@escomposlinux.org>
  - use DESTDIR in make install
* Sat May 5 2007 Ricardo Villalba <rvm@escomposlinux.org>
  - fixed some typos
* Mon Feb 12 2007 Ricardo Villalba <rvm@escomposlinux.org>
  - first spec file
