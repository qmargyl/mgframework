@echo off

echo|set /p="<html><head><link rel=stylesheet href=mgf.css type=text/css><title>MG Framework test log</title></head><body><table>"
echo.

call run_basic_testcases.bat

call run_feature_testcases.bat

call run_robustness_testcases.bat

echo|set /p="</table></body></html>"
echo.