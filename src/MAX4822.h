/**********************************************************************
* Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
**********************************************************************/

#ifndef MAX4822_H
#define MAX4822_H

#include "Arduino.h"

/**
* @brief MAX4822 - +3.3V/+5V, 8-Channel, Relay Drivers with Fast Recovery 
* Time and Power-Save Mode
* 
* @details The MAX4822–MAX4825 8-channel relay drivers offer built-in 
* kickback protection and drive +3V/+5V nonlatching or dual-coil-latching 
* relays. Each independent open-drain output features a 2.7O (typ) 
* on-resistance and is guaranteed to sink 70mA (min) of load current. 
* These devices consume less than 300µA (max) quiescent current and have 
* 1µA output off-leakage current. A Zener-kickback-protection circuit 
* significantly reduces recovery time in applications where switching 
* speed is critical.
*/

class MAX4822
{
    public:
    
    static const uint8_t OUTPUT_CNTL_REG = 0;
    
    static const uint8_t POWER_SAVE_REG = 1;
    
    enum RelayChannel
    {
        NONE,
        RLY_1,
        RLY_2,
        RLY_3,
        RLY_4,
        RLY_5,
        RLY_6,
        RLY_7,
        RLY_8
    };
    
    enum PowerSave
    {
        DISABLED,
        SEVENTY_PERCENT_VCC,
        SIXTY_PERCENT_VCC,
        FIFTY_PERCENT_VCC,
        FORTY_PERCENT_VCC,
        THIRTY_PERCENT_VCC,
        TWENTY_PERCENT_VCC,
        TEN_PERCENT_VCC
    };
    
    enum CmdResult
    {
        OpFailure,
        Success
    };
        
    ///@brief MAX4822 default constructor
	MAX4822();
	
    ///@brief begin mbr fx
    ///@param[in] spi_bus - reference to SPI bus for this device
    ///@param[in] cs - Pin connected to chip select of this device
    ///@param[in] num_devices - Number of daisychained devices; defaults to 0.
    void begin(uint8_t mosi = 11, uint8_t sclk = 13, uint8_t cs = 10, uint8_t num_devices = 0);
    
    ///@brief Sets all relays of device connected to set
    ///@param[in] set - Pin connected to SET pin of device
	///@param[in] device - Device number in daisychain mode, defaults to 0
	///@return Result of operation.
    CmdResult set_all_relays(uint8_t set, uint8_t device = 0);
    
    ///@brief Resets all relays of device connected to reset
    ///@param[in] reset - Pin connected to RESET pin of device
	///@param[in] device - Device number in daisychain mode, defaults to 0
	///@return Result of operation.
    CmdResult reset_all_relays(uint8_t reset, uint8_t device = 0);
    
    ///@brief Sets private relay state and sends it if 'send_data' is true
    ///@param[in] r - Relay to set
    ///@param[in] send_data - Default value is true.  
    ///If false, private array is updated appropriately.  This allows the user to
    ///update the data array and then send all data on last update.
    ///@param[in] n - MAX4822 device in daisychained mode; defaults to 0.
    ///@return Result of operation.
    CmdResult set_relay(RelayChannel r, bool send_data = true, uint8_t n = 0);
    
    ///@brief Clears private relay state and sends it if 'send_data' is true
    ///@param[in] r - Relay to reset
    ///@param[in] send_data - Default value is true.  
    ///If false, private array is updated appropriately.  This allows the user to
    ///update the data array and then send all data on last update.
    ///@param[in] n - MAX4822 device in daisychained mode; defaults to 0.
    ///@return Result of operation.
    CmdResult reset_relay(RelayChannel r, bool send_data = true, uint8_t n = 0);
    
    ///@brief Sets private power save value nd sends it if 'send_data' is true
    ///@param[in] pwr_save - Power save value 
    ///@param[in] send_data - Default value is true.  
    ///If false, private array is updated appropriately.  This allows the user to
    ///update the data array and then send all data on last update.
    ///@param[in] n - MAX4822 device in daisychained mode; defaults to 0.
    ///@return Result of operation.
    CmdResult set_pwr_save(PowerSave pwr_save, bool send_data = true, uint8_t n = 0);
    
    private:

    uint8_t m_mosi, m_sclk, m_cs;	
    uint8_t m_num_devices;
    PowerSave pwr_save;
    
    uint8_t m_relay_data[16];
    uint8_t m_pwr_save_data[16];
};

#endif /* MAX4822_H */