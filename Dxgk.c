/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DISPLIB static library
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include "pdisplib.h"
//#define NDEBUG
#include <debug.h>

/*
 * This should probably be inside of the SDK/LIB/DRIVERS, but I'm keeping it here
 * due to the need of the LDDM headers :).
 */

static PDXGKDDI_START_DEVICE                   PublicStartDevice;
static DXGKRNL_INTERFACE                       PublicDxgkrnlInterface;

/*
 * This internally replaces the StartAdapter routine in the function pointer list
 * passed to DxgkInitialize, This mostly allows us to intercept the call to acquire a public version of the Dxgkrnl
 * interface and break into the debugger before going into a WDDM Miniport.
 */
NTSTATUS
RDDM_StartDevice(
    _In_  const PVOID          MiniportDeviceContext,
    _In_  PDXGK_START_INFO     DxgkStartInfo,
    _In_  PDXGKRNL_INTERFACE   DxgkInterface,
    _Out_ PULONG               NumberOfVideoPresentSources,
    _Out_ PULONG               NumberOfChildren)
{
  /* Copy the DXGKRNL Interface into a public copy */
  RtlCopyMemory(&PublicDxgkrnlInterface, DxgkInterface, sizeof(PublicDxgkrnlInterface));

  DPRINT("RDDM: Starting display miniport driver\n");
  __debugbreak();

  /* Execute The Miniport */
  return PublicStartDevice(MiniportDeviceContext, DxgkStartInfo, DxgkInterface, NumberOfVideoPresentSources, NumberOfChildren);
}

/*
 * Some information:
 * First off, unless this library is totally implemented 100% stable, WDDM drivers
 * compiled with the ReactOS toolchain will never start. This is because
 * as far as i can tell the managment of DXGKRNL is mostly done within this library
 * Starting the driver, prepping function call backs to pass to dxgkrnl are all done here.
 *
 * this also means that DXGKRNL as a service doesn't start unless a driver has invoked it
 * as of this recent commit this behavior is now true on ReactOS as well :).
 */
NTSTATUS
NTAPI
DxgkInitialize(
  _In_ PDRIVER_OBJECT              DriverObject,
  _In_ PUNICODE_STRING             RegistryPath,
  _In_ PDRIVER_INITIALIZATION_DATA DriverInitializationData)
{
    /* This is internal and gets filled out VIA a IOCTRL - Thank you Vista RTM Symbols */
    NTSTATUS (NTAPI *DpiInitialize)(PDRIVER_OBJECT, PUNICODE_STRING, PDRIVER_INITIALIZATION_DATA);
    DRIVER_INITIALIZATION_DATA *LocDriverInitializationData;
    UNICODE_STRING DxgkrnlServiceName;
    DEVICE_OBJECT *DxgkrnlDeviceObject;
    IO_STATUS_BLOCK IoStatusBlock;
    UNICODE_STRING DeviceName;
    NTSTATUS Status;
    KEVENT Event;
    IRP *Irp;

    DxgkrnlDeviceObject = 0;
    DpiInitialize = NULL;
    DPRINT("DxgkInitialize: Starting a RDDM Driver\n");
    if (!DriverObject,
        !RegistryPath)
    {
        DPRINT("DriverObject or RegistryPath is NULL\n");
        return STATUS_INVALID_PARAMETER;
    }

    /* Later in this function we make changes to this structure, so let's keep a copy of the unmodified one */
    LocDriverInitializationData = DriverInitializationData;
    if (DriverInitializationData)
    {

        /* It appears Windows will actually fail if the Miniport is below a specific version - we don't care */
        DPRINT("RDDM: This WDDM Miniport version is %X", DriverInitializationData->Version);

        /* First load DXGKrnl itself */
        RtlInitUnicodeString(&DxgkrnlServiceName, L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\DXGKrnl");
        Status = ZwLoadDriver(&DxgkrnlServiceName);
        if (Status == STATUS_SUCCESS)
        {
            /* Okay we suceeded, Go ahead and grab the DxgkrnlDeviceObject */
            RtlInitUnicodeString(&DeviceName, L"\\Device\\DxgKrnl");
            Status = IoGetDeviceObjectPointer(&DeviceName,
                                              FILE_ALL_ACCESS,
                                              (PFILE_OBJECT*)&DriverInitializationData,
                                              &DxgkrnlDeviceObject);
            if (Status != STATUS_SUCCESS)
            {
                RDDM_UnloadDxgkrnl(&DxgkrnlServiceName);
                return Status;
            }
            /* Grab a function pointer to DpiInitialize via IOCTRL */
            KeInitializeEvent(&Event, NotificationEvent, 0);
            Irp = IoBuildDeviceIoControlRequest(RDDM_FindIoControlCode(),
                                                DxgkrnlDeviceObject,
                                                NULL,
                                                0,
                                                &DpiInitialize,
                                                sizeof(DpiInitialize),
                                                TRUE,
                                                &Event,
                                                &IoStatusBlock);

            /* Can't continue without being able to call this routine */
            if (!Irp)
            {
              Status = STATUS_INSUFFICIENT_RESOURCES;
              /* So fail */
              RDDM_UnloadDxgkrnl(&DxgkrnlServiceName);
              return Status;
            }

            Status = IofCallDriver(DxgkrnlDeviceObject, Irp);
        }

        /* Execute the thing */
        if ( Status != STATUS_SUCCESS)
        {
            RDDM_UnloadDxgkrnl(&DxgkrnlServiceName);
        }
        else
        {
          DPRINT1("DISPLIB HAS BEEN SETUP\n");
          __debugbreak();
          /* Intercept the StartDevice Miniport routine so we can grab a public version of the Dxgkrnl interface */
          PublicStartDevice = LocDriverInitializationData->DxgkDdiStartDevice;
          LocDriverInitializationData->DxgkDdiStartDevice = (PDXGKDDI_START_DEVICE)RDDM_StartDevice;
          Status = DpiInitialize(DriverObject, RegistryPath, LocDriverInitializationData);
        }
        return Status;
    }
    return STATUS_INVALID_PARAMETER;
}