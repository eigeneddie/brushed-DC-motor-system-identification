//System Identification 
// Eddie Sutawika

#include "mbed.h"
#include "motor.h"  
#include "encoderMotor.h"

Serial pc(USBTX,USBRX, 9600);

DigitalOut led(LED1); // checking to see if the motor should be on or not


    //1. Define pin class of motor Parameter : digital right, digital left, dan PWM pin respectively
        motor motor1 (PC_7, PA_8, PB_10);


    //2. Define pin class encoder penggiring. Parameter : ChA, ChB, dan PPR penggiring secara berurutan
    // PPR Total = PPR Encoder * Mode Encoding * Gear Reduction 

        encoderMotor enc1(PB_2, PB_12, 103.6 , encoderMotor::X4_ENCODING); //PPR Total = PPR Encoder * Mode Encoding * Gear Reduction 103.6

    //3. Define array to store DC motor response
    
        double v_3[300]; // Array of speed
        int step[300];
        double Ts = 0.02 ; // [s] Sampling time
        double lead_Leadscrew = 8; //[mm/rev] Lead of leadscrew

    //--> Keep track of time
        int i,delay[300],oldtime  ;

        Timer t; 

    //4. Main program

int main()
{
// Initialization

    led = 0;
    wait(3);
    pc.printf("CLEARDATA");
    pc.printf("\n");

    pc.printf("Step, V3, delay (ms)");
    pc.printf("\n");
    
    t.start();
    oldtime = 0 ;


// Start getting data
    for ( i = 0 ; i < 300 ; i++ )
    {
    // at 150x20ms, start giving step input.
        if ( i == 150 )
        { 
        // Step input by giving 100% PWM.
        // Also turn on LED to signify it should be on.
            motor1.setpwm(1);
            led = 1; 
            
        }
        
        //Output speed in [mm/s].
        step[i] = led;
        v_3[i] = enc1.getRevolutions() * lead_Leadscrew/ Ts ; 
        delay[i] = t.read_ms() - oldtime;
        oldtime = t.read_ms();
        wait_ms(Ts*1000);


    }
     
    t.reset();
    
    // Turn the motor and LED off.
    motor1.setpwm(0);
    led = 0;
   
    for ( i = 0 ; i < 300 ; i++ )
    {
        
        //Delay dalam ms.
        //Jangan lupa bagi s_l dan s_r dibagi 100000 di excel.
        pc.printf("%d,%d,%d", step[i], int(v_3[i]*100000),delay[i]); 
        pc.printf("\n");
        
    }    
    while(1)
    {
        //Do nothing forever.
    }   
}
