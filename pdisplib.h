/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DISPLIB static library
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include <ntddk.h>
#include <windef.h>
#include <winerror.h>
#include <stdio.h>
#include <locd3d.h>
#include <wdm.h>

/* Create an IO request to fill out the function pointer list */
#define IOCTL_VIDEO_DDI_FUNC_REGISTER \
	CTL_CODE( FILE_DEVICE_VIDEO, 0xF, METHOD_NEITHER, FILE_ANY_ACCESS  )

ULONG
RDDM_FindIoControlCode();

VOID
RDDM_UnloadDxgkrnl(_In_ PUNICODE_STRING DxgkrnlServiceName);