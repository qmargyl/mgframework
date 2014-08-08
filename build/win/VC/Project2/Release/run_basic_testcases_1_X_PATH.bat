

:: Basic regression test for mgframework
@ECHO OFF
echo Basic regression test for mgframework path calculation


:: Create directory for log files..
mkdir .\system_test\result




:: Run all test cases and save log files..

echo Running test case .\system_test\tc_010_basic_path.mgf
project2.exe -logging -no_random -script .\system_test\tc_010_basic_path.mgf	>	.\system_test\result\tc_010_basic_path.log




:: Evaluate all log files..

project2.exe -log_eval .\system_test\result\tc_010_basic_path.log -log_compare .\system_test\expected\tc_010_basic_path.log.filtered

pause
