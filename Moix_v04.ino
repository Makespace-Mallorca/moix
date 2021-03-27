// 10/09/2020 19:13
//    Hacemos una prueba para comprobar lo que aguanta la batería con los LEDS encendidos
//    Para ello ponemos RANGE_STOP de 20 a -20
//    useSleepMode de true a false
// VERSION 23/07/2020 12:50
// Se modifica para que cada 15" se despierte durante 2"
// Se utiliza el OnboardLED para indicar si está o no escuchando sonido

// Nota: Funciona: RELOJ / SLEEP / SLEEPSHORT / MICRO / BOTON --> WAKEUP / SLEEP

// WakeUp ext0 --> Pin D32 con pulsador y resistencia
//https://lastminuteengineers.com/esp32-deep-sleep-wakeup-sources/
//https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/

RTC_DATA_ATTR bool manualStart=false;
// Indica que está en modo sleep por qué no está escuchando nada
RTC_DATA_ATTR bool sleepByNoise=false;
// ***************************************************************************
// VARIABLES DE CONFIGURACION
// ***************************************************************************
#define TOSECONDS(a) ((a)*1000)
#define TOMINUTES(a) (TOSECONDS(a)*60)
#define TOHOURS(a) (TOMINUTES(a)*60)

#define MK1000              // Indica que el procesador es un MK10000 y por tanto las funciones de sleep son diferentes

#define NUM_LEDS 6
#define LED_STRENGTH 255    // Valor entre 1 y 255 que indica la fuerza de iluminación de los LED

// Rangos de comportamiento
#define STOP_BATTERY_SECONDS (30) // Nº de segundos que esta en modo deep sleep para que la bateria no se apague.
#define STOP_SECONDS (16)  // Nº de segundos que estamos parados por que no hay sonido y nos permite ahorrar bateria. Debe ser multiplo de 8
#define RANGE_STOP -20    // Si es menor que este valor, paramos durante X segundos
#define RANGE_QUIET 65   // Verde, si es menor que este valor y mayor que STOP (antes 100)
#define RANGE_MODERATE 180  // Azul si está entre QUIET y MODERATE (antes 160)
// Rojo si supera el MODERATE

bool useSleepMode = false;

extern char dateTimeBuffer[];
extern void initializeRealtimeClock();
/*********************************************************
   Configuracion de pin

   RELOJ
   D21   --> SDA   --> ORANGE
   D22   --> SCL   --> BLUE
   D33   --> SQW   --> GREEN  (INTERRUPT SQuare Wave pin
   MICROFONO
   D2   --> GATE ESP32
   A0   --> ENVELOPE --> VP (UP?) --> ESP32

   LED
   D13  --> GPIO 13

   WakeUp ext0 --> Pin D35 con pulsador y resistencia
   D35
*/

#define ULONG unsigned long

// Lee valores continuamente y reacciona al momento
#define MODE_CONTINOUS 0

// Hace la media entre el tiempo estipulado y reaccion al final del tiempo
#define MODE_AVERAGE 1

int currentMode = MODE_AVERAGE;

// PINES Para el MICROFONO. SOLO UTILIZAMOS EL A0, LOS DEMAS SON DEL CODIGO DE EJEMPLO DEL MICROFONO
#define PIN_GATE_IN 2
#define IRQ_GATE_IN  0
#define PIN_LED_OUT 13
#define PIN_ANALOG_IN A0

// Si supero este valor, me indica que tengo que comprobar si hay sonido en el cache
ULONG timeToCheckStop;

// Configurar estos dos valores de lectura para el modo configuración y MODE_AVERAGE
#define SAMPLE_DELAY_MILISECONDS 100
#define SAMPLE_TIME_SECONDS 10
#define SAMPLES_PER_SECOND (1000/SAMPLE_DELAY_MILISECONDS)

#define MAX_DATA (SAMPLE_TIME_SECONDS*SAMPLES_PER_SECOND)

short data[MAX_DATA];
int indexData;

int averageSound;
bool irqWakeup=false;

//********************** WAKE UP REASON *********************
//Function that prints the reason by which ESP32 has been awaken from sleep
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  manualStart=false;
  switch (wakeup_reason)
  {
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : 
            if(!irqWakeup) manualStart=true;
            Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("Wakeup caused by timer"); break;
    case 4  : Serial.println("Wakeup caused by touchpad"); break;
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}

const uint8_t buttonPIN = 35;

//********************** isr BOTON SLEEP *********************
void IRAM_ATTR irqLlamadaAlPulsarSoloCuandoEstaDespierto() 
{
  Serial.println("irqLlamadaAlPulsarSoloCuandoEstaDespierto");
  Serial.println("Go to sleep now");
  Serial.println("");
  //delay(1000);
  irqWakeup=true;
  manualStart=false;

}

// ************************************************************************************************************
// ************************************************************************************************************
// Inicialización de la placa
// ************************************************************************************************************
// ************************************************************************************************************
void setup()
{
  Serial.begin(9600);

  delay(3000);
  Serial.println("   ");
  Serial.println("****************************************");

  print_wakeup_reason();

  Serial.println("ManualStart:"+(String)manualStart);

  // Activar BOTON SLEEP
  pinMode(buttonPIN, INPUT_PULLUP);
  attachInterrupt(buttonPIN, irqLlamadaAlPulsarSoloCuandoEstaDespierto, FALLING);

  // Activar EXTERNAL WAKEUP
  //Configure GPIO33 as ext0 wake up source for HIGH logic level
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 1);

  //***************************************************************

  for (int i = 0; i < 5; i++)
    Serial.println();

  Serial.println("Initialitating...");

  setupBoardLED();

  initializeRealtimeClock();

  initilizeRGBMatrix();

  setRGBColor(0, 0, 0);

  indexData = 0;

  // if analog input pin 0 is unconnected, random analog noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs. randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));

  // Display status
  Serial.println("Initialized");

  indexData = 0;

  sleepByNoise=false;

  sleepIfNeeded();

}

//*************** Fin SET UP ************************

// Tiempo entre cada loop
void samplerDelay()
{
  delay(SAMPLE_DELAY_MILISECONDS);
}

//******************* LOOP **************************
void loop()
{
  // Recuperamos el tiempo en milisegundos
  ULONG time = millis();

  getRealTimeClock();

  // Check the envelope input
  int value = analogRead(PIN_ANALOG_IN);

  switch (currentMode)
  {
    case MODE_AVERAGE:
      averageMode(value);
      break;
    case MODE_CONTINOUS:
      continousMode(value);
      break;
  }

  getDateTimeInStringFormat();

  Serial.print(dateTimeBuffer);

  Serial.print(" Status: ");
  Serial.println(value);

  samplerDelay();

  sleepIfNeeded();
}
//******************** Fin LOOP *******************


void averageMode(int value)
{
  if(value<RANGE_STOP)
    boardLED_OFF();
  else
    boardLED_ON();
    
  if (storeData(value))
  {
    //value=computeNoiseValueLog();

    value = computeNoiseValueAverage();

    indexData = 0;

    if (value < RANGE_STOP)
    {
      // No se escucha nada, así que esperaremos un tiempo con los leds apagados
      Serial.println("STOP.");

      sleepNow(STOP_SECONDS);

      Serial.println("CONTINUE.");
    }
    else
    {
      changeColorRanged(value);
    }
  }
}

void continousMode(int value)
{
  changeColorRanged(value);
}

void changeColorRanged(int value)
{
  if (value <= RANGE_QUIET)
  {
    Serial.println("Quiet.");

    setRGBColor(0, 1, 0);
  }
  else if ( (value > RANGE_QUIET) && ( value <= RANGE_MODERATE) )
  {
    Serial.println("Moderate.");

    setRGBColor(0, 0, 1);
  }
  else
  {
    Serial.println("Loud.");

    setRGBColor(1, 0, 0);
  }
}
