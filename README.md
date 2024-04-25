# Chat

На тестовое потрачено ~1.5-2ч.
В данном решении были опущено много вещей, так как это тестовое.
Вот главные из них:
1) Нет проверки на занятость ввода/вывода консоли у клиента.
2) Нет обертки над upd сокетами которые используются в p2p. На одной машине все будет хорошо, но как только это выйдет в некую сеть, могут теряться пакеты, приходить не в том порядке и т.п.
3) Нет нормального вывода сообщений
4) В некоторых местах дублировался код, для скорости разработки
5) Сделан из тестового только клиент-сервер, p2p, обработка дисконнектов

Для запуска тестового требуется:
1) git clone нужного репозитория
2) иметь boost и openssl (в windows лежит в репозитории, в linux требуется установить)
3) запустить build.bat(build.sh)
4) перейти в папку exe
5) Запустить сервер и требуемое количество клиентов (client1, client2, client3)

возможно, что на windows не правильно укажется -sip, надо будет изменить батники вручную))

Для сервера и клиента требуются некоторые аргументы:
Сервер: -sport=12345
Клиент: -sip=10.10.10.10 -sport=12345 -pport=54321 -nick=hero

-sip = ip-адресс сервера
-sport = порт сервера
-pport = порт для Udp порта
-nick = никнейм клиента