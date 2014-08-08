/****************************************************************************
* ITG3200.h - ITG-3200/I2C library v0.6 for Arduino                         *
* Copyright 2010-2012 Filipe Vieira & various contributors                  *
* http://code.google.com/p/itg-3200driver                                   *
* This file is part of ITG-3200 Arduino library.                            *
*                                                                           *
* This library is free software: you can redistribute it and/or modify      *
* it under the terms of the GNU Lesser General Public License as published  *
* by the Free Software Foundation, either version 3 of the License, or      *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU Lesser General Public License for more details.                       *
*                                                                           *
* You should have received a copy of the GNU Lesser General Public License  *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
****************************************************************************/
/****************************************************************************
* Tested on Arduino Mega with ITG-3200 Breakout                             *
* SCL     -> pin 21     (no pull up resistors)                              *
* SDA     -> pin 20     (no pull up resistors)                              *
* CLK & GND -> pin GND                                                      *
* INT       -> not connected  (but can be used)                             *
* VIO & VDD -> pin 3.3V                                                     *
*****************************************************************************/
#ifndef ITG3200_h
#define ITG3200_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "I2CDevice.h"
#include "imumaths.h"

#define ITG3200_ADDR_AD0_HIGH  0x69
#define ITG3200_ADDR_AD0_LOW   0x68
#define GYROSTART_UP_DELAY  70


#define WHO_AM_I           0x00  // RW   SETUP: I2C address   
#define SMPLRT_DIV         0x15  // RW   SETUP: Sample Rate Divider       
#define DLPF_FS            0x16  // RW   SETUP: Digital Low Pass Filter/ Full Scale range
#define INT_CFG            0x17  // RW   Interrupt: Configuration
#define INT_STATUS         0x1A  // R	Interrupt: Status
#define TEMP_OUT           0x1B  // R	SENSOR: Temperature 2bytes
#define GYRO_XOUT          0x1D  // R	SENSOR: Gyro X 2bytes  
#define GYRO_YOUT          0x1F  // R	SENSOR: Gyro Y 2bytes
#define GYRO_ZOUT          0x21  // R	SENSOR: Gyro Z 2bytes
#define PWR_MGM            0x3E  // RW	Power Management

/* ---- bit maps ---- */
#define DLPFFS_FS_SEL             0x18  // 00011000
#define DLPFFS_DLPF_CFG           0x07  // 00000111
#define INTCFG_ACTL               0x80  // 10000000
#define INTCFG_OPEN               0x40  // 01000000
#define INTCFG_LATCH_INT_EN       0x20  // 00100000
#define INTCFG_INT_ANYRD_2CLEAR   0x10  // 00010000
#define INTCFG_ITG_RDY_EN         0x04  // 00000100
#define INTCFG_RAW_RDY_EN         0x01  // 00000001
#define INTSTATUS_ITG_RDY         0x04  // 00000100
#define INTSTATUS_RAW_DATA_RDY    0x01  // 00000001
#define PWRMGM_HRESET             0x80  // 10000000
#define PWRMGM_SLEEP              0x40  // 01000000
#define PWRMGM_STBY_XG            0x20  // 00100000
#define PWRMGM_STBY_YG            0x10  // 00010000
#define PWRMGM_STBY_ZG            0x08  // 00001000
#define PWRMGM_CLK_SEL            0x07  // 00000111

/************************************/
/*    REGISTERS PARAMETERS    */
/************************************/
// Sample Rate Divider
#define NOSRDIVIDER         0 // default    FsampleHz=SampleRateHz/(divider+1)
// Gyro Full Scale Range
#define RANGE2000           3   // default
// Digital Low Pass Filter BandWidth and SampleRate
#define BW256_SR8           0   // default    256Khz BW and 8Khz SR
#define BW188_SR1           1
#define BW098_SR1           2
#define BW042_SR1           3
#define BW020_SR1           4
#define BW010_SR1           5
#define BW005_SR1           6
// Interrupt Active logic lvl
#define ACTIVE_ONHIGH       0 // default
#define ACTIVE_ONLOW        1
// Interrupt drive type
#define PUSH_PULL           0 // default
#define OPEN_DRAIN          1
// Interrupt Latch mode
#define PULSE_50US          0 // default
#define UNTIL_INT_CLEARED   1
// Interrupt Latch clear method
#define READ_STATUSREG      0 // default
#define READ_ANYREG         1
// Power management
#define NORMAL              0 // default
#define STANDBY             1
// Clock Source - user parameters
#define INTERNALOSC         0   // default
#define PLL_XGYRO_REF       1
#define PLL_YGYRO_REF       2
#define PLL_ZGYRO_REF       3
#define PLL_EXTERNAL32      4   // 32.768 kHz
#define PLL_EXTERNAL19      5   // 19.2 Mhz

class ITG3200 : public I2CDevice 
{

public:
    int offsets[3];

    ITG3200(bool hl);

    // Gyro initialization
    void init();
    void init(byte _SRateDiv, byte _Range, byte _filterBW, byte _ClockSrc, bool _ITGReady, bool _INTRawDataReady);

    // Sample Rate Divider
    byte getSampleRateDiv();
    void setSampleRateDiv(byte _SampleRate);
    // Digital Low Pass Filter BandWidth and SampleRate
    byte getFSRange();
    void setFSRange(byte _Range); // RANGE2000
    byte getFilterBW();
    void setFilterBW(byte _BW); // see register parameters above
    // Interrupt Configuration
    bool isINTActiveOnLow();
    void setINTLogiclvl(bool _State); //ACTIVE_ONHIGH, ACTIVE_ONLOW
    // Interrupt drive type
    bool isINTOpenDrain();
    void setINTDriveType(bool _State); //OPEN_DRAIN, PUSH_PULL
    // Interrupt Latch mode
    bool isLatchUntilCleared();
    void setLatchMode(bool _State); //UNTIL_INT_CLEARED, PULSE_50US
    // Interrupt Latch clear method
    bool isAnyRegClrMode();
    void setLatchClearMode(bool _State); //READ_ANYREG, READ_STATUSREG
    // INT pin triggers
    bool isITGReadyOn();
    void setITGReady(bool _State);
    bool isRawDataReadyOn();
    void setRawDataReady(bool _State);
    // Trigger Status
    bool isITGReady();
    bool isRawDataReady();
    // Gyro Sensors
    void readTemp(double *_Temp);
    void readGyroRaw( int *_GyroX, int *_GyroY, int *_GyroZ); // uncalibrated raw values
    void readGyroRaw( int *_GyroXYZ); // uncalibrated raw values
    void setOffsets(int _Xoffset, int _Yoffset, int _Zoffset);
    void zeroCalibrate(unsigned int totSamples, unsigned int sampleDelayMS);	// assuming gyroscope is stationary (updates XYZ offsets)
    void readGyroRawCal(int *_GyroX, int *_GyroY, int *_GyroZ); // raw value with offset
    void readGyroRawCal(int *_GyroXYZ); // raw value with offset
    imu::Vector<3> read_gyro(); // deg/sec calibrated & ScaleFactor
    void readGyro(double *_GyroXYZ); // deg/sec calibrated & ScaleFactor
    // Power management
    void reset(); // after reset all registers have default values
    bool isLowPower();
    void setPowerMode(bool _State); // NORMAL, STANDBY
    bool isXgyroStandby();
    bool isYgyroStandby();
    bool isZgyroStandby();
    void setXgyroStandby(bool _Status); // NORMAL, STANDBY
    void setYgyroStandby(bool _Status);
    void setZgyroStandby(bool _Status);
    byte getClockSource();
    void setClockSource(byte _CLKsource); // see register parameters above

};
#endif