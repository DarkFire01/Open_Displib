/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DISPLIB static library
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include "pdisplib.h"

/*
 *  Load the DXGKNRL.SUS
 */
NTSTATUS
RDDM_LoadDxgkrnl(FILE_OBJECT **FileObject, DEVICE_OBJECT **DeviceObject)
{
    NTSTATUS Status;
    UNICODE_STRING DriverServiceName;
    UNICODE_STRING DriverSourceName;
   // DPRINT1("RDDM_LoadDxgkrnl: Attempting to load DXGKRNL\n");
    RtlInitUnicodeString(&DriverServiceName, L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\DXGKrnl");
    Status = ZwLoadDriver(&DriverServiceName);
    if (Status != STATUS_SUCCESS)
    {
        DPRINT1("Failed to load DXGKrnl From WDDM miniport driver %d\n", Status);
        return Status;
    }
    RtlInitUnicodeString(&DriverSourceName, L"\\Device\\DxgKrnl"); /* Taken from windows Vista->11 registry */

    Status = IoGetDeviceObjectPointer(&DriverSourceName, 0xc0000000,
                                      FileObject, DeviceObject);
 //   DPRINT1("RDDM_LoadDxgkrnl: Sucessfully Loaded DXGKRNL\n");
    return Status;
}


NTSTATUS
RDDM_DeviceIoControlConfiguration(ULONG IoControlCode,
                                  DEVICE_OBJECT *DeviceObject,
                                  PDXGKRNL_INTERFACE DriverInitData)
{
    NTSTATUS Status = 0;
    PIRP Irp;
    IO_STATUS_BLOCK IoStatusBlock = {0};
    KEVENT Event = {0};

    KeInitializeEvent(&Event, NotificationEvent, FALSE);
    Irp = IoBuildDeviceIoControlRequest(0x230047, //TODO: name this, was gathered via windbg (FROM VISTA RTM)
                                        DeviceObject,
                                        NULL,
                                        0,
                                        DriverInitData,
                                        sizeof(DXGKRNL_INTERFACE),
                                        TRUE,
                                        &Event,
                                        &IoStatusBlock);

    Status = IofCallDriver(DeviceObject, Irp);
    if (Status < 0) {
       // DPRINT1("Failed to setup dxgkrnl function pointers: %d\n", Status);
        return Status;
    }

    return STATUS_SUCCESS;
}

VOID
RDDM_SetupIoControlReq(DEVICE_OBJECT *DeviceObject)
{
    NTSTATUS Status;
    DXGKRNL_INTERFACE DriverInitData; //TODO: This isn't correct
    Status = RDDM_DeviceIoControlConfiguration(0x230047, //TODO: name this, was gathered via windbg (FROM VISTA RTM)
                                               DeviceObject,
                                               &DriverInitData);
    //DPRINT1("RDDM_DeviceIoControlConfiguration left with Status %d\n", Status);
    //DPRINT1("Miniport has detected DXGKRNL_INTERFACE size: 0x%X\n",DriverInitData.Size);
    //DPRINT1("Miniport has detected WDDM version 0x%X\n",DriverInitData.Version);
    //DPRINT1("Pointer to first function pointer is %X\n",DriverInitData.DxgkCbEvalAcpiMethod);
}
