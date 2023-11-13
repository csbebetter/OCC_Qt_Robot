SET "OCC_PATH=D:\demo\C++\OpenCASCADE-7.7.0-vc14-64\opencascade-7.7.0"

call "%OCC_PATH%\env.bat" %1 %2 %3

set "BIN_DIR=win%ARCH%\%VCVER%\bind"
set "LIB_DIR=win%ARCH%\%VCVER%\libd"

if ["%CASDEB%"] == [""] (
  set "BIN_DIR=win%ARCH%\%VCVER%\bin"
  set "LIB_DIR=win%ARCH%\%VCVER%\lib"
)

set "PATH=%~dp0%LIB_DIR%;%~dp0%BIN_DIR%;%PATH%"