@echo off

:: Some SVN clients can use localized messages (e.g. SlikSVN), force English
set LC_ALL=C

:: Undefine for multiple runs in the same command prompt session
set Revision=
for /f "tokens=2" %%i in ('svn info ^| find "Revision: "') do set Revision=%%i

:: Set to UNKNOWN if no svn or working copy
if "%REVISION%"=="" (
  set REVISION=UNKNOWN
)

if "%Revision%"=="UNKNOWN" (
echo Unknown SVN revision. SVN missing in PATH or not a working copy.
) else (
echo SVN Revision: %Revision%
)
echo.

echo #define SVN_REVISION "SVN-r%REVISION%">src\svn_revision.h