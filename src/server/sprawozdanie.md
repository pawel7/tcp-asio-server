Napisałem serwer i klienta wg specyfikacji.

Kompilacja:
```
 ./Bld_proj.sh
```
Uruchomienie:
```
 ./server & ./client
 ```

Korzystamy z menu - klawisze 1..6 i enter

 Przed ponownym uruchomieniem trzeba zobaczyć i ubić procesy na porcie 5005
 
 ```
 sudo lsof -t -i:5005
 kill -9 <nr procesu>
```

Przykładowe wyniki są w plikach *.log

