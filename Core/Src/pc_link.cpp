/*
 * pc_link.cpp
 *
 *  Created on: May 21, 2022
 *      Author: marci
 */


#include "pc_link.h"
#include "convert.h"
#include <vector>

void GameController::sendReport()
{
#ifdef GAME_CONTROLLER_TEST
    setTestData();
#endif

    std::vector<uint8_t> reportData
    {
        _ReportID,
        LOBYTE(data.X),
        HIBYTE(data.X),
        LOBYTE(data.Y),
        HIBYTE(data.Y),
        LOBYTE(data.Z),
        HIBYTE(data.Z),
        LOBYTE(data.Rz),
        HIBYTE(data.Rz),
        LOBYTE(data.Rx),
        HIBYTE(data.Rx),
        LOBYTE(data.Ry),
        HIBYTE(data.Ry),
        LOBYTE(data.slider),
        HIBYTE(data.slider),
        LOBYTE(data.dial),
        HIBYTE(data.dial),
        data.hat,
        static_cast<uint8_t>((data.buttons) & 0xFF),
        static_cast<uint8_t>((data.buttons >> 8) & 0xFF),
        static_cast<uint8_t>((data.buttons >> 16) & 0xFF),
        static_cast<uint8_t>((data.buttons >> 24) & 0xFF)
    };

    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, reportData.data(), reportData.size());    //send vector content
}

void GameController::setTestData()
{
    _testCounter++;
    data.X = data.Y = data.Z = data.Rz = _testCounter << 8;
    data.Rx = data.Ry = data.slider = data.dial = _testCounter << 7;
    data.hat = (_testCounter >> 5) + 1;
    uint8_t pattern = 1 << (_testCounter >> 5);
    data.buttons = pattern | (pattern << 8) | (pattern << 16) | (pattern << 24);
}

void SimController::sendReport()
{
    std::vector<uint8_t> reportData
    {
        _ReportID,
        'Y', 'o', 'k', 'e'
    };

    memcpy(usbdSendBuf, reportData.data(), reportData.size());  //copy vector content to send buffer
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbdSendBuf, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);  //send entire buffer
}

//parse new data received from simulator
void SimController::parseSimData()
{
    newDataReceived = 0;    //mark the data has been already processed
    uint8_t* pData = usbdRecBuf;
    if(_ReportID != *pData++)
    {
        //wrong report - exit
        return;
    }

    _simData.yokeXref = parseData<float>(pData);
    _simData.flags = parseData<uint32_t>(pData);
    _simData.normalizedSpeed = parseData<uint8_t>(pData);
    _simData.rotAccBodyX = parseData<float>(pData);
    _simData.rotAccBodyY = parseData<float>(pData);
    _simData.rotAccBodyZ = parseData<float>(pData);
    _simData.engineType = parseData<uint8_t>(pData);
    _simData.flapsPos = parseData<uint8_t>(pData);
    _simData.propellerPct = parseData<uint8_t>(pData);
    _simData.elevatorTrim = parseData<float>(pData);


    simOnline = getSimFlag(SimDataFlag::SimDataValid);
}
