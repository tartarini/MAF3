RMDIR .\Doc /s /q
RMDIR .\DocWithTests /s /q
rem RMDIR .\QAResults /s /q

rem doxygen 1.5.9 used

"%ProgramFiles%\doxygen\bin\doxygen.exe" MAF3Doxyfile
"%ProgramFiles%\doxygen\bin\doxygen.exe" MAF3DoxyfileWithTests

echo "DOXY SUCCESSFUL"