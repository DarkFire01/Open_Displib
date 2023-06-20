/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DISPLIB static library windows 8+
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include "pdisplib.h"
//#define NDEBUG
#include <debug.h>

#if 0
typedef struct PKMDDOD_INITIALIZATION_DATA;
/*
 * TODO: Implement windows 8+ style initalization for
 * WDDM miniport drivers.
 */
NTSTATUS
NTAPI
DxgkInitializeDisplayOnlyDriver(
  _In_ PDRIVER_OBJECT              DriverObject,
  _In_ PUNICODE_STRING             RegistryPath,
  _In_ PKMDDOD_INITIALIZATION_DATA KmdDodInitializationData);

#if 0
Sometime after windows 7 MS decided to create a new way for initializing WDDM
miniport drivers. this "new version" is for slightly different purposes.
However we may come across a time this is a nessesary evil.

One of the types of modern "init":

DriverEntry(
    // Initialize DDI function pointers and dxgkrnl
    KMDDOD_INITIALIZATION_DATA InitialData = {0};

    InitialData.Version = DXGKDDI_INTERFACE_VERSION;

  -> [New structure for setting up callbacks]
    InitialData.DxgkDdiAddDevice                    = BddDdiAddDevice;
    InitialData.DxgkDdiStartDevice...

    DxgkInitializeDisplayOnlyDriver(pDriverObject, pRegistryPath, &InitialData);

#endif

NTSTATUS
NTAPI
DxgkInitializeDisplayOnlyDriver(
  _In_ PDRIVER_OBJECT              DriverObject,
  _In_ PUNICODE_STRING             RegistryPath,
  _In_ PKMDDOD_INITIALIZATION_DATA KmdDodInitializationData)
{
    return 1;
}
#endif
/* Note two: */
/* During the setup of Windows 8+ there's a new IOCTRL that we're not exactly sure what it's name is */
/*
 * This value does a similar goal of IOCTL_VIDEO_DDI_FUNC_REGISTER but i haven't tried to figure out what
 * it's name etc should be yet and behaviors. Will do this when i implement the windows 8 Startup sequence.
 * 0x230047u <-
 * for more information see this webpage
 * https://win8displayarchitecture.blogspot.com/2012/02/win8developerpreviewdisplayarchitecture.html
 */