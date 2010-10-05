cd %1/

IF NOT EXIST ..\..\Install md ..\..\Install ..\..\Install\bin ..\..\Install\bin\Debug ..\..\Install\bin\Release ..\..\Install\include

mingw32-make.exe clean
qmake.exe -spec win32-g++ -r
IF %ERRORLEVEL% NEQ 0 GOTO END
mingw32-make.exe
IF %ERRORLEVEL% NEQ 0 GOTO END
mingw32-make.exe install
IF %ERRORLEVEL% NEQ 0 GOTO END

echo "BUILD SUCCESSFUL"
cd
:END
