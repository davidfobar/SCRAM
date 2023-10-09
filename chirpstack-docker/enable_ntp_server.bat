@echo off

echo Administrative permissions required. Detecting permissions...
net session >nul 2>&1
if %errorLevel% == 0 (
    echo Success: Administrative permissions confirmed.
) else (
    echo Failure: Current permissions inadequate.
    pause >nul
    exit
)

net stop W32Time
sc config W32Time start=auto
net start W32Time

reg add HKLM\SYSTEM\CurrentControlSet\Services\W32Time\TimeProviders\NtpServer /v Enabled /t REG_DWORD /d 1 /f

w32tm /config /update

netsh advfirewall firewall add rule name="NtpIn" dir=in action=Allow protocol=UDP localport=123 profile=any
netsh advfirewall firewall add rule name="NtpOut" dir=out action=Allow protocol=UDP localport=123 profile=any
