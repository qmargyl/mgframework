

:: Feature regression test for mgframework
@ECHO OFF


echo|set /p="<tr><th>Feature system regression test for mgframework</th></tr>"



:: Create directory for log files..
mkdir .\system_test\result




:: Run all test cases and save/analyze log files..

:: Path test cases
:: ===============

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\system_test\tc_401_feature_path.mgf:ext_testcase_001<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\system_test\tc_401_feature_path.mgf:ext_testcase_001	>	.\system_test\result\tc_401_feature_path_testcase_001.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\system_test\result\tc_401_feature_path_testcase_001.log -log_compare .\system_test\expected\tc_401_feature_path_testcase_001.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\system_test\tc_401_feature_path.mgf:ext_testcase_002<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\system_test\tc_401_feature_path.mgf:ext_testcase_002	>	.\system_test\result\tc_401_feature_path_testcase_002.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\system_test\result\tc_401_feature_path_testcase_002.log -log_compare .\system_test\expected\tc_401_feature_path_testcase_002.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\system_test\tc_401_feature_path.mgf:ext_testcase_003<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\system_test\tc_401_feature_path.mgf:ext_testcase_003	>	.\system_test\result\tc_401_feature_path_testcase_003.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\system_test\result\tc_401_feature_path_testcase_003.log -log_compare .\system_test\expected\tc_401_feature_path_testcase_003.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\system_test\tc_401_feature_path.mgf:ext_testcase_004<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\system_test\tc_401_feature_path.mgf:ext_testcase_004	>	.\system_test\result\tc_401_feature_path_testcase_004.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\system_test\result\tc_401_feature_path_testcase_004.log -log_compare .\system_test\expected\tc_401_feature_path_testcase_004.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\system_test\tc_401_feature_path.mgf:ext_testcase_005<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\system_test\tc_401_feature_path.mgf:ext_testcase_005	>	.\system_test\result\tc_401_feature_path_testcase_005.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\system_test\result\tc_401_feature_path_testcase_005.log -log_compare .\system_test\expected\tc_401_feature_path_testcase_005.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\system_test\tc_401_feature_path.mgf:ext_testcase_006 (005 with A*)<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\system_test\tc_401_feature_path.mgf:ext_testcase_006	>	.\system_test\result\tc_401_feature_path_testcase_006.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\system_test\result\tc_401_feature_path_testcase_006.log -log_compare .\system_test\expected\tc_401_feature_path_testcase_006.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\system_test\tc_401_feature_path.mgf:ext_testcase_007 (002 with A*)<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\system_test\tc_401_feature_path.mgf:ext_testcase_007	>	.\system_test\result\tc_401_feature_path_testcase_007.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\system_test\result\tc_401_feature_path_testcase_007.log -log_compare .\system_test\expected\tc_401_feature_path_testcase_007.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

:: Symbol test cases
:: ===============

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\system_test\tc_402_feature_symbol.mgf:ext_testcase_001<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\system_test\tc_402_feature_symbol.mgf:ext_testcase_001	>	.\system_test\result\tc_402_feature_symbol_testcase_001.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\system_test\result\tc_402_feature_symbol_testcase_001.log -log_compare .\system_test\expected\tc_402_feature_symbol_testcase_001.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\system_test\tc_402_feature_symbol.mgf:ext_testcase_002<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\system_test\tc_402_feature_symbol.mgf:ext_testcase_002	>	.\system_test\result\tc_402_feature_symbol_testcase_002.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\system_test\result\tc_402_feature_symbol_testcase_002.log -log_compare .\system_test\expected\tc_402_feature_symbol_testcase_002.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\system_test\tc_402_feature_symbol.mgf:ext_testcase_003<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\system_test\tc_402_feature_symbol.mgf:ext_testcase_003	>	.\system_test\result\tc_402_feature_symbol_testcase_003.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\system_test\result\tc_402_feature_symbol_testcase_003.log -log_compare .\system_test\expected\tc_402_feature_symbol_testcase_003.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

:: Feature 1001: Advanced assignment
echo|set /p="<tr><td><p>"
echo|set /p="Test case: Advanced assignment (tc_402_feature_symbol.mgf:ext_testcase_004)<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\system_test\tc_402_feature_symbol.mgf:ext_testcase_004	>	.\system_test\result\tc_402_feature_symbol_testcase_004.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\system_test\result\tc_402_feature_symbol_testcase_004.log -log_compare .\system_test\expected\tc_402_feature_symbol_testcase_004.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\system_test\tc_402_feature_symbol.mgf:ext_testcase_005<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\system_test\tc_402_feature_symbol.mgf:ext_testcase_005	>	.\system_test\result\tc_402_feature_symbol_testcase_005.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\system_test\result\tc_402_feature_symbol_testcase_005.log -log_compare .\system_test\expected\tc_402_feature_symbol_testcase_005.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"
