/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DISPLIB static library
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include "pdisplib.h"

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

/*
 * UNIMPLEMENTED;
 */
NTSTATUS
NTAPI
DxgkInitialize(
  _In_ PDRIVER_OBJECT              DriverObject,
  _In_ PUNICODE_STRING             RegistryPath,
  _In_ PDRIVER_INITIALIZATION_DATA DriverInitializationData
)
{
    NTSTATUS Status;
    FILE_OBJECT *FileObject;
    DEVICE_OBJECT *DeviceObject;

    /* Setup DXGKRNL and verify parameters */
    Status = RDDM_LoadDxgkrnl(&FileObject, &DeviceObject);
    if (!RegistryPath || !DriverObject)
      return STATUS_INVALID_PARAMETER;

    /*
     * One thing we could do is verify some WDDM version crap but
     * ReactOS isn't going to really care about using some older WDDM miniport
     */


    //TODO: build a IO Contro request


    //TODO: windows seems to unload if the following above fails
    return Status;
}
