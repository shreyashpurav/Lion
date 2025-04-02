/**
 * Include FEH and Arduino components
 */
#include <FEH.h>
#include <Arduino.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>

/**
 * Include custom components
 */
#include <LionMotion.h>

/**
 * Variables
 */
// Declarations for motors
FEHMotor right_motor(FEHMotor::Motor0, 9.0);
FEHMotor left_motor(FEHMotor::Motor1, 9.0);
// Declarations for sensors
// NOTE: Digital is restricted to Pin{8...14}
DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin14);
AnalogInputPin CdS_Cell(FEHIO::Pin13);
// Declarations for servo(s)
FEHServo arm(FEHServo::Servo0);
// Global variables for touch input, time
int x, y;
float t;
int diff;
# define speed 30
float wait = 0.25;

/**
 * Constants
 */
#define cds_red 0.6
#define servo_min 500
#define servo_max 2426

/**
 * Standard Functions
 */

// void setSpeed(int _speed)
// {
//     speed = (int)((12.0 / BatteryVoltage()) * _speed);
// }

void setWait(int _wait)
{
    wait = _wait;
}

/**
 * Standard functions for the motion control of the robots
 */
// fd = forward, bk = backward, rt = right, lt = left, fdr = forward turning right, fdl = forward turning left

void fd(int counts)
{
    int d;
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_motor.SetPercent(speed);
    left_motor.SetPercent(speed);
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < counts)
    {
        d = (left_encoder.Counts() - right_encoder.Counts()) % (100 - speed);
        d /= 2.0;
        right_motor.SetPercent(speed + d);
        left_motor.SetPercent(speed - d);
        //         if (left_encoder.Counts() > right_encoder.Counts())
        // {
        // right_motor.SetPercent(speed + 2);
        // left_motor.SetPercent(speed - 2);
        // }
        //         else if (left_encoder.Counts() < right_encoder.Counts())
        //         {
        //             right_motor.SetPercent(speed - 2);
        //             left_motor.SetPercent(speed + 2);
        //         }
    }
    right_motor.Stop();
    left_motor.Stop();
    Sleep(wait);
}

// void fd(int counts, int sp)
// {
//     int d; // derivative
//     right_encoder.ResetCounts();
//     left_encoder.ResetCounts();
//     right_motor.SetPercent(sp);
//     left_motor.SetPercent(speed);
//     while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < counts)
//     {
//         d = (left_encoder.Counts() - right_encoder.Counts()) % (100 - speed);
//         d /= 2.0;
//         right_motor.SetPercent(speed + d);
//         left_motor.SetPercent(speed - d);
//     }
//     right_motor.Stop();
//     left_motor.Stop();
//     Sleep(wait);
// }

void bk(int counts)
{
    int d;
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_motor.SetPercent(-speed);
    left_motor.SetPercent(-speed);
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < counts)
    {
        d = (left_encoder.Counts() - right_encoder.Counts()) % (100 - speed);
        d /= 2.0;
        right_motor.SetPercent(-speed - d);
        left_motor.SetPercent(-speed + d);
    }
    right_motor.Stop();
    left_motor.Stop();
    Sleep(wait);
}

// void bk(int counts, int speed)
// {
//     int d;
//     right_encoder.ResetCounts();
//     left_encoder.ResetCounts();
//     right_motor.SetPercent(-speed);
//     left_motor.SetPercent(-speed);
//     while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < counts)
//     {
//         d = (left_encoder.Counts() - right_encoder.Counts()) % (100 - speed);
//         d /= 2.0;
//         right_motor.SetPercent(-speed - d);
//         left_motor.SetPercent(-speed + d);
//     }
//     right_motor.Stop();
//     left_motor.Stop();
//     Sleep(wait);
// }

void rt(int counts)
{
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_motor.SetPercent(-speed);
    left_motor.SetPercent(speed);
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < counts)
        ;
    right_motor.Stop();
    left_motor.Stop();
    Sleep(wait);
}

// void rt(int counts, int speed)
// {
//     right_encoder.ResetCounts();
//     left_encoder.ResetCounts();
//     right_motor.SetPercent(-speed);
//     left_motor.SetPercent(speed);
//     while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < counts)
//         ;
//     right_motor.Stop();
//     left_motor.Stop();
//     Sleep(wait);
// }

void lt(int counts)
{
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_motor.SetPercent(speed);
    left_motor.SetPercent(-speed);
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < counts)
        ;
    right_motor.Stop();
    left_motor.Stop();
    Sleep(wait);
}

// void lt(int counts, int speed)
// {
//     right_encoder.ResetCounts();
//     left_encoder.ResetCounts();
//     right_motor.SetPercent(speed);
//     left_motor.SetPercent(-speed);
//     while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < counts)
//         ;
//     right_motor.Stop();
//     left_motor.Stop();
//     Sleep(wait);
// }

void bkr(int counts, int right)
{
    int d;
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_motor.SetPercent(-speed + right);
    left_motor.SetPercent(-speed - right);
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < counts)
        ;
    right_motor.Stop();
    left_motor.Stop();
    Sleep(wait);
}

void fdr(int counts, int right)
{
    int d;
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_motor.SetPercent(speed - right);
    left_motor.SetPercent(speed + right);
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < counts)
        ;
    right_motor.Stop();
    left_motor.Stop();
    Sleep(wait);
}


void CdSCellStart()
{
    arm.SetMax(servo_max);
    arm.SetMin(servo_min);
    arm.SetDegree(0);

    LCD.Clear(DARKSLATEGRAY);
    LCD.SetFontColor(RED);
    LCD.WriteLine("Waiting for CdS Cell");

    t = TimeNow();
    while ((CdS_Cell.Value() > cds_red) || (TimeNow() - t) < 30);
    
    LCD.Clear(RED);
    LCD.SetFontColor(DARKSLATEGRAY);
    LCD.WriteLine("Ready to Pounce");
}

void TouchStart()
{
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

    lt(158);
    bk(290);
    bkr(110, 10);
    fd(225);

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

    bk(100);
    fdr(60, 5);
    fdr(100, 20); // turn out
    fd(100);
    lt(80);
    fd(225); // go to ramp
    lt(80);  // turn to ramp
    fd(448); // ramp
    lt(79);
    fd(270);
    bk(280);
    rt(80);
    bk(500);
    bkr(100, 5);
}

void Checkpoint4()
{
    CdSCellStart();
    arm.SetDegree(90);
    bk(27);
    fd(27);
    fd(193);
    rt(30);
    fd(20);
    lt(60);
    fd(100);
    arm.SetDegree(0);
    bk(130);
    rt(30);
    bk(220);
    rt(42);
    fd(550);
    lt(36);
    fd(140);
    rt(40);
    fd(200);
    arm.SetDegree(110);
    Sleep(0.25);
    bk(150);
    arm.SetDegree(0);
    lt(35);
    fd(160);
    arm.SetDegree(150);
    Sleep(0.25);
    bk(160);
    arm.SetDegree(180);
    fd(160);
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
    fd(100);

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
    fd(200);

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
    fd(300);

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
    rt(20);

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
    rt(40);

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
    rt(80);
}

void Checkpoint5()
{
    //positioning
arm.SetDegree(30);
lt(55);
fd(90);
//Hit One
arm.SetDegree(95);
Sleep(1.0);

// Hit Two
bk(40);
arm.SetDegree(95);
Sleep(1.0);
fd(40);
arm.SetDegree(30);
Sleep(1.0);

// Hit Three
bk(40);
arm.SetDegree(95);
Sleep(1.0);
fd(40);
arm.SetDegree(30);
Sleep(1.0);

// Set arm to 0
arm.SetDegree(0);
}

void ERCMain()
{
    TouchStart();
    Checkpoint5();
    // Pre5();
}