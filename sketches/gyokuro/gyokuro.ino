#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>


// Parameters 
#define minTemp     25  // Min temperature: 20 Celsius Degrees
#define maxTemp     95  // Max temperature: 95 Celsius Degrees
#define hysteresys  1   // Hysteresys: 1 Celsius Degree
#define oneWirePin  2   // OneWirePin pin for termometer
#define selectorPin A0  // Temp selector pin
#define relayPin    8   // Relay pin

// create a custom heart character
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

// create a custom arrow up character
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

// create a custom arrow down character
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


// Variables
LiquidCrystal lcd( 4 , 6 , 10 , 11 , 12 , 13 );
OneWire oneWire( oneWirePin );
DallasTemperature sensors( &oneWire );
float waterTemperature = 0;
int selectorValue = 0;

void setup()
{
  // set the relayPin for output
  pinMode( relayPin , OUTPUT );  
  
  // set the LCD screen
  lcd.begin( 16 , 2 );
  
  // initialize the custom characters 
  lcd.createChar( 0 , heart );
  lcd.createChar( 1 , arrowUp );
  lcd.createChar( 2 , arrowDown );

  // initialize the temperature sensor
  sensors.begin();
}

void loop() 
{

  // measure the water temperature
  sensors.requestTemperatures();
  waterTemperature = sensors.getTempCByIndex( 0 );

  // retrieve the seletor value
  selectorValue = getSelectorValue( selectorPin , minTemp , maxTemp );

  // print water and Selector info on screen
  lcd.setCursor( 5 , 0 );
  lcd.print( ( int ) waterTemperature );
  lcd.print( ":" );
  lcd.print( selectorValue );


  // controlling the relaya and the info on screen
  if ( waterTemperature > ( selectorValue + hysteresys ) ){
    setRelay( relayPin , LOW ); 
    lcd.setCursor( 0 , 1 );
    lcd.write( byte ( 2 ) );
    lcd.print( " Cooling down " );
    lcd.write( byte ( 2 ) );
  } else if ( waterTemperature < ( selectorValue - hysteresys ) ){
    setRelay( relayPin , HIGH );  
    lcd.setCursor( 0 , 1 );
    lcd.write( byte ( 1 ) );
    lcd.print( "  Heating up  " );
    lcd.write( byte ( 1 ) );
  } else {
    lcd.setCursor( 0 , 1 );
    lcd.write( byte ( 0 ) );
    lcd.print( "    Enjoy!    " );
    lcd.write( byte ( 0 ) );
  }   
}
