

:: Class test for mgframework
@ECHO OFF


echo|set /p="<tr><th>Class test for mgframework</th></tr>"



:: Create directory for log files..
mkdir .\class_test\result




:: Run all test cases and save/analyze log files..

:: Path test cases
:: ===============

echo|set /p="<tr><td><p>"
echo|set /p="Test suite: A* algorithm<br>"
echo|set /p="{<br>Running ... "
project2.exe -logging -classtest astar	>	.\class_test\result\tc_101_astar_suite.log
echo|set /p="<b>DONE</b><br>"
project2.exe -log_eval .\class_test\result\tc_101_astar_suite.log
echo|set /p="}<br>"
echo|set /p="</td></tr></p>"
