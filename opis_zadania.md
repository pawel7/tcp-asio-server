## Aktualny opis zadania

Twoim zadaniem jest napisanie serwera TCP, który otwiera połączenia z klientem pod adresem localhost:5005.
Serwer ma za zadanie odbierać wiadomości od klienta.
Wiadomość ma być wysłana w formacie HTTP (np. HTTP GET temp lub POST). Jeżeli wiadomość będzie poprawnym komunikatem HTTP, to serwer odpowiada OK w formacie HTTP, jeżeli nie to wysyła błąd (400 bad request). Jeźeli zapytanie jest poprawne to obsĺugujemy tylko GET I POST. 
GET pobiera wartość temperatury z listy, a POST zapisuje temperaturę pod odpowiednie miejsce w liście.
Należy to przetestować pisząc program klienta który po połączeniu z serwerem (localhost) oraz po wybraniu opcji (1,2,3,4,5) wyślę przygotowane zapytanie do serwera:
1 - get temp1
2 - get temp2
3- post temp1
4- post temp2
5- hello (bez HTTP)

Do napisania klient i serwer (docelowo TCP) nie muszą być asychronicznie.
Pierwsza wersja:
* klient pyta się o temperaturę, wysyłając polecenie typu GET temp
* serwer wysyła po jednej kolejne liczby, które ma zapisane w tablicy


  ## Setup
Budujemy serwera i klienta - za pomocą `Bld_proj.sh`

```
./Bld_proj.sh
./server & ./client

```


### Notatki z rozmowy z Pawłem I z 9.11.2021

bazując na asio/src/examples/cpp14/echo/async_tcp_echo_server.cpp
dopisać klienta do serwera, skompilować i uruchomić  dwa procesy.
Potem dołożyć prostą bibiotekę do parsowania HTTP, 
by sparsować to, co wczytano w socket_.async_read_some(asio::buffer(data_, max_length),
czyli data_ i tylko wysłać do klienta "OK" lub "Błąd" .
Mam używać standardów C++11 i nowszych (do C++17, ale nie C++20 ).


Dotyczasowe rozumienie - przed rozmową<br>
Mam napisać program SerwerTcp.<br>


Rozumiem, że docelowo będzie on używany następująco:<br>
W samochodzie będzie podłączona karta Arduino, np. Arduino Mega 2560, z nakładką SIM7000E, 
która będzie zbierała pewne dane podczas jazdy samochodu.<br>
Ten zestaw będzie miał jakiś określony adres IP.<br>
Na procesorze karty będą działały następujące programy:<br>
 a) Jakiś program, który będzie odczytywał dane (np. tempetarurę, położenie z GPS ) i zapisywał je w pamięci karty<br>
 b) program serwera który będzie udostępniał te dane.<br>
 
 Można będzie połączyć się zdalnie z serwerem używając przydzielonego adresu IP i portu<br>

Serwer ten będzie akceptował proste komendy, np.<br>
* podaj dane z określonego przedziału czasowego,
* wyczyść pamięć danych

Na razie zapiszę je jako url, np.<br>
```
IP serwera/getdata?from=<timestamp>&to=<timestamp>
IP serwera/clear
```

