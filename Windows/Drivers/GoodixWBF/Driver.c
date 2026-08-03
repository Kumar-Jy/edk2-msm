// Driver.c : KMDF DriverEntry, EvtDriverDeviceAdd, PNP/power callbacks.

#include "GoodWix.h"
#include <wdffdo.h>
#include <wdfrequest.h>
#include <wdfdevice.h>
#include <wdfio.h>

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD       GoodixEvtDeviceAdd;
EVT_WDF_DEVICE_PREPARE_HARDWARE GoodixEvtPrepareHardware;
EVT_WDF_DEVICE_RELEASE_HARDWARE GoodixEvtReleaseHardware;
EVT_WDF_DEVICE_D0_ENTRY         GoodixEvtDeviceD0Entry;
EVT_WDF_DEVICE_D0_EXIT          GoodixEvtDeviceD0Exit;
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL GoodixEvtIoDeviceControl;

//
// IOCTL dispatch prototype lives in bio.c but is declared here so the
// default queue can bind it.
//

NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    )
{
    WDF_DRIVER_CONFIG config;
    WDFDRIVER         driver;
    NTSTATUS          status;

    WDF_DRIVER_CONFIG_INIT(&config, GoodixEvtDeviceAdd);
    config.DriverPoolTag = 'WBFG';

    status = WdfDriverCreate(
        DriverObject,
        RegistryPath,
        WDF_NO_OBJECT_ATTRIBUTES,
        &config,
        &driver);

    if (!NT_SUCCESS(status)) {
        KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
                   "GoodixWBF: WdfDriverCreate failed %!STATUS!\n", status));
    }

    return status;
}

NTSTATUS
GoodixEvtDeviceAdd(
    _In_    WDFDRIVER       Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
{
    NTSTATUS              status;
    PGOODIX_FP_CONTEXT    ctx;
    WDF_OBJECT_ATTRIBUTES attributes;
    WDFDEVICE             device;
    WDF_IO_QUEUE_CONFIG   queueConfig;
    WDFQUEUE              queue;

    UNREFERENCED_PARAMETER(Driver);

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, GOODIX_FP_CONTEXT);
    status = WdfDeviceCreate(&DeviceInit, &attributes, &device);
    if (!NT_SUCCESS(status)) {
        KdPrint(("GoodixWBF: WdfDeviceCreate failed 0x%x\n", status));
        return status;
    }

    ctx = GoodixGetFpContext(device);
    ctx->Device       = device;
    ctx->_Initialized = FALSE;
    ctx->_PoweredOn   = FALSE;
    ctx->_SensorStatus = WINBIO_SENSOR_NOT_PRESENT;

    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchSequential);
    queueConfig.EvtIoDeviceControl = GoodixEvtIoDeviceControl;
    queueConfig.PowerManaged = WdfTrue;

    status = WdfIoQueueCreate(device, &queueConfig,
                              WDF_NO_OBJECT_ATTRIBUTES, &queue);
    if (!NT_SUCCESS(status)) {
        KdPrint(("GoodixWBF: WdfIoQueueCreate failed 0x%x\n", status));
        return status;
    }
    ctx->DefaultQueue = queue;

    return STATUS_SUCCESS;
}

NTSTATUS
GoodixEvtPrepareHardware(
    _In_ WDFDEVICE      Device,
    _In_ WDFCMRESLIST   ResourcesRaw,
    _In_ WDFCMRESLIST   ResourcesTranslated
    )
{
    UNREFERENCED_PARAMETER(Device);
    UNREFERENCED_PARAMETER(ResourcesRaw);
    UNREFERENCED_PARAMETER(ResourcesTranslated);
    return STATUS_SUCCESS;
}

NTSTATUS
GoodixEvtReleaseHardware(
    _In_ WDFDEVICE      Device,
    _In_ WDFCMRESLIST   ResourcesTranslated
    )
{
    UNREFERENCED_PARAMETER(Device);
    UNREFERENCED_PARAMETER(ResourcesTranslated);
    return STATUS_SUCCESS;
}

NTSTATUS
GoodixEvtDeviceD0Entry(
    _In_ WDFDEVICE                Device,
    _In_ WDF_POWER_DEVICE_STATE   PreviousState
    )
{
    PGOODIX_FP_CONTEXT ctx;

    UNREFERENCED_PARAMETER(PreviousState);

    ctx = GoodixGetFpContext(Device);
    ctx->_PoweredOn = TRUE;
    return STATUS_SUCCESS;
}

NTSTATUS
GoodixEvtDeviceD0Exit(
    _In_ WDFDEVICE                Device,
    _In_ WDF_POWER_DEVICE_STATE   NewState
    )
{
    PGOODIX_FP_CONTEXT ctx;

    UNREFERENCED_PARAMETER(NewState);

    ctx = GoodixGetFpContext(Device);
    ctx->_PoweredOn = FALSE;
    return STATUS_SUCCESS;
}