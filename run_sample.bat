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
        echo 2
        echo sample_ending_01.txt
        echo 3
    ) | nfa_to_dfa.exe
)

echo.
pause
