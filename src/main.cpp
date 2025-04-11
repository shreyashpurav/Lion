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
 * Include Standard components
 */
#include <stdlib.h>

/**
 * Variables
 */
// Declarations for motors
FEHMotor right_motor(FEHMotor::Motor0, 9.0);
FEHMotor left_motor(FEHMotor::Motor1, 9.0);

// Declarations for sensors
// NOTE: Digital is restricted to Pin{8...14}
DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin10);
AnalogInputPin CdS_Cell(FEHIO::Pin13);

// Declarations for servo(s)
FEHServo arm(FEHServo::Servo0);

// Global constants and variables
#define motor_percent 40
#define wait 0.25
int x, y, course;
long t;
int differential;

/**
 * Constants
 */
#define cds_red 0.6
#define servo_min 500
#define servo_max 2426

/**
 * Display functions
 */
void CourseSelect()
{
    LCD.SetFontColor(WHITE);
    char letters[4][2] = {{'A', 'E'}, {'L', 'F'}, {'C', 'G'}, {'D', 'H'}};
    /* Display Options */
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            LCD.DrawRectangle(i * (LCD_WIDTH / 4), j * (LCD_HEIGHT / 2), (i + 1) * (LCD_WIDTH / 4), (j + 1) * (LCD_HEIGHT / 2));
            LCD.WriteAt(letters[i][j], ((2 * i + 1) / 2.0) * (LCD_WIDTH / 4), ((2 * j + 1) / 2.0) * (LCD_HEIGHT / 2));
        }
    }
    /* Get Input */
    while (!LCD.Touch(&x, &y))
        ;
    while (LCD.Touch(&x, &y))
        ;

    for (int i = 4; i >= 1; i--)
    {
        for (int j = 2; j >= 1; j--)
        {
            if (x < i * (LCD_WIDTH / 4) && y < j * (LCD_HEIGHT / 2))
            {
                course = (i - 1) + (j - 1) * 4;
            }
        }
    }
    LCD.Clear();
}

/**
 * Sub-functions that enable the standard functions
 */

void dMotion(int c, bool rSgn, bool lSgn, int v)
{
    int _rCounts_, _lCounts_;
    int rV = v, lV = v, rDifferential, lDifferential;
    if (!rSgn)
    {
        rV = -v;
    }
    if (!lSgn)
    {
        lV = -v;
    }

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_motor.SetPercent(rV);
    left_motor.SetPercent(lV);

    _rCounts_ = right_encoder.Counts();
    _lCounts_ = left_encoder.Counts();
    t = millis();

    while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < c)
    {
        // If right motor is lagging behind, |rightcounts| need to be increased
        // So if !rSgn, then the difference needs to become negative
        // Similarly, if left motor is lagging behind, diff will be negative
        // So to increase |leftcounts| diff will be subtracted from lSgn and added to -lSgn
        differential = (left_encoder.Counts() - right_encoder.Counts()) % (100 - v);
        differential /= 2.0;
        if (rSgn)
        {
            rDifferential = differential;
        }
        else
        {
            rDifferential = -differential;
        }
        if (lSgn)
        {
            lDifferential = -differential;
        }
        else
        {
            lDifferential = differential;
        }
        right_motor.SetPercent(rV + rDifferential);
        left_motor.SetPercent(lV + lDifferential);

        if (millis() - t > 200)
        {
            if (right_encoder.Counts() == _rCounts_ && left_encoder.Counts() == _lCounts_)
            {
                break;
            }
            _rCounts_ = right_encoder.Counts();
            _lCounts_ = left_encoder.Counts();
            t = millis();
        }
    }
    right_motor.Stop();
    left_motor.Stop();
    Sleep(wait);
}

void rMotion(int c, int mSgn, int tSgn, int v, int d)
{
    int speed = v, difference = d;
    if (!mSgn)
    {
        speed = -v;
        difference = -d;
    }
    if (!tSgn)
    {
        difference *= -1;
    }
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_motor.SetPercent(speed - difference);
    left_motor.SetPercent(speed + difference);
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < c)
        ;
    right_motor.Stop();
    left_motor.Stop();
    Sleep(wait);
}

/**
 * Standard functions for the motion control of the robots
 */

void fd(int counts, int speed)
{
    dMotion(counts, true, true, speed);
}

void fd(int counts)
{
    fd(counts, motor_percent);
}

void bk(int counts, int speed)
{
    dMotion(counts, false, false, speed);
}

void bk(int counts)
{
    bk(counts, motor_percent);
}

void rt(int counts, int speed)
{
    dMotion(counts, false, true, speed);
}

void rt(int counts)
{
    rt(counts, motor_percent);
}

void lt(int counts, int speed)
{
    dMotion(counts, true, false, speed);
}

void lt(int counts)
{
    lt(counts, motor_percent);
}

void fdr(int counts, int right, int speed)
{
    rMotion(counts, true, true, speed, right);
}

void fdr(int counts, int right)
{
    fdr(counts, right, motor_percent);
}

void fdl(int counts, int left, int speed)
{
    rMotion(counts, true, false, speed, left);
}

void fdl(int counts, int left)
{
    fdl(counts, left, motor_percent);
}

void bkr(int counts, int right, int speed)
{
    rMotion(counts, false, true, speed, right);
}

void bkr(int counts, int right)
{
    bkr(counts, right, motor_percent);
}

void bkl(int counts, int left, int speed)
{
    rMotion(counts, false, false, speed, left);
}

void bkl(int counts, int left)
{
    bkl(counts, left, motor_percent);
}

/**
 * The actual methods
 */
void CdSCellStart()
{
    arm.SetMax(servo_max);
    arm.SetMin(servo_min);
    arm.SetDegree(0);

    LCD.Clear(LIGHTGRAY);
    LCD.SetFontColor(RED);
    LCD.WriteLine("Waiting for CdS Cell");

    t = TimeNow();
    while ((CdS_Cell.Value() > cds_red) || (TimeNow() - t) < 30)
        ;

    LCD.Clear(RED);
    LCD.SetFontColor(DARKSLATEGRAY);
    LCD.WriteLine("Ready to Pounce");
}

void TouchStart()
{
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
    // positioning
    arm.SetDegree(30);
    lt(55);
    fd(90);
    // Hit One
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

void HitDn(int dodge_angle)
{
    arm.SetDegree(95);
    Sleep(0.5);
    lt(dodge_angle);
    arm.SetDegree(30);
    Sleep(0.5);
    rt(dodge_angle);
}

void FinalRunA()
{
    /* Press Button */
    bk(30);
    /* Go to Window */
    fd(210);
    lt(162);
    bk(270);
    bkr(110, 20);
    /* Open Window */
    fd(150);
    // while (!RCS.isWindowOpen()) {
    //     fd(50);
    // }
    /* From Window, Go to Composter */
    bk(90);
    rt(30);
    arm.SetDegree(30);
    fd(200);
    /* Hit down */
    HitDn(10);
    HitDn(10);
    HitDn(10);
    // TODO: Add Bonus after everything else works
    /* From Composter, Go to Apple Bucket */
    bkl(200, 10);
    arm.SetDegree(40);
    fd(130);
    arm.SetDegree(20);
    /* From Apple Bucket, Go to Ramp */
    bk(130);
    rt(40);
    bk(120);
    rt(42);
    /* Climb Ramp */
    fd(400);
    lt(30);
    fd(100);
    rt(32);
    fd(120);
    arm.SetDegree(45);
    bk(70);
    /* From Apple Crate, Go to Lever */
    arm.SetDegree(0);
    switch (RCS.GetLever())
    {
    case 0:
        bk(90);
        lt(36);
        fd(120);
        break;
    case 1:
        bk(50);
        lt(36);
        fd(80);
        break;
    case 2:
        lt(36);
        fd(50);
        break;
    default:
        lt(36);
        fd(50);
    }
    arm.SetDegree(80);
    Sleep(wait);
    bk(20);
    arm.SetDegree(105);
    Sleep(wait);
    fd(20);
    Sleep(4.5);
    arm.SetDegree(60);
    Sleep(wait);
    /* From Lever, Go to Thermostat */
    bk(20);
    arm.SetDegree(0);
    switch (RCS.GetLever())
    {
    case 0:
        bkr(90, 18);
        fd(50);
        break;
    case 1:
        bk(50);
        bkr(60, 18);
        fd(90);
        break;
    case 2:
        bk(100);
        bkr(40, 18);
        fd(120);
        break;
    default:
        bk(100);
        bkr(40, 18);
        fd(120);
    }
    /* Code for CdS */
}

void FinalRunB()
{
    /* Press Button */
    bk(30);
    /* Align with back wall */
    fd(210);
    lt(120);
    bk(500); // overshoot
    fd(50);
    /* Turn to wall */
    rt(80);
    fd(500); // overshoot
    bkr(40, 20);
    bk(200);
}

void FinalRunC()
{
    FinalRunA();
}

void FinalRunD()
{
    FinalRunA();
}

void FinalRunE()
{
    FinalRunA();
}

void FinalRunF()
{
    FinalRunA();
}

void FinalRunG()
{
    FinalRunA();
}

void FinalRunH()
{
    FinalRunA();
}

/**
 * Main.
 */
void ERCMain()
{
    // RCS.InitializeTouchMenu("1240E8QWF");
    // LCD.Clear();
    // CourseSelect();
    // TouchStart();
    // CdSCellStart();

    // switch (course)
    // {
    // case 0:
    //     FinalRunA();
    //     break;
    // case 1:
    //     FinalRunB();
    //     break;
    // case 2:
    //     FinalRunC();
    //     break;
    // case 3:
    //     FinalRunD();
    //     break;
    // case 4:
    //     FinalRunE();
    //     break;
    // case 5:
    //     FinalRunF();
    //     break;
    // case 6:
    //     FinalRunG();
    //     break;
    // case 7:
    //     FinalRunH();
    //     break;
    // default:
    //     FinalRunG();
    // }

    TouchStart();
    bk(500);
}