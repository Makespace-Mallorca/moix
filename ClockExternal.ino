// ***************************************************************************
// INCLUDES PARA RELOJ EXTERNO
// ***************************************************************************
// https://gist.github.com/deveth0/796afa5d35a6c9d79e30008938d42e4e

#include <Wire.h>
#include <RtcDS3231.h>
  
//************************ HORARIO **************************
const int hourStart=0;

const int hourEnd=23; // Terminará a las 23:59:59

//*********************************************************** 
 
struct myClock
{
  int year, mon, mday, hour, min, sec;
  RtcDateTime rt;
} realTimeClock;

RtcDS3231<TwoWire> rtcObject(Wire);

char dateTimeBuffer[100];

void initializeRealtimeClock()
{
  Serial.println("Real time Clock");

  rtcObject.Begin();

  //rtcObject.SetDateTime(RtcDateTime(2020,7,23,11,05,0)); // SET Date & Time MANUALLY
  //rtcObject.SetDateTime(RtcDateTime(RtcDateTime(__DATE__, __TIME__)); // SET Date & Time = Compiltion Time

  getRealTimeClock();

  printTime();

}

// Coge la fecha y la hora actual y lo guarda en la variable realTimeClock
void getRealTimeClock()
{
  RtcDateTime rt = rtcObject.GetDateTime();
         
  realTimeClock.year = rt.Year();
  realTimeClock.mon = rt.Month();
  realTimeClock.mday = rt.Day();
  realTimeClock.hour = rt.Hour();
  realTimeClock.min = rt.Minute();
  realTimeClock.sec = rt.Second();
  realTimeClock.rt=rt;

}

// CONVERT in STRING  --> PRINT --> 2019.04.15 23:59:59 
void getDateTimeInStringFormat()
{
  snprintf(dateTimeBuffer, sizeof(dateTimeBuffer), "%d.%02d.%02d %02d:%02d:%02d", 
  realTimeClock.year, realTimeClock.mon, realTimeClock.mday, realTimeClock.hour, realTimeClock.min, realTimeClock.sec);
}
void printTime()
{
  getDateTimeInStringFormat();
  Serial.println("Clock:" + (String)dateTimeBuffer);
}

// OPENING HOUR or NOT -->Comprueba si tenemos que entrar en SleepMode
// Este método se llama justo al iniciar la placa y durante el loop
void sleepIfNeeded()
{
  if (!useSleepMode) return; //  

  getRealTimeClock();

  if (realTimeClock.hour >= hourStart && realTimeClock.hour <= hourEnd)
  {
    Serial.println("OPEN --> no hay que hacer nada");
    manualStart=false;
  }
  else 
  {
    sleepAll();
  }
}
//************************************************************************************************************
// SLEEP ALL --> si no hay ruido
//************************************************************************************************************

void sleepAll()
{
    if(manualStart) 
    {
      return;
    }
    
    turnLedsOff();

    Serial.println("GO TO SLEEP");

      // Calculamos los segundos hasta que llegue la hora indicada
    getRealTimeClock();
    printTime();
    
    RtcDateTime startTime(realTimeClock.year,realTimeClock.mon,realTimeClock.mday,hourStart,0,0);

    uint64_t d=startTime.TotalSeconds64()-realTimeClock.rt.TotalSeconds64();

    TimeSpan span(d);

    Serial.println("TimeSpan Day:"+(String)span.days()+". Hours:"+(String)span.hours()+". Minutes:"+(String)span.minutes()+". Secs:" +(String)span.seconds()+". Total:"+(String)span.totalseconds());

    delay(5000);

    uint64_t total=span.totalseconds();

    if(total>STOP_BATTERY_SECONDS) total=STOP_BATTERY_SECONDS;
    
    esp_sleep_enable_timer_wakeup(total*1000000);
    esp_deep_sleep_start();

}

//************************************************************************************************************
// SHORT SLEEP --> si no hay ruido
//************************************************************************************************************
void sleepNow(int totalSeconds)
{
  // if (!useSleepMode) return;

  turnLedsOff();

  sleepByNoise=true;

  esp_sleep_enable_timer_wakeup(totalSeconds*1000000);
  
  esp_deep_sleep_start();
}
