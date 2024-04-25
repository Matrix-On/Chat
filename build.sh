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
mkdir -p exe/client1
mkdir -p exe/server1

# Копируем исполняемые файлы и библиотеки для клиента
cp build/client exe/client1/
cp keys/rootca.crt exe/client1/

# Копируем исполняемые файлы и библиотеки для сервера
cp build/server exe/server1/
cp keys/user.crt exe/server1/
cp keys/user.key exe/server1/
cp keys/dh2048.pem exe/server1/

# Копируем остальные файлы из папки keys
cp keys/* exe/

# Получаем IP-адрес
IP_TEMP=$(ip addr show | grep "inet " | grep -v "127.0.0.1" | awk '{print $2}' | cut -d "/" -f 1)

# Генерируем скрипты для запуска клиентов
echo "client1/client -nick=hero -pport=54321 -sport=12345 -sip=$IP_TEMP" > exe/client1.sh
echo "client1/client -nick=unkn -pport=54322 -sport=12345 -sip=$IP_TEMP" > exe/client2.sh
echo "client1/client -nick=slap -pport=54323 -sport=12345 -sip=$IP_TEMP" > exe/client3.sh

# Генерируем скрипт для запуска сервера
echo "server1/server -sport=12345" > exe/server.sh

# Даем права на выполнение скриптам
chmod +x exe/*.sh
chmod +x exe/client1/client
chmod +x exe/server1/server
