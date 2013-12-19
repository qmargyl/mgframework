

:: Basic regression test for mgframework
@ECHO OFF
echo Basic regression test for mgframework path calculation


:: Create directory for log files..
mkdir .\test\result




:: Run all test cases and save log files..

echo Test case .\test\tc_008_basic_script.mgf
project2.exe -logging -script .\test\tc_008_basic_script.mgf	>	.\test\result\tc_008_basic_script.log

:: Evaluate log files and save filtered log..

project2.exe -log_eval .\test\result\tc_008_basic_script.log -log_compare .\test\expected\tc_008_basic_script.log.filtered


pause
