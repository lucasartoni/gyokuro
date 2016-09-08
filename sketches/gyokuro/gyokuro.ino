#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define minTemp     25  // Min temperature: 20 Celsius Degrees
#define maxTemp     95  // Max temperature: 95 Celsius Degrees
#define hysteresys  1   // Hysteresys: 1 Celsius Degree
#define oneWirePin  2   // OneWirePin pin for termometer
#define selectorPin A0  // Temp selector pin
#define relayPin    8  // Relay pin

byte heart[8] = { 
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
byte arrowUp[8] = { 
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};
byte arrowDown[8] = { 
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100
};


float waterTemperature = 0;   // Water Temperature
int selectorValue = 0;        // Selector Value

LiquidCrystal lcd( 4 , 6 , 10 , 11 , 12 , 13 );
OneWire oneWire( oneWirePin );
DallasTemperature sensors( &oneWire );

void setup()
{
  pinMode( relayPin , OUTPUT );  

  // selectorValue = 33 ;
  
  // 16 x 2 setup on LCD
  lcd.begin( 16 , 2 );
  lcd.createChar( 0 , heart);
  lcd.createChar( 1 , arrowUp);
  lcd.createChar( 2 , arrowDown);

  // starts the sensors
  sensors.begin();

  // starts the serial port for debugging
 // Serial.begin( 9600 );
}

void loop() 
{

  sensors.requestTemperatures();
  waterTemperature = sensors.getTempCByIndex( 0 );
  selectorValue = getSelectorValue( selectorPin , minTemp , maxTemp );

  // Water and Selector info on screen
  lcd.setCursor( 5 , 0 );
  lcd.print( ( int ) waterTemperature );
  lcd.print( ":" );
  lcd.print( selectorValue );

  if ( waterTemperature > ( selectorValue + hysteresys ) ){
    setRelay( relayPin , LOW ); 
    lcd.setCursor( 0 , 1 );
    lcd.write( byte ( 2 ) );
    lcd.print( " Cooling down " ); // spaces to better fit the LCD row
    lcd.write( byte ( 2 ) );
  } else if ( waterTemperature < ( selectorValue - hysteresys ) ){
    setRelay( relayPin , HIGH );  
    lcd.setCursor( 0 , 1 );
    lcd.write( byte ( 1 ) );
    lcd.print( "  Heating up  " ); // spaces to better fit the LCD row
    lcd.write( byte ( 1 ) );
  } else {
    lcd.setCursor( 0 , 1 );
    lcd.write( byte ( 0 ) );
    lcd.print( "    Enjoy!    " ); // spaces to better fit the LCD row
    lcd.write( byte ( 0 ) );
  }
    
}




