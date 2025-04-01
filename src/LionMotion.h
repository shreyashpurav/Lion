/**
 * LionMotion.h
 * 
 * @author Shreyash Purav
 */

#ifndef LIONMOTION_H
#define LIONMOTION_H

#include <FEH.h>
#include <Arduino.h>

#include <FEHIO.h>
#include <FEHMotor.h>

/**
 * @brief Motion control of the lion
 */
class Lion
{
public:
    Lion(FEHMotor &right_motor, FEHMotor &left_motor, DigitalEncoder &right_encoder, DigitalEncoder &left_encoder);
    void fd(int counts);
    void fd(int counts, int _speed);
    void bk(int counts);
    void bk(int counts, int _speed);
    void rt(int counts);
    void rt(int counts, int _speed);
    void lt(int counts);
    void lt(int counts, int _speed);

    void fdr(int counts, int right);
    void fdr(int counts, int right, int _speed);
    void fdl(int counts, int left);
    void fdl(int counts, int left, int _speed);
    void bkr(int counts, int right);
    void bkr(int counts, int right, int _speed);
    void bkl(int counts, int left);
    void bkl(int counts, int left, int _speed);

    void setSpeed(int _speed);
    void setWait(int _wait);
private:
    FEHMotor &right_motor, &left_motor;
    DigitalEncoder &right_encoder, &left_encoder;
    int diff, speed, wait;

    void diffMotion(int c, bool rSgn, bool lSgn, int v);
    void regMotion(int c, bool mSgn, bool tSgn, int v, int d);
};

#endif // LIONMOTION_H