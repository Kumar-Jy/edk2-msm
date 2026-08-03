// driver.h : Goodix WBF (Windows Biometric Framework) sensor driver
//
// NOTES ON STATUS
// ----------------
// This is a KMDF skeleton for the Goodix GF3208 fingerprint sensor on
// Xiaomi POCO F1 (beryllium). On this hardware the GF3208 is wired to the
// Qualcomm gfdip TrustZone app; the kernel driver is only an SPI+IRQ+GPIO
// shim and all biometric compute runs in TEE. Consequently this driver can
// LOAD and enumerate on ACPI\GXFP3208, report sensor attributes to WBF and
// toggle power/irq/reset GPIOs, but genuine capture/match requires either a
// functional SPI transport + GF firmware protocol (not present here) or the
// inbox vendor WBF driver talking to real silicon. Treat it as a compilable
// integration skeleton and starting point.

#ifndef _GOODIXDRIVER_H_
#define _GOODIXDRIVER_H_

#include <ntddk.h>
#include <wdf.h>
#include <wdftypes.h>

//
// WinBio / WBF biometric interface used by WbioSrvc.
//
// NOTE: the SDK's winbio_ioctl.h / winbio_types.h headers pull in
// user-mode/MIDL-only types (RECT/POINT, WINBIO_EXTENDED_*) that do not
// compile in a pure KMDF driver. A WBF sensor driver only needs the device
// interface GUID and the mandatory IOCTL codes, so those are declared here
// with values taken verbatim from winbio_ioctl.h.
//

DEFINE_GUID(GUID_DEVINTERFACE_BIOMETRIC_READER,
            0xe2b5183a, 0x99ea, 0x4cc3, 0xad, 0x6b,
            0x80, 0xca, 0x8d, 0x71, 0x5b, 0x80);
// {E2B5183A-99EA-4CC3-AD6B-80CA8D715B80}

#define BIO_CTL_CODE(code) \
    CTL_CODE(FILE_DEVICE_BIOMETRIC, (code), METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_BIOMETRIC_GET_ATTRIBUTES    BIO_CTL_CODE(0x001)
#define IOCTL_BIOMETRIC_GET_SENSOR_STATUS BIO_CTL_CODE(0x004)
#define IOCTL_BIOMETRIC_CAPTURE_DATA      BIO_CTL_CODE(0x005)

//
// Minimal WINBIO_SENSOR_STATUS values (from winbio_types.h).
//
typedef ULONG WINBIO_SENSOR_STATUS, *PWINBIO_SENSOR_STATUS;
#define WINBIO_SENSOR_NOT_PRESENT   ((WINBIO_SENSOR_STATUS)8) // UNAVAILABLE(path absent)
#define WINBIO_SENSOR_NOT_READY     ((WINBIO_SENSOR_STATUS)0) // UNKNOWN

#define GOODIX_DRIVER_NAME_STR     "GoodixWBF"
#define GOODIX_DEVICE_NAME_STR     "Goodix GFP Fingerprint Sensor"

// ACPI hardware id(s) this driver binds to (see .inf).
// The GF3208 on beryllium is exposed as ACPI\GXFP3208 to match the inbox
// "Goodix Fingerprint SPI Device" hardware id pattern.
#define GOODIX_ACPI_HID L"GXFP3208"

// GPIOs used on the beryllium fingerprint slot, matching the ACPI _CRS
// and the fingerprint_resources.asl PEP power sequence:
//   reset  = GPIO 37  (active low)
//   irq    = GPIO 121 (interrupt)
//   power  = GPIO 94  (enable high)
#define GOODIX_GPIO_RESET   37
#define GOODIX_GPIO_IRQ     121
#define GOODIX_GPIO_PWR     94

// Driver device context (one per physical device stack instance).
typedef struct _GOODIX_FP_CONTEXT {
    // WDF framework device (this device).
    WDFDEVICE  Device;

    // Work / WDF objects
    WDFQUEUE   DefaultQueue;

    // Active after PrepareHardware; cleaned in ReleaseHardware.
    BOOLEAN    _Initialized;

    // Cached SPI connection (from SPBSerialBus resource) - optional.
    ULONG      _SpiControllerBase;
    ULONG      _SpiConnectionSpeed;
    UINT8      _SpiDataBits;

    // WBF session GUID tracked for completion of IOCTLs.
    GUID       _BioSessionId;

    // Sensor status reported to WbioSrvc.
    WINBIO_SENSOR_STATUS _SensorStatus;

    // Tracks power state D0/D3.
    BOOLEAN    _PoweredOn;
} GOODIX_FP_CONTEXT, *PGOODIX_FP_CONTEXT;

// KMDF gets the context size at DriverEntry time.
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(GOODIX_FP_CONTEXT, GoodixGetFpContext)

#endif // _GOODIXDRIVER_H_