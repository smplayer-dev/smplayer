/* SMPlayer Build Script */
/* root done by Herwig Bauernfeind, enhanced by Silvan Scherrer */

/* version history */
/* version 0.1.0 from 07.09.2009 Silvan (first edition) */
/* version 0.2.2 from 08.09.2009 Herwig (a lot enhancements) */
/* version 0.2.3 from 10.09.2009 Silvan (fixed a slash bug in rootdir) */
/* version 0.2.4 from 19.10.2009 Silvan (enabled lrelease support) */
/* version 0.2.5 from 22.10.2009 Silvan (enabled install) */
/* version 0.2.6 from 29.01.2010 Silvan (added more readmes) */
/* version 0.2.7 from 29.04.2010 Silvan (added SMPlayer version for sed'ing readme's) */
/* version 0.2.8 from 20.05.2010 Silvan (added version to zip) */
/* version 0.2.9 from 11.11.2011 Silvan (adapted to Qt 4.7.3) */
/* version 0.3.0 from 24.12.2011 Silvan (added shadow build) */
/* version 0.3.1 from 16.03.2012 Silvan (get the version from version.cpp) */

/* init the version string (don't forget to change) */
version = "0.3.1"
version_date = "16.03.2012"
'@echo off'

parse arg command option
parse source . . scriptFile

/* init the required vars */
qRC = 0
mRC = 0
buildDir    = strip(directory(),'T','\') /* Make sure we have no trailing backslash */
sourceDir = FixDir(filespec('D', scriptFile) || filespec('P', scriptFile))
os2Dir     = sourceDir || '\os2'
srcDir     = sourceDir || '\src'
installDir = buildDir || '\install'
qErrorFile = buildDir||'\qmake.err'
qOutFile   = buildDir||'\qmake.out'
mErrorFile = buildDir||'\make.err'
mOutFile   = buildDir||'\make.out'

/* get the SMPlayer version */
SMPlayer_version = '0.0.0'
call version
internal_build = translate(SMPlayer_version, '_', '.')

title = "SMPlayer for eCS (OS/2) build script v" || version || " from " || version_date
say title
say
say "Build directory:" buildDir
say "Source directory:" sourceDir
say
say "SMPlayer version:" SMPlayer_version
say

/* translate command to all upercase */
command = translate(command)

if command = "" then signal help

if command = "INSTALL" then do
    SMPlayer_build = option
    select
	when SMPlayer_build \== "" then do
	  zipFile = installDir || '\SMPlayer-' || internal_build || '-' || SMPlayer_build || '.zip'
	end
	otherwise do
	  signal help
	end
    end 
end

/* now we translate also the options */
options = translate(options)

if sourceDir \== buildDir then do
    say "Shadow build in progress ..."
    say
end

select
    when command = "MAKE" & option = "CLEAN" then do
        say "Executing command: "command option

        say "cleaning the tree"
        call make 'distclean'

        say "please execute this script again with 'make' to build SMPlayer"

    end
    when command = "MAKE" then do
        say "Executing command: "command option

        say "building svn_revision"
	ok = SysMkDir(buildDir||'\src')
        address cmd 'sh ' sourceDir||'\get_svn_revision.sh ' sourceDir ' "eCS(OS/2) build"'

        say "creating SMPlayer makefile"
        call qmake

        if qRC = 0 then do
            say "building SMPlayer"
	    if option = "" then do
            	call make
	    end
	    else do
		call make 'debug'
	    end
        end

    end

    when command = "INSTALL" then do
        say "Executing command: "command

/* first delete everything */
	call deleteall

/* create the installDir,the translation and the icon subdir */
	ok = SysMkDir(installDir)
	ok = SysMkDir(installDir||'\translations')

/* copy the exe */
	ok = SysCopyObject(buildDir||'\src\smplayer.exe',installDir)

/* copy the readme */
	rm.0 = 3
	rm.1 = 'readme.os2'
	rm.2 = 'liesmich.os2'
	rm.3 = 'lisezmoi.os2'
	do i = 1 to rm.0
	cmdtorun = 'sed "s;_VERSION_;' || SMPlayer_version || ';g" ' || os2Dir || '\' || rm.i || ' | sed "s;_BUILD_;' || SMPlayer_build || ';g" >' || installDir || '\' || rm.i
        address cmd cmdtorun
	end

/* create the qm files from ts files */
	ok = SysFileTree(srcDir||'\translations\*.ts', rm.,'FO')
        do i = 1 to rm.0
	    fileName = filespec('N',rm.i)
	    fileName = left(fileName,lastpos('.', fileName)-1) || '.qm'
            cmdtorun = 'lrelease ' || rm.i || ' -qm ' || installDir || '\translations\' || fileName
	    address cmd cmdtorun
        end

/* zip all dynamic stuff */
	ok = directory(installDir)
	cmdtorun = 'zip -r ' || zipFile || ' *'
	address cmd cmdtorun
        ok = directory(buildDir)

/* zip all icons */
	ok = directory(os2Dir)
	cmdtorun = 'zip ' || zipFile || ' *.ico'
	address cmd cmdtorun
	ok = directory(buildDir)

    end

    when command = "UNINSTALL" then do
        say "Executing command: "command

	call deleteall
	
    end

    otherwise do
        say 'Unknown parameter "'command'" - aborting...'
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
    address cmd 'qmake ' sourceDir ' 2>'qErrorFile' 1>'qOutFile
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

/**
 *  Fixes the directory path by a) converting all slashes to back
 *  slashes and b) ensuring that the trailing slash is present if
 *  the directory is the root directory, and absent otherwise.
 *
 *  @param dir      the directory path
 *  @param noslash
 *      optional argument. If 1, the path returned will not have a
 *      trailing slash anyway. Useful for concatenating it with a
 *      file name.
 */
FixDir: procedure expose (Globals)
    parse arg dir, noslash
    noslash = (noslash = 1)
    dir = translate(dir, '\', '/')
    if (right(dir, 1) == '\' &,
        (noslash | \(length(dir) == 3 & (substr(dir, 2, 1) == ':')))) then
        dir = substr(dir, 1, length(dir) - 1)
    return dir

/**
 *  reads the version.cpp and gets the SMPlayer version from there
 */ 
version: procedure expose SMPlayer_version srcDir

    SMPlayerVer = ' '
    /* SMPlayer Version file */
    Version = srcDir || "\version.cpp"

    do until lines(Version) = 0
        verline = linein(Version)
        if left(Verline,15) = "#define VERSION" then do
            parse var verline . ' '. ' ' SMPlayerVer
        end
    end

    ok = stream(Version,'c','close')
    if SMPlayerVer \== ' ' then do
    	SMPlayer_version = strip(SMPlayerVer,,'"')
    end

    return

help:
    say "Parameters:"
    say "    make"
    say "    make debug"
    say "    make clean"
    say "    install version"
    say "    uninstall"
exit 255
