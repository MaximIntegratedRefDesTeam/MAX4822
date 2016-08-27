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

#include "MAX4822.h"

//*********************************************************************
MAX4822::MAX4822()
{
	//empty
}

//*********************************************************************
void MAX4822::begin(uint8_t mosi, uint8_t sclk, uint8_t cs, uint8_t num_devices)
{
	m_mosi = mosi;
	m_sclk = sclk;
	m_cs = cs;
	m_num_devices = num_devices;
	
	pinMode(m_mosi, OUTPUT);
    pinMode(m_sclk, OUTPUT);
    pinMode(m_cs, OUTPUT);
    
    digitalWrite(m_cs, HIGH);
    digitalWrite(m_mosi, LOW);
    digitalWrite(m_sclk, LOW);
	
    if(m_num_devices)
    {
        for(uint8_t idx = 0; idx < m_num_devices; idx++)
        {
            m_relay_data[idx] = 0;
            m_pwr_save_data[idx] = 0;
        }
    }
    else
    {
        m_relay_data[0] = 0;
        m_pwr_save_data[0] = 0;
    }
}

//*********************************************************************
MAX4822::CmdResult MAX4822::set_all_relays(uint8_t set, uint8_t device)
{
	MAX4822::CmdResult result = OpFailure;
	
	if(device <= m_num_devices)
	{
		pinMode(set, OUTPUT);
		digitalWrite(set, LOW);
		delayMicroseconds(1);
		digitalWrite(set, HIGH);
		
		m_relay_data[device] = 0xFF;
		result = MAX4822::Success;
	}
	
	return result;
}

//*********************************************************************
MAX4822::CmdResult MAX4822::reset_all_relays(uint8_t reset, uint8_t device)
{
	MAX4822::CmdResult result = OpFailure;
	
	if(device <= m_num_devices)
	{
		pinMode(reset, OUTPUT);
		digitalWrite(reset, LOW);
		delayMicroseconds(1);
		digitalWrite(reset, HIGH);
		
		m_relay_data[device] = 0;
		result = MAX4822::Success;
	}
	
	return result;
}

//*********************************************************************
MAX4822::CmdResult MAX4822::set_relay(RelayChannel r, bool send_data, uint8_t n)
{
    MAX4822::CmdResult result = OpFailure;
    
    if(n <= m_num_devices)
    {
        m_relay_data[n] |= (1 << (r - 1));
        
        if(send_data)
        {
            uint16_t num_writes = n + 1;
            
            digitalWrite(m_cs, LOW);
            while(num_writes--)
            {
				shiftOut(m_mosi, m_sclk, MSBFIRST, MAX4822::OUTPUT_CNTL_REG);
				shiftOut(m_mosi, m_sclk, MSBFIRST, m_relay_data[num_writes]);
            }
            digitalWrite(m_cs, HIGH);
        }
        
        result = MAX4822::Success;
    }
    
    return result;
}

//*********************************************************************
MAX4822::CmdResult MAX4822::reset_relay(RelayChannel r, bool send_data, uint8_t n)
{
    MAX4822::CmdResult result = OpFailure;
    
    if(n <= m_num_devices)
    {
        m_relay_data[n] &= ~(1 << (r - 1));
        
        if(send_data)
        {
            uint16_t num_writes = n + 1;
            
            digitalWrite(m_cs, LOW);
            while(num_writes--)
            {
                shiftOut(m_mosi, m_sclk, MSBFIRST, MAX4822::OUTPUT_CNTL_REG);
				shiftOut(m_mosi, m_sclk, MSBFIRST, m_relay_data[num_writes]);
            }
            digitalWrite(m_cs, HIGH);
        }
        
        result = MAX4822::Success;
    }
    
    return result;
}

//*********************************************************************
MAX4822::CmdResult MAX4822::set_pwr_save(PowerSave pwr_save, bool send_data, uint8_t n)
{
    MAX4822::CmdResult result = OpFailure;
    
    if(n <= m_num_devices)
    {
        m_pwr_save_data[n] = pwr_save;
        
        if(send_data)
        {
            uint16_t num_writes = n + 1;
            
            digitalWrite(m_cs, LOW);
            while(num_writes--)
            {
				shiftOut(m_mosi, m_sclk, MSBFIRST, MAX4822::POWER_SAVE_REG);
				shiftOut(m_mosi, m_sclk, MSBFIRST, m_pwr_save_data[num_writes]);
            }
            digitalWrite(m_cs, HIGH);
        }
        
        result = MAX4822::Success;
    }
    
    return result;
}
