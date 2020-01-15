#include <Wire.h>
#include <DS3231.h>
DS3231 clock;
RTCDateTime dt;

#include <SD.h>
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

int ledPin = 5;
int tempPin = 0;
int chipSelect = 4; //chip select pin for SD
File file; // file object
void setup( )
{
  Serial.begin(9600);
  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);

  pinMode(ledPin, OUTPUT);
  pinMode(chipSelect, OUTPUT); // chip select to OUTPUT

  if (!SD.begin(chipSelect)) { // Initialize SD card
    Serial.println("Could not initialize SD card.");
  }
  if (SD.exists("temp.txt")) { // if "XXXX.txt" exists, file will be deleted
    Serial.println("File exists.");
    if (SD.remove("temp.txt") == true) {
      Serial.println("Successfully removed file.");
    } else {
      Serial.println("Could not removed file.");
    }
  }
}
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }
  return( false );
}
void loop( )
{
  digitalWrite(ledPin, LOW);
  dt = clock.getDateTime();
  float temperature;
  float humidity;
  if( measure_environment( &temperature, &humidity ) == true )
  {
      file = SD.open("temp.txt", FILE_WRITE);
      if (file) {
        file.print("Zeit: ");
        file.print(dt.year);   file.print("-");
        file.print(dt.month);  file.print("-");
        file.print(dt.day);    file.print(" ");
        file.print(dt.hour);   file.print(":");
        file.print(dt.minute); file.print(":");
        file.print(dt.second); file.println("");
        file.print( "Raumtemperatur = " );
        file.print( temperature, 1 );
        file.println( "°C");
        file.print("Luftfeuchtigkeit = ");
        file.print( humidity, 1 );
        file.println( "%" );
        int tempReading = analogRead(tempPin);
        double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
        tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp K
        float tempC = tempK - 273.15;            // K to C
        float tempF = (tempC * 9.0)/ 5.0 + 32.0; // C to F
        file.print("Objekttemperatur = ");
        file.print(tempC, 1);
        file.println( "°C" );
        file.close();
        digitalWrite(ledPin, HIGH);
        delay (500);  // MEASURE!
        digitalWrite(ledPin, LOW);
  } else {
    Serial.println("Could not open file (writing).");
    Serial.print("Zeit: ");
    Serial.print(dt.year);   Serial.print("-");
    Serial.print(dt.month);  Serial.print("-");
    Serial.print(dt.day);    Serial.print(" ");
    Serial.print(dt.hour);   Serial.print(":");
    Serial.print(dt.minute); Serial.print(":");
    Serial.print(dt.second); Serial.println("");
    Serial.print( "Raumtemperatur = " );
    Serial.print( temperature, 1 );
    Serial.println( "°C");
    Serial.print("Luftfeuchtigkeit = ");
    Serial.print( humidity, 1 );
    Serial.println( "%" );
    int tempReading = analogRead(tempPin);
    double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
    tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp K
    float tempC = tempK - 273.15;            // K to C
    float tempF = (tempC * 9.0)/ 5.0 + 32.0; // C to F
    Serial.print( "Objekttemperatur = " );
    Serial.print( tempC, 1 );
    Serial.print( "°C" );
//    Serial.println( "_" );
        digitalWrite(ledPin, HIGH);
        delay (5000); // FAIL!
        digitalWrite(ledPin, LOW);
  }
  //working indicator: ( change to for loop!!)
  delay(10000);
  digitalWrite(ledPin, HIGH);
  delay (50);
  digitalWrite(ledPin, LOW);
  delay(10000);
  digitalWrite(ledPin, HIGH);
  delay (50);
  digitalWrite(ledPin, LOW);
  delay(10000);
  digitalWrite(ledPin, HIGH);
  delay (50);
  digitalWrite(ledPin, LOW);
  delay(10000);
  digitalWrite(ledPin, HIGH);
  delay (50);
  digitalWrite(ledPin, LOW);
  delay(10000);
  digitalWrite(ledPin, HIGH);
  delay (50);
  digitalWrite(ledPin, LOW);
  delay(10000);
  digitalWrite(ledPin, HIGH);
  delay (50);
  digitalWrite(ledPin, LOW);
  }
  }
