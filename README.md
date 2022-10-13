# OSRT Semestrálna práca 2021
 Semestrálna práca z predmetu Operačné Systémy Reálneho Času

**1** O programe
---
&emsp;Mojím zvoleným zadaním bolo vytvoriť test osobnosti. Zvolila som si pre to
[tzv. The Big Five test zo stránky Open Psychometric](https://openpsychometrics.org/tests/IPIP-BFFM/ "Open-Source Psychometrics Project"). Tento test užívateľovi predkladá otázky, na ktoré je možné odpovedať piatimi možnosťami (viď. legenda). Test takto hodnotí päť ľudských vlastností: Extraversion, Agreeableness,
Conscientiousness, Emotional Stability, or Intellect/Imagination.  
&emsp;Projekt tvoria dva programy - server a klient. Server dokáže komunikovať so
štyrmi klientmi nezávisle naraz. Klienti sú zhodní.  
&emsp;Server má zoznam otázok a adekvátny hárok hodnotení, ktorý diktuje ako
by mali byť odpovede na konkrétne otázky hodnotené. Server tieto otázky a
potrebné inštrukcie na hodnotenie posiela klientom.  
&emsp;Po spustení klienta a jeho úspešnom pripojení na server sa začnú zobrazovať
otázky. Na tie je nutné odpovedať zadaním čísla v rozsahu 1-5, viď. legenda. Po
zodpovedaní všetkých otázok klient vypíše správu, že všetky otázky boli zodpovedané spolu s výsledkom testu.

\* | Legenda
--- | :---:
1 | Very Inaccurate
2 | Moderately Inaccurate
3 | Neither Accurate Nor Inaccurate
4 | Moderately Accurate
5 | Very Accurate

**2** Kompilácia a spustenie
---
&emsp;Celý projekt (server a klient) je možné skompilovať naraz len príkazom `make`
Server požaduje dva argumenty, jedným je port a druhým počet otázok (min.
1, max. 50), ktoré má klientovi odoslať:  
```
./server <port> <počet_otázok>
```
&emsp;Klient požaduje len jeden argument a tým je port, ktorý musí byť rovnaký ako
port serveru:
```
./client <port>
```
&emsp;V projekte sa tiež nachádza ďalší program, ktorý slúži len ako prototyp.
skompiluje sa cez `make test` a spustí cez `./test`

**3** Vypnutie programu
---
&emsp;Klient sa ukončí automaticky po zodpovedaní všetkých otázok a vypísaní
výsledkov.  
&emsp;Server sa ukončí automaticky po obslúžení všetkých štyroch klientov. V prípade,
ak obsluha nejakého z klientov bola neúspešná a server ostane spustený, ukončí
sa automaticky po piatich minútach od pripojenia štvrtého klienta.
