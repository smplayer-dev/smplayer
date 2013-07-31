@echo off

:: Some SVN clients can use localized messages (e.g. SlikSVN), force English
set LC_ALL=C
set SVN_REV=

for /f "tokens=2" %%i in ('svn info ^| find "Revision:"') do set SVN_REV=%%i

:: Set to UNKNOWN if no svn or working copy
if "%SVN_REV%"=="" (
  set SVN_REV=UNKNOWN
  echo Unknown SVN revision. SVN missing in PATH or not a working copy.
) else (
  echo SVN Revision: %SVN_REV%
)
echo.

echo #define SVN_REVISION "%SVN_REV%">src\svn_revision.h

set SVN_REV=
