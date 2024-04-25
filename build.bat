if exist build rmdir /s /q build
if exist exe rmdir /s /q exe

mkdir build
cd build
cmake ..
cmake --build . --config Release


cd ..
mkdir exe

mkdir exe\client
copy build\Release\client.exe exe\client\*.*
copy openssl\bin\*.dll exe\client\*.dll
copy keys\rootca.crt exe\client\*.*

mkdir exe\server
copy build\Release\server.exe exe\server\*.*
copy openssl\bin\*.dll exe\server\*.dll
copy keys\user.crt exe\server\*.*
copy keys\user.key exe\server\*.*
copy keys\dh2048.pem exe\server\*.*

copy keys\*.* exe\*.*

for /f "tokens=2-3 delims=:(" %%i in ('ipconfig /all^|find "IPv4"') do (
  set IP_TEMP=%%i
  set IP_TEMP=%IP_TEMP: =%
  for /f "tokens=1-4 delims=. " %%A in ("%%i") do (
    set ok1=%%A
    set ok2=%%B
    set ok3=%%C
    set ok4=%%D
))

echo client\client.exe -nick=hero -pport=54321 -sport=12345 -sip=%ok1%.%ok2%.%ok3%.%ok4% >> exe\client1.bat
echo client\client.exe -nick=unkn -pport=54322 -sport=12345 -sip=%ok1%.%ok2%.%ok3%.%ok4% >> exe\client2.bat
echo client\client.exe -nick=slap -pport=54323 -sport=12345 -sip=%ok1%.%ok2%.%ok3%.%ok4% >> exe\client3.bat

echo server\server.exe -sport=12345 >> exe\server.bat




