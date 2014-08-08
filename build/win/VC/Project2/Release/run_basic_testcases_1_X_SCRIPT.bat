

:: Basic regression test for mgframework
@ECHO OFF

echo|set /p="<html><head><link rel=stylesheet href=mgf.css type=text/css><title>MG Framework test log</title></head><body><h1>MG Framework regression test for script execution</h1><table>"
echo.


:: Create directory for log files..
mkdir .\system_test\result


echo|set /p="<tr><td><p>"
echo|set /p="Test case .\system_test\tc_008_basic_script.mgf<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -script .\system_test\tc_008_basic_script.mgf	>	.\system_test\result\tc_008_basic_script.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\system_test\result\tc_008_basic_script.log -log_compare .\system_test\expected\tc_008_basic_script.log.filtered
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"


echo|set /p="</table></body></html>"
echo.
