/* SLG46824 Interface Driver Example
 * Copyright (c) 2018 Avnet Silica 
 *
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SLG46824_driver.h"

#define REGISTER_CONFIG		0x00
#define NVM_CONFIG 				0x04
#define PAGE_WRITE 				0x01

#define ERASE_REGISTER		0xE3

//////////////////////////////////////////////////
// Store nvmData in ascii format
//////////////////////////////////////////////////
const char nvmString0[]  = "05000086230000000000000000000000";
const char nvmString1[]  = "00000000000000000000000000000000";
const char nvmString2[]  = "00000000000000000000000000000000";
const char nvmString3[]  = "00003400B0000000000CF4FCC00F00C4";
const char nvmString4[]  = "0F000000000000000000000000000000";
const char nvmString5[]  = "00000000000000000000000000000000";
const char nvmString6[]  = "00802000800020600000808808000030";
const char nvmString7[]  = "00305050000000000000000000000000";
const char nvmString8[]  = "00000000001422300C00000000000000";
const char nvmString9[]  = "85050000000000000000000000000000";
const char nvmString10[] = "00000020000100000002010000020001";
const char nvmString11[] = "00000201000002000100000201000002";
const char nvmString12[] = "00010000020001000000030101000000";
const char nvmString13[] = "00000000000000000000000000000000";
const char nvmString14[] = "00000000000000000000000000000000";
const char nvmString15[] = "000000000000000000000000000000A5";

// nvmString array
const char* const nvmString[16] = 
{
  nvmString0,
  nvmString1,
  nvmString2,
  nvmString3,
  nvmString4,
  nvmString5,
  nvmString6,
  nvmString7,
  nvmString8,
  nvmString9,
  nvmString10,
  nvmString11,
  nvmString12,
  nvmString13,
  nvmString14,
  nvmString15
};

//array used for NVM programming
uint8_t data_array[16][16];
uint8_t readed_data[16][16];

#define hex_to_ascii(a)        ((a&0x0f) >= 0x0a ? ((a&0x0f) - 0x0a)+'A' : (a&0x0f)+'0')
#define ascii_to_hex(a)        ((a) >= 'A' ? ((a) - 'A')+10 : ((a)-'0'))

uint8_t slg_device_i2c_address =0x10;
uint8_t aRxBuffer[32];
uint8_t	aTxBuffer[32];
uint32_t i2c_err = 0;

extern  Serial pc;			//(SERIAL_TX, SERIAL_RX); 

SLG46824::SLG46824(PinName sda, PinName scl)
	: _slg_i2c(sda, scl)
{

    //_i2c.frequency(400000);  //increase the speed

}


static void delay_ms(uint32_t del)
{
	do{
			wait_ms(1);
	}while (del--);
	
}

bool SLG46824::startup(void)
{
	int result;
	int i;
	
	for (i=0; i<16; i++)
	{
			aTxBuffer[0] = (i<<4);
			result = _slg_i2c.write((int)(slg_device_i2c_address | NVM_CONFIG), (char *)aTxBuffer, (1), true); 
			result |= _slg_i2c.read((int)(slg_device_i2c_address | NVM_CONFIG), (char *)aRxBuffer, (16)); 
			memcpy(&readed_data[i][0], &aRxBuffer[0], 16);
			delay_ms(2);
	}
  return true;
	
}

bool SLG46824::hw_set(void)
{
	
	int i, j;
	int HexVal, result, addr;
	char val1, val2;
	
	char *ptr;
	
	//SLG46824 already programmed, exit. If user would to try to reprogram, 
	//must comment the two source code lines here below
	if (slg_device_i2c_address != 0x10)
		return false;		
	
	//addr = (slg_device_i2c_address | NVM_CONFIG);
	
  // Store each byte value into data_array[][]
	for (i = 0; i < 16; i++)
		{
			ptr = (char *) (nvmString[i]);
			for (j = 0; j < 16; j++)
			{
				val1 = *ptr;
				ptr++;
				val2 = *ptr;
				ptr++;
				HexVal = (ascii_to_hex(val1)<<4) | (ascii_to_hex(val2))<<0;
				data_array[i][j] = (uint8_t)HexVal;
			}
		}
		
  pc.printf("\r\nConfiguring SLG device, wait...");	

	#if 0		//use erase only if you nedd to reprogram device
	//slg46824 erase: the pages erase addresses are on REGISTER area (block 0); please make erase NOT in raw mode but sequentially as sample here below
	pc.printf("\r\nerasing SLG device, wait...");	
	addr = (slg_device_i2c_address);
	for (i = 0; i < 14; i++) 	//address over 0xE0 do not erase (0xE0 protection page, 0xF0 service page)
	{
			//erase page
			aTxBuffer[0] = ERASE_REGISTER;
			aTxBuffer[1] = (i | 0x80);
			result = _slg_i2c.write(addr, (char *)aTxBuffer, (2), false); 
			delay_ms(50);
			if(result != HAL_OK)
					i2c_err++;		
	}
	#endif

	//slg46824 programming
  pc.printf("\r\nProgramming SLG device, wait...");	
		//addr = (slg_device_i2c_address);
	addr = (slg_device_i2c_address | NVM_CONFIG);
	for (i = 0; i < 14; i++) 	//address over 0xE0 not programmed (see isp_guide_slg46824_26_rev.0.1.pdf, figure 3 page 5)
	{	
			//write page
		 	memcpy(&aTxBuffer[1], data_array[i], 16);		
			aTxBuffer[0] = (i<<4);
			result |= _slg_i2c.write(addr, (char *)aTxBuffer, (16+1), false); 
			delay_ms(50);
			if(result != HAL_OK)
					i2c_err++;
	}

	pc.printf("\r\nSLG programmed, now starting application\r\n\n");	

	return true;
}


bool SLG46824::getI2CAddress(uint8_t *addr)
{
		char dummy = 0;
		int i, res;
		
		for (i=0; i<16; i++)
		{
			res = _slg_i2c.read((i<<4), &dummy, 1);  // select the register, no I2C Stop
			if (res == 0)
			{
				*addr = i;
				slg_device_i2c_address = (i<<4);
				wait_ms(50);
				return true;
			}
			//i2c.read(addr, readdata, 8);        // read the length byte and the 7 databytes
			wait_ms(50);
		}

    return false;
}

void SLG46824::test_task(void)
{
	/*
	int result;
  while (1)
  {
			delay_ms(100);
			aTxBuffer[0] = 0x7a;
			aTxBuffer[1] = 0xff;
			result = _slg_i2c.write((int)(slg_device_i2c_address| REGISTER_CONFIG), (char *)aTxBuffer, (2), false); 
			//result = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(slave_address | REGISTER_CONFIG), (uint8_t *)aTxBuffer, 2, 1000);
			if(result != HAL_OK)
					i2c_err++;		
			delay_ms(200);
			aTxBuffer[1] = 0x00;
			//result = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(slave_address | REGISTER_CONFIG), (uint8_t *)aTxBuffer, 2, 1000);
			result = _slg_i2c.write((int)(slg_device_i2c_address| REGISTER_CONFIG), (char *)aTxBuffer, (2), false); 
			if(result != HAL_OK)
					i2c_err++;

  }
	*/

}

