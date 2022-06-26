/*
 * pc_link.h
 *
 *  Created on: May 21, 2022
 *      Author: marci
 */

#ifndef INC_PC_LINK_H_
#define INC_PC_LINK_H_

#include "stm32f4xx_hal.h"
#include "usbd_custom_hid_if.h"

//#define GAME_CONTROLLER_TEST

constexpr size_t SimDataSize = 63;

enum class SimDataFlag : uint8_t
{
    SimDataValid,
    AutopilotOn
};

struct GameControllerDataType
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    int16_t Rz;
    uint16_t Rx;
    uint16_t Ry;
    uint16_t slider;
    uint16_t dial;
    uint8_t hat;
    uint32_t buttons;
};

struct SimDataType
{
    uint8_t flapPositions;
    uint8_t flapIndex;
    float yokeXref;
    uint32_t flags;
    float normalizedSpeed;
    float rotAccBodyX;
    float rotAccBodyY;
    float rotAccBodyZ;
    uint8_t engineType;
};

class GameController
{
public:
    void sendReport();
    GameControllerDataType data;
    static constexpr uint32_t ReportInterval = 10000U;    //game controller report sending interval
private:
    void setTestData();
    const uint8_t _ReportID = 1;
    uint8_t _testCounter{0};
};

class SimController
{
public:
    void sendReport();
    bool isNewDataReceived() const { return 0 != newDataReceived; }
    void parseSimData();
    SimDataType& getSimData() { return _simData; }
    bool getSimFlag(SimDataFlag flag) const { return (_simData.flags & (1 << static_cast<uint8_t>(flag))) != 0; }
    bool simOnline{false};
    static constexpr uint32_t ReportInterval = 23456U;    //simulator controller report sending interval
    static constexpr uint32_t OfflineTimout = 100000U;    //timout of sim data inactivity
private:
    const uint8_t _ReportID = 2;
    SimDataType _simData;
};

#endif /* INC_PC_LINK_H_ */
