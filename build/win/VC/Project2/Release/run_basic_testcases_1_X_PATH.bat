

:: Basic regression test for mgframework
@ECHO OFF
echo Basic regression test for mgframework



:: Create directory for log files..
mkdir .\test\result




:: Run all test cases and save log files..

echo Running test case .\test\tc_010_basic_path.mgf
project2.exe -logging -script .\test\tc_010_basic_path.mgf	>	.\test\result\tc_010_basic_path.log




:: Evaluate all log files..

project2.exe -log_eval .\test\result\tc_010_basic_path.log

pause
