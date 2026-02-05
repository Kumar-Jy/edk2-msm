Include ("fingerprint_resources.asl")

Device (FPRT)
{
	Name (_HID, "GXFP55A4")  // _HID: Hardware ID
	Name (_CID, "GXFP55A4")  // _CID: Compatible ID
	Name (_UID, "GXFP55A4")  // _UID: Unique ID
	Name (_DEP, Package (3)  // _DEP: Dependencies
	{
		\_SB.GIO0, 
		\_SB.PEP0, 
		\_SB.TREE
	})
	Method (_CRS, 0, Serialized)  // _CRS: Current Resource Settings
	{
		Name (BBUF, ResourceTemplate ()
		{
			GpioIo (Exclusive, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GIO0", 0, ResourceConsumer, , ) { 37 }
			GpioInt (Edge, ActiveHigh, ExclusiveAndWake, PullNone, 0, "\\_SB.GIO0", 0, ResourceConsumer, , ) { 121 }
		})
		Return (BBUF) /* \_SB_.FPRT._CRS.BBUF */
	}

	Name (PGID, Buffer (10)
	{
		"\\_SB.FPRT"
	})
	Name (DBUF, Buffer (DBFL){})
	CreateByteField (DBUF, 0, STAT)
	CreateByteField (DBUF, 2, DVAL)
	CreateField (DBUF, 24, 160, DEID)
	Method (_S1D, 0, NotSerialized)  // _S1D: S1 Device State
	{
		Return (0x03)
	}

	Method (_S2D, 0, NotSerialized)  // _S2D: S2 Device State
	{
		Return (0x03)
	}

	Method (_S3D, 0, NotSerialized)  // _S3D: S3 Device State
	{
		Return (0x03)
	}

	Method (_PS0, 0, NotSerialized)  // _PS0: Power State 0
	{
		DEID = Buffer (ESNL){}
		DVAL = 0
		DEID = PGID /* \_SB_.FPRT.PGID */
		If (\_SB.ABD.AVBL)
		{
			\_SB.PEP0.FLD0 = DBUF /* \_SB_.FPRT.DBUF */
		}
	}

	Method (_PS3, 0, NotSerialized)  // _PS3: Power State 3
	{
		DEID = Buffer (ESNL){}
		DVAL = 3
		DEID = PGID /* \_SB_.FPRT.PGID */
		If (\_SB.ABD.AVBL)
		{
			\_SB.PEP0.FLD0 = DBUF /* \_SB_.FPRT.DBUF */
		}
	}
}