//--------------------------------------------------------------------------------------------------
// TEST Audio
//--------------------------------------------------------------------------------------------------
Scope (\_SB.ADSP.SLM1.ADCM.AUDD)
{
    Device(AFT1){
        Name (_HID, "AFLT0001")
        Name (_UID, Zero)  // _UID: Unique ID
        Name (_DEP, Package ()  // _DEP: Dependencies
        {
            \_SB.ADSP.SLM1.ADCM.AUDD.SPK1,
            \_SB.ADSP.SLM1.ADCM.AUDD
        })
    }
    
    Device (SPK1)
    {
        Name (_HID, "TTAS2559") // Taxes TAS2559
        Name (_UID, 0)
        Alias(\_SB.PSUB, _SUB)
        Name (_DEP, Package()
        {
            \_SB.GIO0, 
            \_SB.I2C6
        })
        Method (_CRS, 0x0, NotSerialized)
        {
            Name (RBUF, ResourceTemplate()
            {
                I2CSerialBus(0x4C, ,400000, AddressingMode7Bit, "\\_SB.I2C6",,,,) // Must be TAS2559 Here
                I2CSerialBus(0x4D, ,400000, AddressingMode7Bit, "\\_SB.I2C6",,,,) // Must be 2560
                GpioInt(Level, ActiveHigh, Exclusive, PullDown, 0, "\\_SB.GIO0") {11} // 2559
                GpioInt(Level, ActiveHigh, Exclusive, PullDown, 0, "\\_SB.GIO0") {30} // 2560
            })
            Return (RBUF)
        }
    }
}
