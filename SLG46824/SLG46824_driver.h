/* SPWFInterface Example
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#ifndef SLG46824_H
#define SLG46824_H

#include "mbed.h"
 
/** SLG46824 Interface class.
    This is an interface to a SLG46824 module.
 */
class SLG46824
{
public:
    
	 SLG46824(PinName sda, PinName scl);
    
    bool startup(void);
    
    bool hw_set(void);

    bool getI2CAddress(uint8_t *addr);

		void test_task(void);

private:
		I2C _slg_i2c;

};
 
#endif  //SLG46824_H

