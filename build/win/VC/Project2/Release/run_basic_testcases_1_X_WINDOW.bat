

:: Basic regression test for mgframework
@ECHO OFF
echo Basic regression test for mgframework window


:: Create directory for log files..
mkdir .\test\result




:: Run all test cases and save log files..

echo Running test case .\test\tc_005_basic_window.mgf
project2.exe -logging -script .\test\tc_005_basic_window.mgf	>	.\test\result\tc_005_basic_window.log

:: Evaluate log files and save filtered log..

project2.exe -log_eval .\test\result\tc_005_basic_window.log -log_compare .\test\expected\tc_005_basic_window.log.filtered


pause
