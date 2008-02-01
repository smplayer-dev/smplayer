REM This batch file can help you to create a directory with all required files
REM Just change the variables at the beginning

REM Warning: it will only work with sources from the SVN and the command svn has to be in the path
REM Note: it doesn't install mplayer

set OUTPUT_DIR=smplayer-build

set SMPLAYER_DIR=svn\smplayer\trunk
set SMPLAYER_THEMES_DIR=svn\smplayer-themes\trunk

set QT_DIR=c:\Qt\4.3.3
set MINGW_DIR=c:\MinGW

mkdir %OUTPUT_DIR%

copy %SMPLAYER_DIR%\src\release\smplayer.exe %OUTPUT_DIR%
copy %SMPLAYER_DIR%\*.txt %OUTPUT_DIR%
copy %QT_DIR%\bin\QtCore4.dll %OUTPUT_DIR%
copy %QT_DIR%\bin\QtGui4.dll %OUTPUT_DIR%
copy %QT_DIR%\bin\QtNetwork4.dll %OUTPUT_DIR%
copy %MINGW_DIR%\bin\mingwm10.dll %OUTPUT_DIR%

rem translations
mkdir %OUTPUT_DIR%\translations
copy %SMPLAYER_DIR%\src\translations\*.qm %OUTPUT_DIR%\translations

rem Qt translations
copy %QT_DIR%\translations\qt_de.qm %OUTPUT_DIR%\translations
copy %QT_DIR%\translations\qt_es.qm %OUTPUT_DIR%\translations
copy %QT_DIR%\translations\qt_ru.qm %OUTPUT_DIR%\translations
copy %QT_DIR%\translations\qt_sk.qm %OUTPUT_DIR%\translations
copy %QT_DIR%\translations\qt_sv.qm %OUTPUT_DIR%\translations
copy %QT_DIR%\translations\qt_zh_CN.qm %OUTPUT_DIR%\translations

rem shortcuts
mkdir %OUTPUT_DIR%\shortcuts
copy %SMPLAYER_DIR%\src\shortcuts\*.keys %OUTPUT_DIR%\shortcuts

rem docs
svn export --force %SMPLAYER_DIR%\docs %OUTPUT_DIR%\docs

rem themes
svn export --force %SMPLAYER_THEMES_DIR%\themes %OUTPUT_DIR%\themes

