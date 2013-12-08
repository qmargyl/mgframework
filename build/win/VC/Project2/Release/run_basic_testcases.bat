

:: Basic regression test for mgframework
@ECHO OFF
echo Basic regression test for mgframework



:: Create directory for log files..
mkdir .\test\result




:: Run all test cases and save log files..

echo Running test case .\test\tc_001_basic_exit.mgf
project2.exe -logging -script .\test\tc_001_basic_exit.mgf		>	.\test\result\tc_001_basic_exit.log
project2.exe -log_eval .\test\result\tc_001_basic_exit.log -log_compare .\test\expected\tc_001_basic_exit.log.filtered

echo Running test case .\test\tc_002_basic_mo.mgf
project2.exe -logging -script .\test\tc_002_basic_mo.mgf		>	.\test\result\tc_002_basic_mo.log
project2.exe -log_eval .\test\result\tc_002_basic_mo.log -log_compare .\test\expected\tc_002_basic_mo.log.filtered

echo Running test case .\test\tc_003_basic_runframes.mgf
project2.exe -logging -script .\test\tc_003_basic_runframes.mgf		>	.\test\result\tc_003_basic_runframes.log
project2.exe -log_eval .\test\result\tc_003_basic_runframes.log -log_compare .\test\expected\tc_003_basic_runframes.log.filtered

echo Running test case .\test\tc_004_basic_misc.mgf
project2.exe -logging -script .\test\tc_004_basic_misc.mgf		>	.\test\result\tc_004_basic_misc.log
project2.exe -log_eval .\test\result\tc_004_basic_misc.log -log_compare .\test\expected\tc_004_basic_misc.log.filtered

echo Running test case .\test\tc_005_basic_window.mgf
project2.exe -logging -script .\test\tc_005_basic_window.mgf		>	.\test\result\tc_005_basic_window.log
project2.exe -log_eval .\test\result\tc_005_basic_window.log -log_compare .\test\expected\tc_005_basic_window.log.filtered

echo Running test case .\test\tc_006_basic_pe.mgf
project2.exe -logging -script .\test\tc_006_basic_pe.mgf		>	.\test\result\tc_006_basic_pe.log
project2.exe -log_eval .\test\result\tc_006_basic_pe.log -log_compare .\test\expected\tc_006_basic_pe.log.filtered

echo Running test case .\test\tc_007_basic_map.mgf
project2.exe -logging -script .\test\tc_007_basic_map.mgf		>	.\test\result\tc_007_basic_map.log
project2.exe -log_eval .\test\result\tc_007_basic_map.log -log_compare .\test\expected\tc_007_basic_map.log.filtered

echo Running test case .\test\tc_008_basic_script.mgf
project2.exe -logging -script .\test\tc_008_basic_script.mgf		>	.\test\result\tc_008_basic_script.log
project2.exe -log_eval .\test\result\tc_008_basic_script.log -log_compare .\test\expected\tc_008_basic_script.log.filtered

echo Running server instance with test case .\test\tc_009_basic_server.mgf
project2.exe -server -logging -script .\test\tc_009_basic_server.mgf	>	.\test\result\tc_009_basic_server.log
project2.exe -log_eval .\test\result\tc_009_basic_server.log -log_compare .\test\expected\tc_009_basic_server.log.filtered

echo Running test case .\test\tc_010_basic_path.mgf
project2.exe -logging -script .\test\tc_010_basic_path.mgf		>	.\test\result\tc_010_basic_path.log
project2.exe -log_eval .\test\result\tc_010_basic_path.log -log_compare .\test\expected\tc_010_basic_path.log.filtered

echo Running test case .\test\tc_010_basic_path.mgf:testcase_2
project2.exe -logging -script .\test\tc_010_basic_path.mgf:testcase_2	>	.\test\result\tc_010_basic_path_testcase_2.log
project2.exe -log_eval .\test\result\tc_010_basic_path_testcase_2.log -log_compare .\test\expected\tc_010_basic_path_testcase_2.log.filtered

echo Running test case .\test\tc_011_basic_so.mgf
project2.exe -logging -script .\test\tc_011_basic_so.mgf		>	.\test\result\tc_011_basic_so.log
project2.exe -log_eval .\test\result\tc_011_basic_so.log -log_compare .\test\expected\tc_011_basic_so.log.filtered

echo Running test case .\test\tc_012_basic_symbol.mgf
project2.exe -logging -script .\test\tc_012_basic_symbol.mgf		>	.\test\result\tc_012_basic_symbol.log
project2.exe -log_eval .\test\result\tc_012_basic_symbol.log -log_compare .\test\expected\tc_012_basic_symbol.log.filtered

echo Running test case .\test\tc_013_basic_performance.mgf
project2.exe -logging -script .\test\tc_013_basic_performance.mgf	>	.\test\result\tc_013_basic_performance.log
project2.exe -log_eval .\test\result\tc_013_basic_performance.log -log_compare .\test\expected\tc_013_basic_performance.log.filtered

echo Running test case .\test\tc_999_basic_testcoverage.mgf
project2.exe -logging -script .\test\tc_999_basic_testcoverage.mgf	>	.\test\result\tc_999_basic_testcoverage.log
project2.exe -log_eval .\test\result\tc_999_basic_testcoverage.log -log_compare .\test\expected\tc_999_basic_testcoverage.log.filtered


