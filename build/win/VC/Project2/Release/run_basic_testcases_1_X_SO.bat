

:: Basic regression test for mgframework
@ECHO OFF
echo Basic regression test for mgframework path calculation


:: Create directory for log files..
mkdir .\system_test\result




:: Run all test cases and save log files..

echo Running test case .\system_test\tc_011_basic_so.mgf
project2.exe -logging -script .\system_test\tc_011_basic_so.mgf	>	.\system_test\result\tc_011_basic_so.log




:: Evaluate all log files..

project2.exe -log_eval .\system_test\result\tc_011_basic_so.log

pause
