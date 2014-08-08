

:: Basic regression test for mgframework
@ECHO OFF
echo Basic regression test for mgframework window


:: Create directory for log files..
mkdir .\system_test\result




:: Run all test cases and save log files..

echo Running test case .\system_test\tc_005_basic_window.mgf
project2.exe -logging -script .\system_test\tc_005_basic_window.mgf	>	.\system_test\result\tc_005_basic_window.log

:: Evaluate log files and save filtered log..

project2.exe -log_eval .\system_test\result\tc_005_basic_window.log -log_compare .\system_test\expected\tc_005_basic_window.log.filtered


pause
