@echo off
setlocal

set "APP=nfa_to_dfa.exe"
set "RESULT=0"
if not defined RUN_PROGRAM set "RUN_PROGRAM=1"

echo Building NFA to DFA project...
echo.

where gcc >nul 2>nul
if %errorlevel%==0 (
    echo Found GCC.
    gcc -Wall -Wextra -std=c11 main.c nfa.c dfa.c -o "%APP%"
    set "RESULT=%errorlevel%"
    goto after_build
)

where clang >nul 2>nul
if %errorlevel%==0 (
    echo Found Clang.
    clang -Wall -Wextra -std=c11 main.c nfa.c dfa.c -o "%APP%"
    set "RESULT=%errorlevel%"
    goto after_build
)

where cl >nul 2>nul
if %errorlevel%==0 (
    echo Found Microsoft C compiler.
    cl /nologo /W4 main.c nfa.c dfa.c /Fe:"%APP%"
    set "RESULT=%errorlevel%"
    goto after_build
)

echo ERROR: No C compiler was found.
echo.
echo Install MinGW-w64 or MSYS2, then make sure gcc is added to PATH.
echo After installation, close and reopen VS Code or Command Prompt.
set "RESULT=1"
goto end

:after_build
if "%RESULT%"=="0" (
    echo.
    echo Build successful: %APP%
    if "%RUN_PROGRAM%"=="1" (
        echo Starting program...
        echo.
        "%APP%"
    )
) else (
    echo.
    echo Build failed. Please read the compiler errors above.
)

:end
echo.
pause
exit /b %RESULT%
