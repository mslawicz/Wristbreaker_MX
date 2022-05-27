/*
 * SVM_lut.cpp
 *
 *  Created on: 27 maj 2022
 *      Author: marci
 */

#include "SVM_lut.h"

//alternate-reverse space vector modulation
//values for 0-90 degrees in 1 degree step
const int16_t SVM_value[] =
{
    0x0000, 0x03C8, 0x0791, 0x0B5A, 0x0F21,
    0x12E8, 0x16AE, 0x1A73, 0x1E36, 0x21F8,
    0x25B8, 0x2976, 0x2D32, 0x30EB, 0x34A2,
    0x3856, 0x3C07, 0x3FB5, 0x4360, 0x4707,
    0x4AAA, 0x4E49, 0x51E4, 0x557B, 0x590E,
    0x5C9C, 0x6024, 0x63A8, 0x6727, 0x6AA0,
    0x6E14, 0x6F13, 0x700D, 0x7100, 0x71ED,
    0x72D4, 0x73B4, 0x748E, 0x7560, 0x762C,
    0x76F0, 0x77AD, 0x7862, 0x790F, 0x79B5,
    0x7A53, 0x7AE9, 0x7B76, 0x7BFB, 0x7C78,
    0x7CEB, 0x7D57, 0x7DB9, 0x7E12, 0x7E62,
    0x7EA8, 0x7EE5, 0x7F19, 0x7F43, 0x7F64,
    0x7F7A, 0x7F87, 0x7F89, 0x7F82, 0x7F70,
    0x7F54, 0x7F2D, 0x7EFC, 0x7EC0, 0x7E7A,
    0x7E29, 0x7DCD, 0x7D67, 0x7CF5, 0x7C79,
    0x7BF1, 0x7B5F, 0x7AC1, 0x7A18, 0x7964,
    0x78A4, 0x77DA, 0x7703, 0x7622, 0x7535,
    0x743D, 0x7339, 0x722A, 0x710F, 0x6FE9,
    0x6EB8
};

const uint8_t SVM_size = sizeof(SVM_value) / sizeof(SVM_value[0]);

//get SVM value
//angle in degrees * 100
//returns SVM value <-32768,32767>
int16_t getSVMvalue(int32_t angle)
{
    bool negative = false;

    //make angle >= 0
    if(angle < 0)
    {
        angle = - angle;
        negative = !negative;
    }

    //make angle < 360 degrees
    angle %= 36000;

    //make angle < 180 degrees
    if(angle > 18000)
    {
        angle -= 18000;
        negative = !negative;
    }

    //make angle < 90 degrees
    if(angle > 9000)
    {
        angle = 18000 - angle;
    }

    uint8_t lowerIndex = angle / 100;
    int16_t value;

    if(lowerIndex >= SVM_size - 1)
    {
        value = SVM_value[SVM_size - 1];
    }
    else
    {
        value = (SVM_value[lowerIndex] + (SVM_value[lowerIndex + 1] - SVM_value[lowerIndex]) * (angle % 100) / 100);
    }

    return negative ? -value : value;
}


