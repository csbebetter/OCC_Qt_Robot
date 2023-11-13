@echo off

Setlocal EnableDelayedExpansion

rem Setup environment
call "%~dp0env.bat" vc143 win64 Debug

rem Define path to project file
set "PRJFILE=OCC_Qt_Robot.sln"

rem Launch Visual Studio - either professional (devenv) or Express, as available
if exist "%DevEnvDir%\devenv.exe"  (
  start "" "%DevEnvDir%\devenv.exe" "%PRJFILE%"
) else if exist "%DevEnvDir%\%VisualStudioExpressName%.exe"  (
  start "" "%DevEnvDir%\%VisualStudioExpressName%.exe" "%PRJFILE%"
) else (
  echo Error: Could not find MS Visual Studio ^(%VCFMT%^)
  echo Check relevant environment variable ^(e.g. VS100COMNTOOLS for vc10^)
)
