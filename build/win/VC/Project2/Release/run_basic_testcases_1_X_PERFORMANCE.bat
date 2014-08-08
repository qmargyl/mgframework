

:: Basic regression test for mgframework
@ECHO OFF
echo Basic regression test for mgframework path calculation


:: Create directory for log files..
mkdir .\system_test\result




:: Run all test cases and save log files..

echo Running test case .\system_test\tc_013_basic_performance.mgf
project2.exe -logging -script .\system_test\tc_013_basic_performance.mgf	>	.\system_test\result\tc_013_basic_performance.log




:: Evaluate all log files..

project2.exe -log_eval .\system_test\result\tc_013_basic_performance.log

pause
