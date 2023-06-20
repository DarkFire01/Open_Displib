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
 * Eventually we want to be able to read the OS version and use the Win8 Variant
 * We save that for when we understand why the win8+ value is what it is
 */
ULONG
RDDM_FindIoControlCode()
{
    /* For now just return the Vista-7 REGISTER IOCTRL */
    return IOCTL_VIDEO_DDI_FUNC_REGISTER;
}

/* Something failed, unload driver */
VOID
RDDM_UnloadDxgkrnl(_In_ PUNICODE_STRING DxgkrnlServiceName)
{
    RtlInitUnicodeString(DxgkrnlServiceName,
                         L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\DXGKrnl");
    ZwUnloadDriver(DxgkrnlServiceName);
}
