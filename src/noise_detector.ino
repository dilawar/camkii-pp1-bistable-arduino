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

#define WINDOW_SIZE 20

#define INPUT_PIN   A0
#define INPUT_GND   A1
#define INPUT_VCC   A2

#define OUTPUT_PINS_START   7
#define OUTPUT_PINS_END     12
#define GROUND_PIN          13

/**
 * @brief Keep the running values of signal.
 */
int signal_[WINDOW_SIZE];

/*  Running mean of signal. */
float running_mean_ = 0.0;
int pp1 = 12;

float kh1 = 300.0;

double computeMean(  )
{
    double sum = 0.0;
    for (size_t i = 0; i < WINDOW_SIZE; i++) 
        sum += signal_[i];
    running_mean_ = sum / WINDOW_SIZE;
    return running_mean_;
}

void appendToSignal( int sensorValue )
{
    for (size_t i = 0; i < WINDOW_SIZE - 1; i++) 
        signal_[i] = signal_[i+1];
    signal_[WINDOW_SIZE-1] = sensorValue;
}

int slow( int ca )
{
    return 1024 * pow(( ca / kh1 ),  6) / pow(( 1 + pow((ca / kh1 ), 3) ), 2 );
}

int fast( int ca )
{
    return 1024 * pow(( ca / kh1 ), 3) / ( 1 + pow((ca / kh1 ), 3) ); 
}

bool isClockWiseNeighbourOn( size_t pin )
{
    int pinIndex = (pin + 1) % 6;
    return HIGH == digitalRead( pinIndex );
}

void system( int sensorValue )
{
    int max = 1024;


    int rateOn = slow( sensorValue );

    // The probability of turning any OUTPUT ON.
    for( size_t pin = OUTPUT_PINS_START; pin <= OUTPUT_PINS_END; pin++ )
    {
        if( isClockWiseNeighbourOn( pin ) )
            rateOn = fast( sensorValue );

        int r = random( 0, 1024 );
        if( r < rateOn )
        {
            if( LOW == digitalRead( pin ) )
            {
                digitalWrite( pin, HIGH );
                delay( 10 );
                pp1 += 1;
            }
        }
    }

    int rateOff = pp1;
    for( size_t pin = OUTPUT_PINS_START; pin <= OUTPUT_PINS_END; pin++ )
    {
        int r = random( 0, 1024 );
        if( r < rateOff )
        {
            if( HIGH == digitalRead( pin ) )
            {
                digitalWrite( pin, LOW );
                delay( 10 );
                pp1 -= 1;
            }
        }
    }

    char msg[100];
    sprintf( msg, "ca=%4d,pp1=%2d,rON=%3d,rOFF=%3d", sensorValue, pp1, rateOn, rateOff );
    Serial.print( msg );

    for( size_t i = OUTPUT_PINS_START; i <= OUTPUT_PINS_END; i++)
    {
        Serial.print( ' ' );
        Serial.print( digitalRead( i ) );
    }
    Serial.println( " " );
}

// the setup routine runs once when you press reset:
void setup()
{
    // initialize serial communication at 9600 bits per second:
    Serial.begin( 38400 );

    pinMode( GROUND_PIN, OUTPUT );
    digitalWrite( GROUND_PIN, LOW );

    for( size_t i = OUTPUT_PINS_START; i < OUTPUT_PINS_END; i ++ )
    {
        pinMode( i, OUTPUT );
        digitalWrite( i, LOW );
    }


    pinMode( INPUT_VCC, OUTPUT );
    digitalWrite( INPUT_VCC, HIGH );

    pinMode( INPUT_GND, OUTPUT );
    digitalWrite( INPUT_GND, LOW );

    // Read sensor input from here. Calcium
    pinMode( INPUT_PIN, INPUT );

    /* Initialize signal  */
    for (size_t i = 0; i < WINDOW_SIZE; i++) 
        signal_[i] = 10;

}

// the loop routine runs over and over again forever:
void loop() 
{
    // read the input on analog pin 0:
    int sensorValue = analogRead(INPUT_PIN);
    system( sensorValue );

#if 0
    Serial.print( sensorValue );
    Serial.print( ' ' );
    Serial.print( running_mean_ );
    Serial.print( '\n' );
    if( sensorValue > (1.1 * running_mean_) )
    {
        Serial.println( "Trigger" );
        for (size_t i = 0; i < 5; i++) 
        {
            digitalWrite( 13, HIGH );
            delay( 30 );
            digitalWrite( 13, LOW);
            delay( 30 );
            
        }
    }
    digitalWrite( 13, LOW );
    delay(1); 
#endif 

}
