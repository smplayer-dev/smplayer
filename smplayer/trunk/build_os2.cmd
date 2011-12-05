/* smplayer Build Script */
/* root done by Herwig Bauernfeind, enhanced by Silvan Scherrer */

/* version history */
/* version 0.1.0 from 07.09.2009 Silvan (first edition) */
/* version 0.2.2 from 08.09.2009 Herwig (a lot enhancements) */
/* version 0.2.3 from 10.09.2009 Silvan (fixed a slash bug in rootdir) */
/* version 0.2.4 from 19.10.2009 Silvan (enabled lrelease support) */
/* version 0.2.5 from 22.10.2009 Silvan (enabled install) */
/* version 0.2.6 from 29.01.2010 Silvan (added more readmes) */
/* version 0.2.7 from 29.04.2010 Silvan (added smplayer version for sed'ing readme's */
/* version 0.2.8 from 20.05.2010 Silvan (added version to zip) */

/* init the version string (don't forget to change) */
version = "0.2.8"
version_date = "20.05.2010"
smplayer_version = "0.6.9"
smplayer_build = "beta1"
internal_build = "0_6_9-b2"
'@echo off'

parse upper arg cmdline

if cmdline = "" then signal help

/* init the required vars */
qRC = 0
mRC = 0
rootdir    = strip(directory(),'T','\') /* Make sure we have no trailing backslash */
rootdirS   = rootdir || '\'
srcDir     = rootdirS || 'src'
installDir = rootdirS || 'os2'
translationFiles = srcDir || '\translations\smplayer_*.qm'
qErrorFile = rootdirS||'qmake.err'
qOutFile   = rootdirS||'qmake.out'
mErrorFile = rootdirS||'make.err'
mOutFile   = rootdirS||'make.out'

select
    when cmdline = "MAKE CLEAN" then do
        say "Executing command: "cmdline

        ok = directory(srcdir)
        say "cleaning the tree"
        call make 'distclean'
        ok = directory(rootdir)

        say "delete all translated .qm files"
        ok = SysFileTree(translationFiles, rm.,'FO')
        do i = 1 to rm.0
            ok = SysFileDelete(rm.i)
        end

        say "please execute this script again with 'make' to build smplayer"

    end
    when cmdline = "MAKE" | cmdline = "MAKE DEBUG" then do
        say "Executing command: "cmdline

        say "creating makefile for getrev"
        ok = directory(rootdirS||'getrev')
        call qmake

        if qRC = 0 then do
            say "building getrev"
            call make
        end

        ok = directory(rootdir)

        if qRC <> 0 | mRC <> 0 then Signal error

        say "building svn_revision"
        address cmd 'getrev\release\getrev.exe > src\svn_revision.h'

        ok = directory(srcDir)

        say "creating all translated .qm files"
        lrelease smplayer.pro

        say "creating smplayer makefile"
        call qmake

        if qRC = 0 then do
            say "building smplayer"
	    if cmdline = "MAKE" then do
            	call make
	    end
	    else do
		call make 'debug'
	    end
        end

        ok = directory(rootdir)
    end

    when cmdline = "INSTALL" then do
        say "Executing command: "cmdline

/* first delete everything */
	call deleteall

/* create the installDir,the translation and the icon subdir */
	ok = SysMkDir(installDir)
	ok = SysMkDir(installDir||'\translations')
	ok = SysMkDir(installDir||'\icons')

/* copy all translated files */
	ok = SysFileTree(translationFiles, rm.,'FO')
        do i = 1 to rm.0
            ok = SysCopyObject(rm.i, installDir||'\translations')
        end

/* copy the exe and the icon (including rename and setting the new LONGNAME EA) */
	ok = SysCopyObject(srcDir||'\release\smplayer.exe',installDir)
	ok = SysCopyObject(srcDir||'\smplayer_os2.ico',installDir)
	address cmd 'ren 'installDir'\smplayer_os2.ico smplayer.ico'
	ok = SysPutEA(installDir||'\smplayer.ico', '.LONGNAME', 'FDFF'x || d2c(12)||'00'x ||'smplayer.ico')

/* copy some additional icons */
	ok = SysFileTree(rootdirS||'icons\smplayer*.ico', rm.,'FO')
        do i = 1 to rm.0
            ok = SysCopyObject(rm.i, installDir||'\icons')
        end

/* copy the readme */
	rm.0 = 3
	rm.1 = 'readme.os2'
	rm.2 = 'liesmich.os2'
	rm.3 = 'lisezmoi.os2'
	do i = 1 to rm.0
	cmdtorun = 'sed "s;_VERSION_;' || smplayer_version || ';g" ' || rootdirS || rm.i || ' | sed "s;_BUILD_;' || smplayer_build || ';g" >' || rootdirS || 'os2\' || rm.i
        address cmd cmdtorun
	end

/* zip all up */
	ok = directory(installDir)
	cmdtorun = 'zip -r smplayer-' || internal_build || '.zip *'
	address cmd cmdtorun
	ok = directory(rootdir)
    end

    when cmdline = "UNINSTALL" then do
        say "Executing command: "cmdline

	call deleteall
	
    end

    otherwise do
        say 'Unknown parameter "'cmdline'" - aborting...'
        exit 1
    end
end

/* cleanup the mess */
error:

if qRC = 0 & mRC = 0 then do
    ok = SysFileDelete(mOutFile)
    ok = SysFileDelete(mErrorFile)
    ok = SysFileDelete(qOutFile)
    ok = SysFileDelete(qErrorFile)
end
else do
    if mRC <> 0 then do
        say "Alarm! Make errors occured! Look at "mOutFile" and "mErrorFile
    end
    if qRC <> 0 then do
        say "Alarm! qMake errors occured! Look at "qOutFile" and "qErrorFile
    end
end

exit 0

qmake:
    address cmd 'qmake 2>'qErrorFile' 1>'qOutFile
    qRC = RC
    if qRC <> 0 then do
        call beep 880, 20
        say "Alarm! qmake RC="RC
    end
return

make:
    makeparm = arg(1)
    address cmd 'make 'makeparm' 2>'mErrorFile' 1>'mOutFile
    mRC = RC
    if mRC <> 0 then do
        call beep 880, 20
        say "Alarm! make RC="RC
    end
return


deleteall: /* delete everything in installDir (inkluding subdirs) */

	say "Delete the install dir"

	ok = SysFileTree(installDir||'\*', rm.,'FOS')
        do i = 1 to rm.0
            ok = SysFileDelete(rm.i)
        end
	ok = SysFileTree(installDir||'\*', rm.,'OS')
        do i = 1 to rm.0
	    ok = SysRmDir(rm.i)
        end

	ok = SysRmDir(installDir)
	say "wait 30 seconds"
	call SysSleep(30)

return

help:
    title = "smplayer for eCS (OS/2) build script v" || version || " from " || version_date
    say title
    say "Parameters:"
    say "    make"
    say "    make debug"
    say "    make clean"
    say "    install"
    say "    uninstall"
exit 255
