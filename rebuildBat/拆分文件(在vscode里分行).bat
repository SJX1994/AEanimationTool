@echo off
color 0a
echo ˵�������Ҫ�ָ���ı��ļ�������Ϊtest.txt����BAT�����TXT����ͬһĿ¼�¡�
echo �����밴�����зָ�TXT�ļ������س���
set /p n=
powershell -c "$n=1;$m=1;gc 'good.txt'|%%{$f=''+$m+'.txt';$_>>$f;if($n%%%n% -eq 0){$m++};$n++}" 
pause