

:: Basic regression test for mgframework
@ECHO OFF
echo Basic regression test for mgframework



:: Create directory for log files..
mkdir .\test\result




:: Run all test cases and save log files..

echo Running test case .\test\tc_001_basic_exit.mgf
project2.exe -logging -script .\test\tc_001_basic_exit.mgf		>	.\test\result\tc_001_basic_exit.log
project2.exe -log_eval .\test\result\tc_001_basic_exit.log

echo Running test case .\test\tc_002_basic_mo.mgf
project2.exe -logging -script .\test\tc_002_basic_mo.mgf		>	.\test\result\tc_002_basic_mo.log
project2.exe -log_eval .\test\result\tc_002_basic_mo.log

echo Running test case .\test\tc_003_basic_runframes.mgf
project2.exe -logging -script .\test\tc_003_basic_runframes.mgf		>	.\test\result\tc_003_basic_runframes.log
project2.exe -log_eval .\test\result\tc_003_basic_runframes.log

echo Running test case .\test\tc_004_basic_misc.mgf
project2.exe -logging -script .\test\tc_004_basic_misc.mgf		>	.\test\result\tc_004_basic_misc.log
project2.exe -log_eval .\test\result\tc_004_basic_misc.log

echo Running test case .\test\tc_005_basic_window.mgf
project2.exe -logging -script .\test\tc_005_basic_window.mgf		>	.\test\result\tc_005_basic_window.log
project2.exe -log_eval .\test\result\tc_005_basic_window.log

echo Running test case .\test\tc_006_basic_pe.mgf
project2.exe -logging -script .\test\tc_006_basic_pe.mgf		>	.\test\result\tc_006_basic_pe.log
project2.exe -log_eval .\test\result\tc_006_basic_pe.log

echo Running test case .\test\tc_007_basic_map.mgf
project2.exe -logging -script .\test\tc_007_basic_map.mgf		>	.\test\result\tc_007_basic_map.log
project2.exe -log_eval .\test\result\tc_007_basic_map.log

echo Running test case .\test\tc_008_basic_script.mgf
project2.exe -logging -script .\test\tc_008_basic_script.mgf		>	.\test\result\tc_008_basic_script.log
project2.exe -log_eval .\test\result\tc_008_basic_script.log

echo Running server instance with test case .\test\tc_009_basic_server.mgf
project2.exe -server -logging -script .\test\tc_009_basic_server.mgf	>	.\test\result\tc_009_basic_server.log
project2.exe -log_eval .\test\result\tc_009_basic_server.log

echo Running test case .\test\tc_010_basic_path.mgf
project2.exe -logging -script .\test\tc_010_basic_path.mgf		>	.\test\result\tc_010_basic_path.log
project2.exe -log_eval .\test\result\tc_010_basic_path.log

echo Running test case .\test\tc_010_basic_path.mgf:testcase_2
project2.exe -logging -script .\test\tc_010_basic_path.mgf:testcase_2	>	.\test\result\tc_010_basic_path_tescase_2.log
project2.exe -log_eval .\test\result\tc_010_basic_path_tescase_2.log

echo Running test case .\test\tc_011_basic_so.mgf
project2.exe -logging -script .\test\tc_011_basic_so.mgf		>	.\test\result\tc_011_basic_so.log
project2.exe -log_eval .\test\result\tc_011_basic_so.log

echo Running test case .\test\tc_012_basic_symbol.mgf
project2.exe -logging -script .\test\tc_012_basic_symbol.mgf		>	.\test\result\tc_012_basic_symbol.log
project2.exe -log_eval .\test\result\tc_012_basic_symbol.log

echo Running test case .\test\tc_013_basic_performance.mgf
project2.exe -logging -script .\test\tc_013_basic_performance.mgf	>	.\test\result\tc_013_basic_performance.log
project2.exe -log_eval .\test\result\tc_013_basic_performance.log

echo Running test case .\test\tc_999_basic_testcoverage.mgf
project2.exe -logging -script .\test\tc_999_basic_testcoverage.mgf	>	.\test\result\tc_999_basic_testcoverage.log
project2.exe -log_eval .\test\result\tc_999_basic_testcoverage.log


