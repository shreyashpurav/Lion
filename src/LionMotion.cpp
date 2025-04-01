/**
 * LionMotion.cpp
 *
 * @author Shreyash Purav
 */

#include <FEH.h>
#include <Arduino.h>

#include <FEHIO.h>
#include <FEHMotor.h>

#include "LionMotion.h"

Lion::Lion(FEHMotor &r_motor, FEHMotor &l_motor, DigitalEncoder &r_encoder, DigitalEncoder &l_encoder)
    : right_motor(r_motor), left_motor(l_motor), right_encoder(r_encoder), left_encoder(l_encoder)
{
    speed = (int)((12.0 / BatteryVoltage()) * 50.0);
    wait = 0.25;
}

void Lion::setSpeed(int _speed)
{
    speed = (int)((12.0 / BatteryVoltage()) * _speed);
}

void Lion::setWait(int _wait)
{
    wait = _wait;
}

void Lion::diffMotion(int c, bool rSgn, bool lSgn, int v)
{
    int rV = v, lV = v, rDiff, lDiff;
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
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < c)
    {
        //If right motor is lagging behind, |rightcounts| need to be increased
        //So if !rSgn, then the difference needs to become negative
        //Similarly, if left motor is lagging behind, diff will be negative
        //So to increase |leftcounts| diff will be subtracted from lSgn and added to -lSgn
        diff = (left_encoder.Counts() - right_encoder.Counts()) % (100 - v);
        diff /= 2.0;
        if (rSgn)
        {
            rDiff = diff;
        } else {
            rDiff = -diff;
        }
        if (lSgn)
        {
            lDiff = -diff;
        } else {
            lDiff = diff;
        }
        right_motor.SetPercent(rV + rDiff);
        left_motor.SetPercent(lV + lDiff);
    }
    right_motor.Stop();
    left_motor.Stop();
    Sleep(wait);
}

void Lion::fd(int counts)
{
    Lion::fd(counts, speed);
}

void Lion::fd(int counts, int _speed)
{
    Lion::diffMotion(counts, true, true, _speed);
}

void Lion::bk(int counts)
{
    Lion::bk(counts, speed);
}

void Lion::bk(int counts, int _speed)
{
    Lion::diffMotion(counts, false, false, _speed);
}

void Lion::rt(int counts)
{
    Lion::rt(counts, speed);
}

void Lion::rt(int counts, int _speed)
{
    Lion::diffMotion(counts, false, true, _speed);
}

void Lion::lt(int counts)
{
    Lion::lt(counts, speed);
}

void Lion::lt(int counts, int _speed)
{
    Lion::diffMotion(counts, true, false, _speed);
}

void Lion::regMotion(int c, bool mSgn, bool tSgn, int v, int d)
{
    int _v = v, _d = d;
    if (!mSgn)
    {
        _v = -v;
        _d = -d;
    }
    if (!tSgn)
    {
        _d *= -1;
    }
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_motor.SetPercent(_v - _d);
    left_motor.SetPercent(_v + _d);
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2 < c)
        ;
    right_motor.Stop();
    left_motor.Stop();
    Sleep(wait);
}

void Lion::fdr(int counts, int right)
{
    Lion::fdr(counts, right, speed);
}

void Lion::fdr(int counts, int right, int _speed)
{
    Lion::regMotion(counts, true, true, _speed, right);
}

void Lion::fdl(int counts, int left)
{
    Lion::fdl(counts, left, speed);
}

void Lion::fdl(int counts, int left, int _speed)
{
    Lion::regMotion(counts, true, false, _speed, left);
}

void Lion::bkr(int counts, int right)
{
    Lion::bkr(counts, right, speed);
}

void Lion::bkr(int counts, int right, int _speed)
{
    Lion::regMotion(counts, false, true, _speed, right);
}

void Lion::bkl(int counts, int left)
{
    Lion::bkl(counts, left, speed);
}

void Lion::bkl(int counts, int left, int _speed)
{
    Lion::regMotion(counts, false, false, _speed, left);
}