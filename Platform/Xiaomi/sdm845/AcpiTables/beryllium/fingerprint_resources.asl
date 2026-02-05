Scope (\_SB.PEP0)
{
	Method (FPMX, 0, NotSerialized)
	{
		Return (FPXC) /* \_SB_.PEP0.FPXC */
	}

	Name (FPXC, Package (0x01)
	{
		Package (0x04)
		{
			"DEVICE", 
			"\\_SB.FPRT", 
			Package (0x08)
			{ // beryllium uses an ic with enable pin to control power to fp, while yoga shuts and turns on a regulator
				/*"DSTATE", 
				Zero, 
				Package (0x02)
				{
					"PMICVREGVOTE", 
					Package (0x06)
					{
						"PPP_RESOURCE_ID_LDO19_A", 
						One, 				// Voltage Regulator Type, 1 = LDO
						0x002F4D60, 	// Voltage (uV)
						One, 				// Enable
						0x07, 				// Power Mode
						Zero 				// Headroom
					}
				}, */

				Package (0x02)//enable
				{
					"TLMMGPIO", 
					Package (0x06)
					{
						94, 
						One, 
						Zero, 
						One, 
						0x03, 
						0x07
					}
				}, 

				Package (0x02)
				{
					"DELAY", 
					Package (0x01)
					{
						0x0A
					}
				}, 

				Package (0x02)
				{
					"TLMMGPIO", 
					Package (0x06)
					{
						121,  	// TLMM GPIO
						One, 	// State 					: 1 = HIGH
						Zero, 	// Function Select  	:  0 = ??
						One,  	// Direction       		:   0 = INPUT
						Zero, 	// Pull Type       		:   1 = PULL_DOWN
						Zero 	// Drive Strength  	:   0 = 2mA
					}
				}, 

				Package (0x02)
				{
					"TLMMGPIO", 
					Package (0x06)
					{
						37, 
						One, 
						Zero, 
						One, 
						Zero, 
						Zero
					}
				}, 

				Package (0x02)
				{
					"DELAY", 
					Package (0x01)
					{
						One
					}
				}, 

				Package (0x02)
				{
					"TLMMGPIO", 
					Package (0x06)
					{
						121, 
						One, 
						Zero, 
						One, 
						One, 
						Zero
					}
				}
			}, 

			Package (0x03)
			{
				/*"DSTATE", 
				0x03, 
				Package (0x02)
				{
					"PMICVREGVOTE", 
					Package (0x06)
					{
						"PPP_RESOURCE_ID_LDO19_A", 
						One, 
						Zero, 
						Zero, 
						Zero, 
						Zero
					}
				}*/
				
				Package (0x02)//disable
				{
					"TLMMGPIO", 
					Package (0x06)
					{
						94, 
						Zero, 
						Zero, 
						Zero, 
						One, 
						Zero
					}
				}, 
				
			}
		}
	})
}