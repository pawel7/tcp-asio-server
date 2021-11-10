## Setup
Budujemy step_0 - za pomocą `bld_step0.sh` ( do folderu bin ), lub
 step_0 i async_tcp_echo_server przez `make` ( do folderu server )

cd ../../bin
step_0
lub step_0 "" 54545 www.boost.org 80
w drugim terminalu telnet localhost 54545
GET / HTTP/1.1


Opis zadania.

Notatki z rozmowy z Pawłem I z 9.11.2021

bazując na asio/src/examples/cpp14/echo/async_tcp_echo_server.cpp
dopisać klienta do serwera, skompilować i uruchomić  dwa procesy.
Potem dołożyć prostą bibiotekę do parsowania HTTP, 
by sparsować to, co wczytano w socket_.async_read_some(asio::buffer(data_, max_length),
czyli data_ i tylko wysłać do klienta "OK" lub "Błąd" .
Mam używać standardów C++11 i nowszych (do C++17, ale nie C++20 ).


Dotyczasowe rozumienie - przed rozmową
Mam napisać program SerwerTcp.


Rozumiem, że docelowo będzie on używany następująco:
W samochodzie będzie podłączona karta Arduino, np. Arduino Mega 2560, z nakładką SIM7000E, 
która będzie zbierała pewne dane podczas jazdy samochodu.
Ten zestaw będzie miał jakiś określony adres IP.
Na procesorze karty będą działały następujące programy:
 a) Jakiś program, który będzie odczytywał dane (np. tempetarurę, położenie z GPS ) i zapisywał je w pamięci karty
 b) program serwera który będzie udostępniał te dane.
 
 Można będzie połączyć się zdalnie z serwerem używając przydzielonego adresu IP i portu

Serwer ten będzie akceptował proste komendy, np.
- podaj dane z określonego przedziału czasowego,
- wyczyść pamięć danych

Na razie zapiszę je jako url, np.

IP serwera/getdata?from=<timestamp>&to=<timestamp>
IP serwera/clear


