

:: Feature regression test for mgframework
@ECHO OFF
echo Feature regression test for mgframework



:: Create directory for log files..
mkdir .\test\result




:: Run all test cases and save log files..

:: Path test cases
:: ===============
echo Running test case .\test\tc_401_feature_path.mgf:ext_testcase_001
project2.exe -logging -script .\test\tc_401_feature_path.mgf:ext_testcase_001	>	.\test\result\tc_401_feature_path_testcase_001.log
project2.exe -log_eval .\test\result\tc_401_feature_path_testcase_001.log

echo Running test case .\test\tc_401_feature_path.mgf:ext_testcase_002
project2.exe -logging -script .\test\tc_401_feature_path.mgf:ext_testcase_002	>	.\test\result\tc_401_feature_path_testcase_002.log
project2.exe -log_eval .\test\result\tc_401_feature_path_testcase_002.log

echo Running test case .\test\tc_401_feature_path.mgf:ext_testcase_003
project2.exe -logging -script .\test\tc_401_feature_path.mgf:ext_testcase_003	>	.\test\result\tc_401_feature_path_testcase_003.log
project2.exe -log_eval .\test\result\tc_401_feature_path_testcase_003.log

echo Running test case .\test\tc_401_feature_path.mgf:ext_testcase_004
project2.exe -logging -script .\test\tc_401_feature_path.mgf:ext_testcase_004	>	.\test\result\tc_401_feature_path_testcase_004.log
project2.exe -log_eval .\test\result\tc_401_feature_path_testcase_004.log

echo Running test case .\test\tc_401_feature_path.mgf:ext_testcase_005
project2.exe -logging -script .\test\tc_401_feature_path.mgf:ext_testcase_005	>	.\test\result\tc_401_feature_path_testcase_005.log
project2.exe -log_eval .\test\result\tc_401_feature_path_testcase_005.log





:: Path test cases
:: ===============
echo Running test case .\test\tc_402_feature_symbol.mgf:ext_testcase_001
project2.exe -logging -script .\test\tc_402_feature_symbol.mgf:ext_testcase_001	>	.\test\result\tc_402_feature_symbol_testcase_001.log
project2.exe -log_eval .\test\result\tc_402_feature_symbol_testcase_001.log

echo Running test case .\test\tc_402_feature_symbol.mgf:ext_testcase_002
project2.exe -logging -script .\test\tc_402_feature_symbol.mgf:ext_testcase_002	>	.\test\result\tc_402_feature_symbol_testcase_002.log
project2.exe -log_eval .\test\result\tc_402_feature_symbol_testcase_002.log

::echo Running test case .\test\tc_402_feature_symbol.mgf:ext_testcase_003
::project2.exe -logging -script .\test\tc_402_feature_symbol.mgf:ext_testcase_003	>	.\test\result\tc_402_feature_symbol_testcase_003.log
::project2.exe -log_eval .\test\result\tc_402_feature_symbol_testcase_003.log

::echo Running test case .\test\tc_402_feature_symbol.mgf:ext_testcase_004
::project2.exe -logging -script .\test\tc_402_feature_symbol.mgf:ext_testcase_004	>	.\test\result\tc_402_feature_symbol_testcase_004.log
::project2.exe -log_eval .\test\result\tc_402_feature_symbol_testcase_004.log

::echo Running test case .\test\tc_402_feature_symbol.mgf:ext_testcase_005
::project2.exe -logging -script .\test\tc_402_feature_symbol.mgf:ext_testcase_005	>	.\test\result\tc_402_feature_symbol_testcase_005.log
::project2.exe -log_eval .\test\result\tc_402_feature_symbol_testcase_005.log



pause








