# Strażnik książek – Mechatroniczny Trzymacz Książek

Projekt realizowany w ramach kursu **Systemy elektroniczne w mechatronice** na Politechnice Wrocławskiej.

## 📚 Opis projektu

**Strażnik książek** to inteligentny trzymacz książek w formie figurki Iron-Mana, który reaguje na obecność użytkownika oraz warunki otoczenia. Urządzenie integruje różne technologie mechatroniczne, takie jak:

- Wykrywanie ruchu (czujnik PIR)
- Detekcja obecności książek (fotorezystor)
- Pomiar światła dziennego (fotorezystor)
- Podświetlenie LED (WS2812)
- Ruchoma głowa figurki (serwo MG90S)
- Efekty dźwiękowe (DFPlayer Mini + głośnik)

## ⚙️ Wykorzystane komponenty

- **Mikrokontroler**: Arduino Nano V3.0  
- **Czujniki**: PIR HC-SR505, 2x fotorezystor GL5616  
- **Wykonawcze**: serwomechanizm MG90S, listwa LED WS2812 (2x po 8 diod)  
- **Dźwięk**: DFPlayer Mini + głośnik 1W 8Ω  
- **Zasilanie**: 5V/2A (gniazdo DC barrel)  

## 💡 Funkcjonalność

1. Detekcja ruchu użytkownika aktywuje system.
2. Sprawdzana jest obecność książek oraz poziom światła dziennego.
3. Jeśli książki są obecne i jest ciemno:
   - Włączane jest podświetlenie LED.
   - Głowa figurki porusza się w stronę użytkownika.
   - Odtwarzany jest losowy dźwięk Iron-Mana (MP3 z karty SD).

System automatycznie się dezaktywuje przy braku ruchu.

## 🧠 Kod i biblioteki

Kod Arduino wykorzystuje następujące biblioteki:

- `Servo.h` – sterowanie serwem  
- `Adafruit_NeoPixel.h` – sterowanie LED WS2812  
- `SoftwareSerial.h` – komunikacja z DFPlayer Mini  
- `DFRobotDFPlayerMini.h` – obsługa odtwarzacza MP3

## 📦 Zawartość repozytorium

- `kod.ino` – kod źródłowy dla Arduino
- `schematy/` – schematy elektroniczne (.sch, .brd)
- `3d/` – modele 3D (.stl, .step)
- `bom.csv` – lista komponentów (Bill of Materials)
- `dokumentacja.pdf` – pełna dokumentacja projektowa
- `rysunki_złożeniowe.pdf` – wizualizacje montażu

## 👤 Autor

**Dominik Bukowicz**    
