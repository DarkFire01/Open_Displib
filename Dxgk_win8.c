/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DISPLIB static library windows 8+
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include "pdisplib.h"


#if 0
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

/*
 * Sometime after windows 7 MS decided to create a new way for initializing WDDM
 * miniport drivers (Though only for kernel mode!). this "new version" is for slightly
 * different purposes.
 * However we may come across a time this is a nessesary evil.
 */

/*
 * UNIMPLEMENTED;
 */
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