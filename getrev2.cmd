@echo off

:: Some SVN clients can use localized messages (e.g. SlikSVN), force English
set LC_ALL=C
set svn_revision=
set use_svn_versions=
set version_cpp=

for /f "tokens=2" %%i in ('svn info ^| find /I "Revision:"') do set svn_revision=%%i

if "%svn_revision%"=="" (
  set svn_revision=UNKNOWN
  echo Unknown SVN revision. SVN missing in PATH or not a working copy.
) else (
  echo SVN Revision: %svn_revision%
)
echo.

echo #define SVN_REVISION "%svn_revision%">src\svn_revision.h

:: Get values of USE_SVN_VERSIONS & VERSION
for /f "tokens=3" %%j in ('type src\version.cpp ^| find /I "USE_SVN_VERSIONS"') do set use_svn_versions=%%j
for /f "tokens=3" %%k in ('type src\version.cpp ^| find /I "#define VERSION"') do set version_cpp=%%k

:: Remove quotes
setlocal enableDelayedExpansion
for /f "delims=" %%A in ("!use_svn_versions!") do endlocal & set "use_svn_versions=%%~A"
setlocal enableDelayedExpansion
for /f "delims=" %%A in ("!version_cpp!") do endlocal & set "version_cpp=%%~A"

:: Verify svn revision & version are actually numbers and version has at least ver_major.ver_minor.ver_build
if defined use_svn_versions (
  echo %svn_revision%|findstr /r /c:"^[0-9][0-9]*$" >nul
  if errorlevel 1 (set use_svn_versions=)
)

if defined version_cpp (
  echo %version_cpp%|findstr /r /c:"^[0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*$" >nul
  if errorlevel 1 (set version_cpp=)
)

if defined version_cpp (
  if [%use_svn_versions%]==[1] (
    echo %version_cpp%.%svn_revision%>setup\scripts\pkg_version
  ) else (
    echo %version_cpp%>setup\scripts\pkg_version
  )
)

set svn_revision=
set use_svn_versions=
set version_cpp=
