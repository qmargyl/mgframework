

:: Basic regression test for mgframework
@ECHO OFF
echo Basic regression test for mgframework path calculation


:: Create directory for log files..
mkdir .\test\result




:: Run all test cases and save log files..

echo Running test case .\test\tc_202_robustness_path.mgf
project2.exe -logging -script .\test\tc_202_robustness_path.mgf	>	.\test\result\tc_202_robustness_path_1.log
project2.exe -log_eval .\test\result\tc_202_robustness_path_1.log

echo Running test case .\test\tc_202_robustness_path.mgf
project2.exe -logging -script .\test\tc_202_robustness_path.mgf	>	.\test\result\tc_202_robustness_path_2.log
project2.exe -log_eval .\test\result\tc_202_robustness_path_2.log

echo Running test case .\test\tc_202_robustness_path.mgf
project2.exe -logging -script .\test\tc_202_robustness_path.mgf	>	.\test\result\tc_202_robustness_path_3.log
project2.exe -log_eval .\test\result\tc_202_robustness_path_3.log

pause
