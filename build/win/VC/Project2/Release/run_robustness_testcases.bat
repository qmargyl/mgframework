

:: Basic regression test for mgframework
@ECHO OFF
echo Robustness regression test for mgframework



:: Create directory for log files..
mkdir .\test\result




:: Run all test cases and save log files..

echo Running test case .\test\tc_201_robustness_mo.mgf
project2.exe -logging -script .\test\tc_201_robustness_mo.mgf		>	.\test\result\tc_201_robustness_mo.log
project2.exe -log_eval .\test\result\tc_201_robustness_mo.log

echo Running test case .\test\tc_202_robustness_path.mgf
project2.exe -logging -script .\test\tc_202_robustness_path.mgf		>	.\test\result\tc_202_robustness_path.log
project2.exe -log_eval .\test\result\tc_202_robustness_path.log

pause








