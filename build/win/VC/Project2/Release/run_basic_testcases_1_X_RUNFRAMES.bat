

:: Basic regression test for mgframework
@ECHO OFF
echo Basic regression test for mgframework runframes command


:: Create directory for log files..
mkdir .\system_test\result




:: Run all test cases and save log files..

echo Running test case .\system_test\tc_003_basic_runframes.mgf
project2.exe -logging -script .\system_test\tc_003_basic_runframes.mgf	>	.\system_test\result\tc_003_basic_runframes.log

:: Evaluate log files and save filtered log..

project2.exe -log_eval .\system_test\result\tc_003_basic_runframes.log -log_compare .\system_test\expected\tc_003_basic_runframes.log.filtered


pause
