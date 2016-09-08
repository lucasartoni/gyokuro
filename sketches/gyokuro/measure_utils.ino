
// Analog reads multiple times and averages the results
int analogSmoothRead( int pin , int numReadings , int timeout ){
  int readings[ numReadings ]; 
  int total = 0;
  int average = 0;
  for ( int i = 0 ; i < numReadings ; i++ ){
    readings[ i ] = analogRead( pin );
    total = total + readings[ i ];
    delay( timeout );
  }
  average = total / numReadings;
  return average;
}

// Reads selector multiple times and gives averaged selection
int getSelectorValue( int analogPin , int minValue, int maxValue ){
  return map( analogSmoothRead ( analogPin , 20 , 10 ) , 0 , 1023 , minValue , maxValue );
}

void setRelay( int digitalPin , int value ){
  digitalWrite( digitalPin , value );  
}


