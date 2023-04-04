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

#include <d3dkmddi.h>
#include <d3dkmthk.h>
#include <dispmprt.h>

NTSTATUS
RDDM_LoadDxgkrnl();
