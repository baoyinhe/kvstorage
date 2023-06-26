set "build_type=Debug"
set /p build_type="Choose the type of build (Debug or Release), default Debug:"

mkdir build
cd build

cmake -A Win32 ..
@if ERRORLEVEL 1 goto onError

cmake --build ./  --target install --config %build_type%
@if ERRORLEVEL 1 goto onError

@goto onSuccess

:onError
@echo An error occured!
:onSuccess
@cd ..
