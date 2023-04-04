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
    PCWSTR DriverNameString = L"DXGKrnl"; /* Taken from windows Vista->11 registry */

    /* Attempt to load DXGKRNL.SYS via the Service string */
    RtlInitUnicodeString(&DriverServiceName, DriverNameString);
    Status = ZwLoadDriver(&DriverServiceName);
    if (Status != STATUS_SUCCESS)
    {
        DPRINT1("Failed to load DXGKrnl From WDDM miniport driver\n");
        return Status;
    }
    RtlInitUnicodeString(&DriverSourceName, L"\\Device\\DxgKrnl"); /* Taken from windows Vista->11 registry */

    Status = IoGetDeviceObjectPointer(&DriverSourceName, 0xc0000000,
                                      FileObject, DeviceObject);
    return Status;
}