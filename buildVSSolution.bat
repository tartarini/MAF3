cd %1/

IF NOT EXIST ..\Install md ..\Install ..\Install\include ..\Install\bin ..\Install\bin\Debug ..\Install\bin\Release

qmake.exe -r -tp vc -spec win32-msvc2008 ./mafWithTest.pro

IF %ERRORLEVEL% NEQ 0 GOTO END
devenv ./mafWithTest.sln /build debug /out build_log.txt
type build_log.txt
IF %ERRORLEVEL% NEQ 0 GOTO END

echo "BUILD SUCCESSFUL"
 
:END
