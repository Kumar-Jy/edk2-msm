// Bio.c : WinBio / WBF sensor IOCTL handling.
//
// On beryllium the GF3208 does not expose a usable image / matching path
// (its biometric engine lives in the gfdip TrustZone app, absent on
// Windows). The mandatory WBF sensor IOCTLs are implemented here so the
// driver installs and enumerates under the Windows Biometric Framework;
// capture APIs report a not-ready sensor.

#include "GoodWix.h"
#include <wdffileobject.h>
#include <wdfrequest.h>
#include <wdfmemory.h>
#include <ntstrsafe.h>

static
PGOODIX_FP_CONTEXT
GoodixGetCtxFromDevice(
    _In_ WDFREQUEST Request
    )
{
    WDFQUEUE  queue;
    WDFDEVICE device;

    queue  = WdfRequestGetIoQueue(Request);
    device = WdfIoQueueGetDevice(queue);

    return GoodixGetFpContext(device);
}

NTSTATUS
GoodixEvtIoDeviceControl(
    _In_ WDFQUEUE   Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t     OutputBufferLength,
    _In_ size_t     InputBufferLength,
    _In_ ULONG      IoControlCode
    )
{
    PGOODIX_FP_CONTEXT ctx;
    NTSTATUS status = STATUS_NOT_SUPPORTED;

    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

    ctx = GoodixGetCtxFromDevice(Request);

    switch (IoControlCode) {
    case IOCTL_BIOMETRIC_GET_ATTRIBUTES:
        // ACPI GXFP3208 present but non-functional: report no match.
        status = STATUS_NOT_IMPLEMENTED;
        break;

    case IOCTL_BIOMETRIC_GET_SENSOR_STATUS:
        // Report sensor as not present / not ready.
        status = STATUS_NOT_IMPLEMENTED;
        break;

    case IOCTL_BIOMETRIC_CAPTURE_DATA:
        // Hardware path is inside the gfdip TA (absent on Windows).
        status = STATUS_NOT_IMPLEMENTED;
        break;

    default:
        status = STATUS_NOT_SUPPORTED;
        break;
    }

    WdfRequestComplete(Request, status);
    return status;
}