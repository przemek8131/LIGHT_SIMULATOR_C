Do realizacji symulacji inteligentnych świateł drogowych wykorzystany został język C, w którym napisana została główna część programu.
Natomiast do obsługi plików JSON stanowiących dane wejściowe i wyjściowe programu, wykorzystany został skrypt w języku python "symulator.py". 
Symulację można uruchomić po wcześniejszej kompilacji programu w c i umieszczeniu pliku wykonywalnego w tym samym katalogu co skrypt. 
Plik .exe uruchamiany jest jako subproces w skrypcie. Możliwe jest uruchomienie symulacji zarówno podając pliki wej/wyj np. python symulator.py input.json output.json.
Bez podania plików wej/wyj, wykorzystane do symulacji zostaną wówczas dane z domyślnych plików input/output.json. 
Algorytm opiera swoje działanie na priorytetyzowaniu kierunku z którego odjechać chce najwięcej aut. 
Oprócz tego, nadrzędny jest czas oczekiwania (liczba kroków symulacji) pojazdów na danym kierunku. Jeżeli przekroczy on zadaną wartość, wybierany jest kierunek, który oczekiwał najdłużej. 
Przy czym zarówno dla tej samej liczby pojazdów dla kilku kierunków, jak również dla tego samego czasu oczekiwania wybierany jest pierwszy znaleziony kierunek.

Po wybraniu kierunku od którego rozpoczyna się ustalanie świateł na skrzyżowaniu, możliwe są trzy scenariusze w zależności od tego w którą stronę jedzie pojazd. 
Na skrzyżowaniu dostępne są 4 główne światła dla 4 kierunków oraz 4 światła warunkowe pozwalające na wykonanie skrętu w prawo. 
Z założenia główne zielone światło może zapalić się jedynie na kierunkach przeciwległych np. nie może być zielonego światła na kierunku północnym i zachodnim, 
bo mogłoby to prowadzić do kolizji. Natomiast po zapaleniu światła zielonego na przeciwległych kierunkach możliwe jest zapalenie zielonego światła warunkowego na drogach po bokach.
W przypadku gdy samochód ma zamiar wykonania lewoskrętu, możliwe jest zielone światło tylko dla tego kierunku oraz warunkowe światło w prawo dla kierunku po lewej stronie (docelowym), 
gdyż nie spowoduje to kolizji.
W przypadku gdy samochód ma zamiar jechać prosto, możliwe jest zielone światło dla początkowego kierunku oraz przeciwległego, o ile samochód z naprzeciwka nie chce wykonać lewoskrętu. 
Dodatkowo jeżeli samochód z naprzeciwka skręca w prawo, możliwe jest zielone światło warunkowe na drodze po lewej stronie (od początkowego kierunku).
W przypadku gdy samochód ma zamiar jechać w prawo, ponownie możliwe jest zielone światło dla początkowego kierunku i przeciwległego, o ile samochód z naprzeciwka nie chce wykonać lewoskrętu,
jeżeli chce wykonać lewoskręt to zielone światło zapalane jest tylko na kierunku początkowym. Zielone światło warunkowe z prawej strony (docelowej) jest bezkolizyjne. 
Dodatkowo jeżeli samochód z naprzeciwka skręca w prawo, na kierunku po lewej stronie również zapali się zielone światło warunkowe.
Jeżeli na skrzyżowaniu nie ma żadnego pojazdu, to zielone światło ustalane jest jedynie na kierunku północnym.
Z założenia przy każdym kroku symulacji może przejechać jeden samochód z danego kierunku oraz za każdym razem następuje zmiana koloru świateł i ustalany jest następny stan sygnalizatorów.
