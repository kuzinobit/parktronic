#include <LiquidCrystal.h>  // Подключаем стандартную библиотеку для LCD-дисплея

// Инициализация LCD-дисплея (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Пины для HC-SR04
const int trigPin = 3;
const int echoPin = 2;

// Пины для RGB светодиода
const int redPin = 4;
const int greenPin = 5;
const int bluePin = 6;

// Пин для пьезо-пищалки
const int buzzerPin = 13;

// Переменные для хранения времени и расстояния
long duration;
int distance;

void setup() {
  // Настройка пинов для HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Настройка пинов для RGB светодиода
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Настройка пина для пьезо-пищалки
  pinMode(buzzerPin, OUTPUT);

  // Инициализация LCD-дисплея 16x2
  lcd.begin(16, 2);
  lcd.print("Dist:");  // Печатаем начальный текст
}

void loop() {
  // Генерация ультразвукового импульса
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Чтение импульса Echo и расчет расстояния
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;  // Расчет расстояния в сантиметрах

  // Отображение расстояния на LCD-дисплее
  lcd.setCursor(6, 0);    // Устанавливаем курсор после "Dist:"
  lcd.print("    ");      // Очищаем предыдущие цифры
  lcd.setCursor(6, 0);    // Снова устанавливаем курсор
  lcd.print(distance);    // Печатаем новое значение

  // Логика изменения цвета RGB светодиода и звуковых сигналов
  if (distance > 100) {
    // Если препятствий нет (дальше 100 см) — синий цвет, звук выключен
    setColor(255, 0, 0);  // Синий
    noTone(buzzerPin);    // Отключаем звук
  } else if (distance > 50 && distance <= 100) {
    // Препятствие далеко (50–100 см) — зеленый цвет, один короткий сигнал
    setColor(0, 255, 0);  // Зеленый
    playBeep(1);
  } else if (distance > 30 && distance <= 50) {
    // Препятствие приближается (30–50 см) — желтый цвет, два коротких сигнала
    setColor(0, 200, 200);  // Желтый
    playBeep(2);
  } else if (distance > 20 && distance <= 30) {
    // Препятствие близко (20–30 см) — оранжевый цвет, три коротких сигнала
    setColor(0, 100, 255);  // Оранжевый
    playBeep(3);
  } else if (distance <= 15) {
    // Препятствие очень близко (меньше 15 см) — красный цвет, постоянный звук
    setColor(0, 0, 255);  // Красный
    tone(buzzerPin, 1000);  // Постоянный звук
  }

  delay(500);  // Задержка перед следующим измерением
}

// Функция установки цвета на RGB светодиоде
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

// Функция для проигрывания звуковых сигналов
void playBeep(int count) {
  for (int i = 0; i < count; i++) {
    tone(buzzerPin, 1000);   // Включаем звук с частотой 1000 Гц
    delay(100);              // Задержка для короткого сигнала
    noTone(buzzerPin);       // Отключаем звук
    delay(100);              // Задержка между сигналами
  }
}
