@echo off

echo|set /p="<html><head><link rel=stylesheet href=mgf.css type=text/css><title>MG Framework test log</title></head><body><h1>MG Framework test results</h1><table>"
echo.

call run_basic_testcases.bat

echo|set /p="</table></body></html>"
echo.