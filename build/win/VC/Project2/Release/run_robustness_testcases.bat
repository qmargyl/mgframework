

:: Basic regression test for mgframework
@ECHO OFF


echo|set /p="<tr><th>Robustness regression test for mgframework</th></tr>"



:: Create directory for log files..
mkdir .\test\result




:: Run all test cases and save log files..

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_201_robustness_mo.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_201_robustness_mo.mgf		>	.\test\result\tc_201_robustness_mo_1.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_201_robustness_mo_1.log -log_compare .\test\expected\tc_201_robustness_mo_1.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_201_robustness_mo.mgf:ext_testcase_mo_normal<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_201_robustness_mo.mgf:ext_testcase_mo_normal		>	.\test\result\tc_201_robustness_mo_2.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_201_robustness_mo_2.log -log_compare .\test\expected\tc_201_robustness_mo_2.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_201_robustness_mo.mgf:ext_testcase_mo_nofps<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_201_robustness_mo.mgf:ext_testcase_mo_nofps		>	.\test\result\tc_201_robustness_mo_3.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_201_robustness_mo_3.log -log_compare .\test\expected\tc_201_robustness_mo_3.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_202_robustness_path.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_202_robustness_path.mgf		>	.\test\result\tc_202_robustness_path_1.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_202_robustness_path_1.log -log_compare .\test\expected\tc_202_robustness_path_1.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"
