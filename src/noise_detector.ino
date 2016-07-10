/***
 *       Filename:  noise_detector.ino
 *
 *    Description:  Project file.
 *
 *        Version:  0.0.1
 *        Created:  2016-07-10

 *       Revision:  none
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *
 *        License:  GNU GPL2
 */


// the setup routine runs once when you press reset:
void setup()
{
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);

    pinMode( A2, OUTPUT );
    pinMode( 13, OUTPUT );

    digitalWrite( A2, HIGH );

    pinMode( A1, OUTPUT );
    digitalWrite( A1, LOW );

}

// the loop routine runs over and over again forever:
void loop() 
{
    // read the input on analog pin 0:
    int sensorValue = analogRead(A0);
    // print out the value you read:
    if( sensorValue > 100 )
    {
        Serial.println( "Trigger" );
        digitalWrite( 13, HIGH );
        delay( 100 );
    }
    digitalWrite( 13, LOW );
    delay(1);        // delay in between reads for stability
}
