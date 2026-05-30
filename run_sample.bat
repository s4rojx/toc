@echo off
setlocal

if not exist nfa_to_dfa.exe (
    set "RUN_PROGRAM=0"
    call build.bat
)

if exist nfa_to_dfa.exe (
    echo Running sample input...
    echo.
    (
        echo 3
        echo 2
        echo 0 1
        echo 0
        echo 1
        echo 2
        echo 4
        echo 0 0 0
        echo 0 1 0
        echo 0 0 1
        echo 1 1 2
    ) | nfa_to_dfa.exe
)

echo.
pause
