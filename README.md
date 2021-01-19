# Generator-funkcyjny

Projekt wykonany na płytce FRDM-KL05.
Generator działa w oparciu o przetwornik C/A, generuje przebiegi: sinusoidalny, falę trójkątną i prostokątną. Obsługiwany za pomocą dwóch przycisków (zmiana przebiegu, zmina trybu nastawiania) oraz pola dotykowego płytki KL05 (regulacja napięcia miedzyszczytowego lub częstotliwości w zależności od trybu nastawiania).

# 1.	Podłączenie
  a.	Klawiatura:
  
    C3 – PTB6
    C4 – PTB7
    R4 – GND 
    
  b.	Wyświetlacz:
  
    SCL – PTB3 
    SDA – PTB4
    VCC – 5V
    GND – GND
  c.	Głośnik wpinany jest pomiędzy PTB1 a GND

# 2.	Instrukcja obsługi
  Przycisk S1 powoduje cykliczną zmianę kształtu przebiegu (prostokątny -> sinus -> trójkątny).
  
  Przycisk S2 przełącza między trybem ustawiania częstotliwości a trybem ustawiania napięcia międzyszczytowego wybranego przebiegu.
  
  Pole dotykowe pozwala na regulację wybranego parametru
  
  Wyświetlacz pokazuje kształt przebiegu (sqr, sin, tri) oraz obecnie ustawiany parametr wraz z jego wartością.
 
# 3.	Parametry

  Regulacja częstotliwości w zakresie: 20Hz – 14460Hz
  
  Regulacja napięcia międzyszczytowego: 0.06V – 2.72V
 
# 4.	Zasada działania
Dla tablicy o ilości elementów n, PIT0 powinien generować przerwania z częstotliwością n razy większą od pożądanej częstotliwości sygnału wyjściowego (zwiększenie liczby       próbek spowoduje wygładzenie przebiegu na wyjściu, lecz ograniczy jego maksymalną częstotliwość). Przerwanie od PIT0 powoduje wysłanie kolejnej próbki z wybranej tablicy do przetwornika C/A i ustalenie odpowiedniej wartości na wyjściu (PTB1). Poprzez przesuwanie palcem po polu dotykowym można wpłynąć na częstotliwość generowania przerwań (zmiana wartości rejestru LDVAL) oraz na wartość napięcia na wyjściu (przemnażanie wartości próbki). 

# 5.	Generowanie tablic
Tablice przebiegów powstają poprzez pobranie równo oddalonych od siebie próbek z jednego okresu wybranego przebiegu. Żeby uzyskać jak najwyższą częstotliwość maksymalną przy zachowaniu kształtu przebiegu zdecydowałem się na tablice zawierające 20 próbek. 

Aby wygenerować tablice dla przebiegu sinusoidalnego i trójkątnego użyłem skryptu w języku Python.  

Generator powinien bez większych problemów działać dla tablic innych prostych przebiegów przy zachowaniu długości 20 oraz wartości próbek z zakresu 0x00 – 0x28 (0-40).

# 6.	Prezentacja działania
Film z prezentacją znajduje się pod adresem: https://youtu.be/td5GKXfnqgk
Podczas prezentacji zdecydowałem się na użycie oscyloskopu, ponieważ za jego pomocą dokładniej można zaprezentować działanie projektu.


# Autor
Michał Tomacha
