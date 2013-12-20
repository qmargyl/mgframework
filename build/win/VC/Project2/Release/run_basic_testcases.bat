

:: Basic regression test for mgframework
@ECHO OFF


echo|set /p="<tr><th>Basic regression test for mgframework</th></tr>"



:: Create directory for log files..
mkdir .\test\result


:: Run all test cases and save log files..

echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_001_basic_exit.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_001_basic_exit.mgf		>	.\test\result\tc_001_basic_exit.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_001_basic_exit.log -log_compare .\test\expected\tc_001_basic_exit.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_002_basic_mo.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_002_basic_mo.mgf		>	.\test\result\tc_002_basic_mo.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_002_basic_mo.log -log_compare .\test\expected\tc_002_basic_mo.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_003_basic_runframes.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_003_basic_runframes.mgf		>	.\test\result\tc_003_basic_runframes.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_003_basic_runframes.log -log_compare .\test\expected\tc_003_basic_runframes.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_004_basic_misc.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_004_basic_misc.mgf		>	.\test\result\tc_004_basic_misc.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_004_basic_misc.log -log_compare .\test\expected\tc_004_basic_misc.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_005_basic_window.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging-no_random  -script .\test\tc_005_basic_window.mgf		>	.\test\result\tc_005_basic_window.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_005_basic_window.log -log_compare .\test\expected\tc_005_basic_window.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_006_basic_pe.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_006_basic_pe.mgf		>	.\test\result\tc_006_basic_pe.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_006_basic_pe.log -log_compare .\test\expected\tc_006_basic_pe.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_007_basic_map.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_007_basic_map.mgf		>	.\test\result\tc_007_basic_map.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_007_basic_map.log -log_compare .\test\expected\tc_007_basic_map.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_008_basic_script.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_008_basic_script.mgf		>	.\test\result\tc_008_basic_script.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_008_basic_script.log -log_compare .\test\expected\tc_008_basic_script.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_009_basic_server.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -server -no_random -logging -script .\test\tc_009_basic_server.mgf	>	.\test\result\tc_009_basic_server.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_009_basic_server.log -log_compare .\test\expected\tc_009_basic_server.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_010_basic_path.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_010_basic_path.mgf		>	.\test\result\tc_010_basic_path.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_010_basic_path.log -log_compare .\test\expected\tc_010_basic_path.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_010_basic_path.mgf:testcase_2<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_010_basic_path.mgf:testcase_2	>	.\test\result\tc_010_basic_path_testcase_2.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_010_basic_path_testcase_2.log -log_compare .\test\expected\tc_010_basic_path_testcase_2.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_011_basic_so.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_011_basic_so.mgf		>	.\test\result\tc_011_basic_so.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_011_basic_so.log -log_compare .\test\expected\tc_011_basic_so.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_012_basic_symbol.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_012_basic_symbol.mgf		>	.\test\result\tc_012_basic_symbol.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_012_basic_symbol.log -log_compare .\test\expected\tc_012_basic_symbol.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_013_basic_performance.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_013_basic_performance.mgf	>	.\test\result\tc_013_basic_performance.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_013_basic_performance.log -log_compare .\test\expected\tc_013_basic_performance.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\test\tc_999_basic_testcoverage.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -no_random -script .\test\tc_999_basic_testcoverage.mgf	>	.\test\result\tc_999_basic_testcoverage.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\test\result\tc_999_basic_testcoverage.log -log_compare .\test\expected\tc_999_basic_testcoverage.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"
