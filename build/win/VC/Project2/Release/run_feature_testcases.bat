

:: Feature regression test for mgframework
@ECHO OFF


echo|set /p="<tr><th>Feature regression test for mgframework</th></tr>"



:: Create directory for log files..
mkdir .\test\result




:: Run all test cases and save/analyze log files..

:: Path test cases
:: ===============

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_401_feature_path.mgf:ext_testcase_001<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\test\tc_401_feature_path.mgf:ext_testcase_001	>	.\test\result\tc_401_feature_path_testcase_001.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_401_feature_path_testcase_001.log -log_compare .\test\expected\tc_401_feature_path_testcase_001.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_401_feature_path.mgf:ext_testcase_002<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\test\tc_401_feature_path.mgf:ext_testcase_002	>	.\test\result\tc_401_feature_path_testcase_002.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_401_feature_path_testcase_002.log -log_compare .\test\expected\tc_401_feature_path_testcase_002.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_401_feature_path.mgf:ext_testcase_003<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\test\tc_401_feature_path.mgf:ext_testcase_003	>	.\test\result\tc_401_feature_path_testcase_003.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_401_feature_path_testcase_003.log -log_compare .\test\expected\tc_401_feature_path_testcase_003.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_401_feature_path.mgf:ext_testcase_004<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\test\tc_401_feature_path.mgf:ext_testcase_004	>	.\test\result\tc_401_feature_path_testcase_004.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_401_feature_path_testcase_004.log -log_compare .\test\expected\tc_401_feature_path_testcase_004.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_401_feature_path.mgf:ext_testcase_005<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\test\tc_401_feature_path.mgf:ext_testcase_005	>	.\test\result\tc_401_feature_path_testcase_005.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_401_feature_path_testcase_005.log -log_compare .\test\expected\tc_401_feature_path_testcase_005.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"




:: Path test cases
:: ===============

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_402_feature_symbol.mgf:ext_testcase_001<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\test\tc_402_feature_symbol.mgf:ext_testcase_001	>	.\test\result\tc_402_feature_symbol_testcase_001.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_402_feature_symbol_testcase_001.log -log_compare .\test\expected\tc_402_feature_symbol_testcase_001.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_402_feature_symbol.mgf:ext_testcase_002<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\test\tc_402_feature_symbol.mgf:ext_testcase_002	>	.\test\result\tc_402_feature_symbol_testcase_002.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_402_feature_symbol_testcase_002.log -log_compare .\test\expected\tc_402_feature_symbol_testcase_002.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_402_feature_symbol.mgf:ext_testcase_003<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\test\tc_402_feature_symbol.mgf:ext_testcase_003	>	.\test\result\tc_402_feature_symbol_testcase_003.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_402_feature_symbol_testcase_003.log -log_compare .\test\expected\tc_402_feature_symbol_testcase_003.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"
