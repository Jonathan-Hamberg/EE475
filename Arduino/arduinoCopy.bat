REM Copy the libraries directory from the EE475 directory to the users arduino
REM library directory.  The MIR option is enabled so it will mirror the
REM directory ( delete files that don't exist in source directory)
REM This batch file should be run whenever changes have been made to the Arduino
REM libraries.
Robocopy Libraries %USERPROFILE%\Documents\Arduino\libraries\EE475 /MIR
