

:: Basic regression test for mgframework
@ECHO OFF
echo Basic regression test for mgframework runframes command


:: Create directory for log files..
mkdir .\test\result




:: Run all test cases and save log files..

echo Running test case .\test\tc_003_basic_runframes.mgf
project2.exe -logging -script .\test\tc_003_basic_runframes.mgf	>	.\test\result\tc_003_basic_runframes.log

:: Evaluate log files and save filtered log..

project2.exe -log_eval .\test\result\tc_003_basic_runframes.log -log_compare .\test\expected\tc_003_basic_runframes.log.filtered


pause
