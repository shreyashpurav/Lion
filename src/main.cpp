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

// Global variables for touch input
int x, y;

/**
 * Constants
 */

#define cds_red 0.6
#define servo_min 700
#define servo_max 1200

/**
 * Functions
 */

void CdSCellStart()
{
    arm.SetMax(servo_max);
    arm.SetMin(servo_min);
    arm.SetDegree(0);

    LCD.Clear(DARKSLATEGRAY);
    LCD.SetFontColor(RED);
    LCD.WriteLine("Waiting for CdS Cell");
    while ((CdS_Cell.Value() > cds_red))
    {
        right_motor.SetPercent(0);
        left_motor.SetPercent(0);
    }
    LCD.Clear(RED);
    LCD.SetFontColor(DARKSLATEGRAY);
    LCD.WriteLine("Ready to Pounce");
}

void TouchStart()
{
    arm.SetMax(servo_max);
    arm.SetMin(servo_min);
    arm.SetDegree(0);

    LCD.Clear(BLACK);
    LCD.SetFontColor(PURPLE);
    LCD.WriteLine("Press to Start");
    while (!LCD.Touch(&x, &y))
        ;
    while (LCD.Touch(&x, &y))
        ;
    LCD.Clear(BLACK);
    LCD.SetFontColor(YELLOW);
    LCD.WriteLine("Ready to Pounce");
}

void Checkpoint2()
{
    CdSCellStart();
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
    CdSCellStart();
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
    // Go forward by 100
    LCD.Clear(BLUE);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Press for fd(100)");
    while (!LCD.Touch(&x, &y))
        ;
    while (LCD.Touch(&x, &y))
        ;
    LCD.Clear(WHITE);
    LCD.SetFontColor(BLUE);
    LCD.WriteLine("fd(100)");
    chuck.fd(100);

    // Go forward by 200
    LCD.Clear(BLUE);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Press for fd(200)");
    while (!LCD.Touch(&x, &y))
        ;
    while (LCD.Touch(&x, &y))
        ;
    LCD.Clear(WHITE);
    LCD.SetFontColor(BLUE);
    LCD.WriteLine("fd(200)");
    chuck.fd(200);

    // Go forward by 300
    LCD.Clear(BLUE);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Press for fd(300)");
    while (!LCD.Touch(&x, &y))
        ;
    while (LCD.Touch(&x, &y))
        ;
    LCD.Clear(WHITE);
    LCD.SetFontColor(BLUE);
    LCD.WriteLine("fd(300)");
    chuck.fd(300);

    // Turn right by 20
    LCD.Clear(BLUE);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Press for rt(20)");
    while (!LCD.Touch(&x, &y))
        ;
    while (LCD.Touch(&x, &y))
        ;
    LCD.Clear(WHITE);
    LCD.SetFontColor(BLUE);
    LCD.WriteLine("rt(20)");
    chuck.rt(20);

    // Turn right by 40
    LCD.Clear(BLUE);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Press for rt(40)");
    while (!LCD.Touch(&x, &y))
        ;
    while (LCD.Touch(&x, &y))
        ;
    LCD.Clear(WHITE);
    LCD.SetFontColor(BLUE);
    LCD.WriteLine("rt(40)");
    chuck.rt(40);

    // Turn right by 80
    LCD.Clear(BLUE);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Press for rt(80)");
    while (!LCD.Touch(&x, &y))
        ;
    while (LCD.Touch(&x, &y))
        ;
    LCD.Clear(WHITE);
    LCD.SetFontColor(BLUE);
    LCD.WriteLine("rt(80)");
    chuck.rt(80);
}

void Checkpoint5()
{
}

void ERCMain()
{
    TouchStart();
    arm.TouchCalibrate();
    // Pre5();
}