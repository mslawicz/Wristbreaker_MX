/*
 * motor_BLDC.h
 *
 *  Created on: 27 maj 2022
 *      Author: marci
 */

#ifndef INC_MOTOR_BLDC_H_
#define INC_MOTOR_BLDC_H_

#include "stm32f4xx_hal.h"
#include "actuator.h"
#include <array>

class MotorBLDC : public Actuator
{
public:
    MotorBLDC(uint8_t polePairs, TIM_HandleTypeDef* pPwmHtim);
    ~MotorBLDC() {}
    void initialize() override;
    bool calibrate(HapticParam& hapticParam) override;
    void action() override;
private:
    void setFieldVector(float angle, float magnitude);
    float getSvmValue(float argument);
    uint8_t _polePairs;
    float _phase{0};
    float _magnitude{0};
    TIM_HandleTypeDef* _pPwmHtim;   //pointer to PWM timer handle for this motor
    static constexpr size_t LutSize = 91;
    //Space Vector Modulation Look Up Table for angles 0-90 degrees
    static constexpr std::array<float, LutSize> SvmLUT
    {
    0.0F, 0.029570267F, 0.059134421F, 0.08868635F, 0.118219945F,
    0.147729104F, 0.177207733F, 0.206649745F, 0.236049066F, 0.265399635F,
    0.294695404F, 0.323930345F, 0.353098444F, 0.382193712F, 0.41121018F,
    0.440141902F, 0.468982959F, 0.49772746F, 0.526369544F, 0.554903378F,
    0.583323165F, 0.611623142F, 0.639797582F, 0.667840798F, 0.69574714F,
    0.723511001F, 0.751126819F, 0.778589075F, 0.805892297F, 0.833031063F,
    0.86F, 0.867793786F, 0.875407154F, 0.88283489F, 0.890071839F,
    0.897112902F, 0.90395304F, 0.910587277F, 0.917010697F, 0.92321845F,
    0.929205751F, 0.934967883F, 0.940500197F, 0.945798114F, 0.950857126F,
    0.955672798F, 0.96024077F, 0.964556757F, 0.968616549F, 0.972416017F,
    0.97595111F, 0.979217856F, 0.982212367F, 0.984930837F, 0.987369544F,
    0.989524851F, 0.991393208F, 0.992971153F, 0.994255311F, 0.995242396F,
    0.995929214F, 0.996312663F, 0.996389732F, 0.996157503F, 0.995613153F,
    0.994753955F, 0.993577276F, 0.992080581F, 0.990261433F, 0.98811749F,
    0.985646514F, 0.982846362F, 0.979714994F, 0.976250469F, 0.97245095F,
    0.9683147F, 0.963840085F, 0.959025575F, 0.953869741F, 0.948371261F,
    0.942528916F, 0.936341592F, 0.929808279F, 0.922928074F, 0.91570018F,
    0.908123903F, 0.900198658F, 0.891923965F, 0.883299451F, 0.874324849F,
    0.865F
    };
};



#endif /* INC_MOTOR_BLDC_H_ */
