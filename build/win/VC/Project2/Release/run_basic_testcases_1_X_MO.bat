

:: Basic regression test for mgframework
@ECHO OFF
echo Basic regression test for mgframework path calculation


:: Create directory for log files..
mkdir .\test\result




:: Run all test cases and save log files..

echo Running test case .\test\tc_002_basic_mo.mgf
project2.exe -logging -script .\test\tc_002_basic_mo.mgf	>	.\test\result\tc_002_basic_mo.log




:: Evaluate all log files..

project2.exe -log_eval .\test\result\tc_002_basic_mo.log

pause