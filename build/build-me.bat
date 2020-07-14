@setlocal
@set DOINST=0
@set TMPPRJ=z88dk
@set TMPSRC=..
@set TMPLOG=bldlog-1.txt
@set TMPGEN=Visual Studio 16 2019
@set TMP3RD=D:\Projects\3rdParty.x64
@set TMPCMAKE=cmake
@REM set TMPCMAKE=D:\Projects\cmake\build\bin\Release\cmake.exe

@set TMPOPTS=
@REM set TMPOPTS=%TMPOPTS% -G "%TMPGEN%" -A x64
@set TMPOPTS=%TMPOPTS% -G "%TMPGEN%" -A Win32
@REM set TMPOPTS=%TMPOPTS% -DCMAKE_SYSTEM_VERSION=8.1
@set TMPOPTS=%TMPOPTS% -DCMAKE_INSTALL_PREFIX:PATH=%TMP3RD%
@set TMPOPTS=%TMPOPTS% -DBUILD_Z88DK_UCPP:BOOL=OFF

@echo Being build %TMPPRJ% %DATE% %TIME% >%TMPLOG%
@echo Doing: '%TMPCMAKE% -S %TMPSRC% %TMPOPTS%'
@echo Doing: '%TMPCMAKE% -S %TMPSRC% %TMPOPTS%' >>%TMPLOG%
@%TMPCMAKE% -S %TMPSRC% %TMPOPTS% >>%TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR1

@echo Doing: '%TMPCMAKE% --build . --config Debug'
@echo Doing: '%TMPCMAKE% --build . --config Debug' >>%TMPLOG%
@%TMPCMAKE% --build . --config Debug >>%TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR2

@echo Doing: '%TMPCMAKE% --build . --config Release'
@echo Doing: '%TMPCMAKE% --build . --config Release' >>%TMPLOG%
@%TMPCMAKE% --build . --config Release >>%TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR3
@REM :DONEREL

@echo Appears successful...
@echo.
@if NOT "%DOINST%x" == "1x" (
@echo Install NOT configured! Set DOINST=1
@goto END
)

@echo Continue with install, to %TMP3RD%?
@ask Only 'y' continues. All else aborts...
@if ERRORLEVEL 2 goto NOASK
@if ERRORLEVEL 1 goto INSTALL
@echo Skipping install at this time...
@goto END

:NOASK
@echo Ask utility NOT found...
:INSTALL
@echo Continue with install? Only Ctrl+C aborts
@pause

@echo Doing: '%TMPCMAKE% --build . --config Release --target INSTALL'
@echo Doing: '%TMPCMAKE% --build . --config Release --target INSTALL' >>%TMPLOG%
@%TMPCMAKE% --build . --config Release --target INSTALL >>%TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR4
@echo.
fa4 " -- " %TMPLOG%
@echo.
@echo All done...

@goto END

:ERR1
@echo cmake config, gen error
@goto ISERR

:Err2
@echo build debug error
@goto ISERR

:ERR3
@echo build release error
@goto ISERR

:ERR4
@echo install error
@goto ISERR

:ISERR
@echo See %TMPLOG% for details...
@endlocal
@exit /b 1

:END
@endlocal
@exit /b 0

@REM eof
