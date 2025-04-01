#include <FEH.h>
#include <Arduino.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <string.h>
#include <FEHSD.h>
#include <stdio.h>
#include <time.h>

#include <LionMotion.h>


/**
 * Variables
 */

// Declarations for motors
FEHMotor right_motor(FEHMotor::Motor0, 9.0);
FEHMotor left_motor(FEHMotor::Motor1, 9.0);

// Declarations for sensors
// Note: Digital is restricted to 8-14
DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin14);
AnalogInputPin CdS_Cell(FEHIO::Pin13);

// Declarations for servo(s)
FEHServo arm(FEHServo::Servo0);

// Declaration for the robot motion control object
Lion chuck(left_motor, right_motor, left_encoder, right_encoder);

/**
 * Constants
 */

#define cds_red 0.6
#define servo_min 700
#define servo_max 1200


/**
 * Functions
 */

void start()
{
    while ((CdS_Cell.Value() > cds_red))
    {
        right_motor.SetPercent(0);
        left_motor.SetPercent(0);
    }
}

void Checkpoint2()
{
    start();

    LCD.Clear(BLUE);
    LCD.SetFontColor(GREEN);
    LCD.WriteLine("Processing");
    LCD.WriteLine("Primary Points");

    chuck.lt(158);
    chuck.bk(290);
    chuck.bkr(110, 10);
    chuck.fd(225);

    Sleep(0.5);
    LCD.Clear(YELLOW);
    LCD.SetFontColor(RED);
    LCD.WriteLine("Now get ready for");
    LCD.WriteLine("The Sprity Woodchuck-Lion!");
    Sleep(2.5);

    LCD.Clear(BLUE);
    LCD.SetFontColor(GREEN);
    LCD.WriteLine("Processing");
    LCD.WriteLine("Bonus");

    chuck.bk(100);
    chuck.fdr(60, 5);
    chuck.fdr(100, 20); // turn out
    chuck.fd(100);
    chuck.lt(80);
    chuck.fd(225); // go to ramp
    chuck.lt(80);  // turn to ramp
    chuck.fd(448); // ramp
    chuck.lt(79);
    chuck.fd(270);
    chuck.bk(280);
    chuck.rt(80);
    chuck.bk(500);
    chuck.bkr(100, 5);
}

void Checkpoint4()
{
    start();
    arm.SetDegree(90);
    chuck.bk(27);
    chuck.fd(27);
    chuck.fd(193);
    chuck.rt(30);
    chuck.fd(20);
    chuck.lt(60);
    chuck.fd(100);
    arm.SetDegree(0);
    chuck.bk(130);
    chuck.rt(30);
    chuck.bk(220);
    chuck.rt(42);
    chuck.fd(550);
    chuck.lt(36);
    chuck.fd(140);
    chuck.rt(40);
    chuck.fd(200);
    arm.SetDegree(110);
    Sleep(0.25);
    chuck.bk(150);
    arm.SetDegree(0);
    chuck.lt(35);
    chuck.fd(160);
    arm.SetDegree(150);
    Sleep(0.25);
    chuck.bk(160);
    arm.SetDegree(180);
    chuck.fd(160);
    Sleep(5.0);
    arm.SetDegree(30);
}

void Pre5()
{
    
}

void Checkpoint5()
{

}

void ERCMain()
{
    int x, y;
    arm.SetMax(servo_max);
    arm.SetMin(servo_min);
    arm.SetDegree(0);

    LCD.Clear(BLACK);
    LCD.SetFontColor(GREEN);
    LCD.WriteLine("Press to Start");
    while (!LCD.Touch(&x, &y))
        ;
    while (LCD.Touch(&x, &y))
        ;
    LCD.Clear(RED);
    LCD.SetFontColor(GRAY);
    LCD.WriteLine("Waiting to Start");
    // The real code starts here.
    
}