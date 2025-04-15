#include <Servo.h>                     // Biblioteka do obsługi serwomechanizmów
#include <Adafruit_NeoPixel.h>        // Biblioteka do obsługi paska LED NeoPixel
#include <DFRobotDFPlayerMini.h>      // Biblioteka do obsługi odtwarzacza DFPlayer Mini
#include <SoftwareSerial.h>           // Biblioteka do komunikacji szeregowej na dowolnych pinach

// === Piny ===
const int pinServo = 9;               // Pin, do którego podłączony jest serwomechanizm (np. "głowa")
const int pinLED = 6;                 // Pin danych paska LED
const int pinPIR = 2;                 // Pin czujnika ruchu PIR
const int pinDFPlayerRX = 0;          // DFPlayer TX → Arduino RX (pin 0)
const int pinDFPlayerTX = 1;          // DFPlayer RX → Arduino TX (pin 1)
const int pinDFPlayerBusy = 4;        // Pin informujący, czy DFPlayer odtwarza dźwięk
const int pinBookSensor = A0;         // Pin analogowy czujnika obecności książek
const int pinLightSensor = A1;        // Pin analogowy czujnika natężenia światła
const int pinDebugLED = 13;           // Wbudowana dioda LED do testowania

// === Moduły ===
Servo headServo;                      // Obiekt serwa
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, pinLED, NEO_GRB + NEO_KHZ800); // Pasek LED z 16 diodami
DFRobotDFPlayerMini dfplayer;         // Obiekt DFPlayera
SoftwareSerial mySerial(pinDFPlayerRX, pinDFPlayerTX); // Komunikacja z DFPlayerem na innych pinach

// === Parametry ===
const unsigned long motionTimeout = 5000;  // Czas bezruchu po którym system się wyłącza (ms)
const int dfFolder = 1;                    // Folder z plikami dźwiękowymi na karcie SD
const int numSounds = 5;                   // Liczba dostępnych dźwięków (plików)

unsigned long lastMotionTime = 0;          // Czas ostatniego wykrycia ruchu
bool systemActive = false;                 // Czy system jest aktywny

// === Inicjalizacja ===
void setup() {
  // Ustawienie trybu pracy pinów
  pinMode(pinPIR, INPUT);
  pinMode(pinDFPlayerBusy, INPUT);
  pinMode(pinBookSensor, INPUT);
  pinMode(pinLightSensor, INPUT);
  pinMode(pinDebugLED, OUTPUT);

  headServo.attach(pinServo);             // Podłączenie serwa do pinu
  headServo.write(0);                     // Ustawienie serwa w pozycji początkowej (0°)

  strip.begin();                          // Inicjalizacja paska LED
  strip.show();                           // Wyłączenie wszystkich diod

  mySerial.begin(9600);                   // Start komunikacji z DFPlayerem
  dfplayer.begin(mySerial);              // Inicjalizacja DFPlayera
  dfplayer.volume(25);                   // Ustawienie głośności (0–30)

  randomSeed(analogRead(A1));            // Inicjalizacja generatora losowego, jako źródło użyto czujnika światła

  digitalWrite(pinDebugLED, LOW);        // Zgaszenie diody debugującej
}

// === Główna pętla programu ===
void loop() {
  // Odczyt stanu czujników
  bool motionDetected = digitalRead(pinPIR);                         // Czy wykryto ruch
  bool booksPresent = analogRead(pinBookSensor) > 100;              // Czy są książki (czujnik aktywny powyżej progu)
  bool isDaylight = analogRead(pinLightSensor) > 300;              // Czy jest jasno (światło dzienne)

  // Jeśli wykryto ruch i są książki – aktywuj system
  if (motionDetected && booksPresent) {
    lastMotionTime = millis();              // Zapisz czas ostatniego ruchu
    if (!systemActive) {
      activateSystem(isDaylight);          // Jeśli system był nieaktywny – uruchom go
    }
  }

  // Jeśli system jest aktywny, ale nie było ruchu przez 5s – wyłącz
  if (systemActive && (millis() - lastMotionTime > motionTimeout)) {
    deactivateSystem();
  }
}

// === Aktywacja systemu (po wykryciu ruchu i obecności książek) ===
void activateSystem(bool isDaylight) {
  systemActive = true;                     // Oznacz system jako aktywny

  if (!isDaylight) {                       // Jeśli jest ciemno – włącz LEDy
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 80, 0)); // Ciepły pomarańczowy kolor
    }
    strip.show();                          // Wyświetl kolory na pasku LED
  }

  headServo.write(90);                     // Obróć serwo (np. „głowa” do przodu)
  delay(500);                              // Poczekaj na pełen obrót

  playRandomSound();                       // Odtwórz losowy dźwięk z DFPlayera

  delay(3000);                             // Poczekaj na zakończenie odtwarzania (lub minimum 3 sekundy)

  headServo.write(0);                      // Cofnij serwo do pozycji początkowej
  delay(500);
}

// === Dezaktywacja systemu (po czasie bez ruchu) ===
void deactivateSystem() {
  systemActive = false;                    // Oznacz system jako nieaktywny

  // Zgaś wszystkie LEDy
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();

  headServo.write(0);                      // Cofnij serwo na start
}

// === Funkcja do odtwarzania losowego dźwięku z DFPlayera ===
void playRandomSound() {
  int trackNumber = random(1, numSounds + 1); // Wybierz losowy numer od 1 do numSounds
  dfplayer.playFolder(dfFolder, trackNumber); // Odtwórz plik z podfolderu na karcie SD
}
