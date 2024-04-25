#!/bin/bash

# Удаляем директории build и exe, если они существуют
if [ -d "build" ]; then
    rm -rf build
fi

if [ -d "exe" ]; then
    rm -rf exe
fi

# Создаем директорию build и переходим в нее
mkdir build
cd build

# Генерируем проект с помощью cmake
cmake ..

# Собираем проект
cmake --build . --config Release

# Возвращаемся в исходную директорию
cd ..

# Создаем директорию exe
mkdir -p exe/client
mkdir -p exe/server

# Копируем исполняемые файлы и библиотеки для клиента
cp build/Release/client.exe exe/client/
cp openssl/bin/*.dll exe/client/
cp keys/rootca.crt exe/client/

# Копируем исполняемые файлы и библиотеки для сервера
cp build/Release/server.exe exe/server/
cp openssl/bin/*.dll exe/server/
cp keys/user.crt exe/server/
cp keys/user.key exe/server/
cp keys/dh2048.pem exe/server/

# Копируем остальные файлы из папки keys
cp keys/* exe/

# Получаем IP-адрес
IP_TEMP=$(ip addr show | grep "inet " | grep -v "127.0.0.1" | awk '{print $2}' | cut -d "/" -f 1)

# Генерируем скрипты для запуска клиентов
echo "exe/client/client.exe -nick=hero -pport=54321 -sport=12345 -sip=$IP_TEMP" > exe/client1.sh
echo "exe/client/client.exe -nick=unkn -pport=54322 -sport=12345 -sip=$IP_TEMP" > exe/client2.sh
echo "exe/client/client.exe -nick=slap -pport=54323 -sport=12345 -sip=$IP_TEMP" > exe/client3.sh

# Генерируем скрипт для запуска сервера
echo "exe/server/server.exe -sport=12345" > exe/server.sh

# Даем права на выполнение скриптам
chmod +x exe/*.sh
chmod +x exe/client/*.exe
chmod +x exe/server/*.exe
