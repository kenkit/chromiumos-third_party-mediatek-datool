/*
 * Copyright (c) 2014 MediaTek Inc.
 * Author: Tristan Shieh <tristan.shieh@mediatek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <stdio.h>
#include <endian.h>
#include "tty_usb.h"
#define TGT_TYPE_UNKNOWN   0
#define TGT_TYPE_BROM      1
#define TGT_TYPE_PRELOADER 2

static int _target_type = TGT_TYPE_UNKNOWN;
static int _match_usb_br(usb_ifc_info *ifc)
{
    _target_type = TGT_TYPE_UNKNOWN;
    if(ifc->vid == 0x0E8D && ifc->pid ==0x0003)
    {
        _target_type = TGT_TYPE_BROM;
        return 0;
    }
    return -1;
}

static int _match_usb_pl(usb_ifc_info *ifc)
{
    _target_type = TGT_TYPE_UNKNOWN;
    if((ifc->vid == 0x0E8D && ifc->pid == 0x2000) ||
       (ifc->vid == 0x0E8D && ifc->pid == 0x3000))
    {
        _target_type = TGT_TYPE_PRELOADER;
        return 0;
    }
    return -1;
}

static int _match_usb_auto(usb_ifc_info *ifc)
{
    if(_match_usb_br(ifc)==0)
        return 0;
    if(_match_usb_pl(ifc)==0)
        return 0;
    return -1;
}

tty_usb_handle *tty_usb_open_br(void)
{
    return tty_usb_open(_match_usb_br);
}

tty_usb_handle *tty_usb_open_pl(void)
{
    return tty_usb_open(_match_usb_pl);
}

tty_usb_handle *tty_usb_open_auto(void)
{
    return tty_usb_open(_match_usb_auto);
}

bool tty_usb_is_target_brom(void)
{
    return (_target_type==TGT_TYPE_BROM);
}

void tty_usb_w8(tty_usb_handle *h, uint8_t data)
{
    tty_usb_write(h, &data, 1);
}

uint8_t tty_usb_r8(tty_usb_handle *h)
{
    uint8_t data;
    tty_usb_read(h, &data, 1);
    return data;
}

int tty_usb_w8_echo(tty_usb_handle *h, uint8_t data)
{
    tty_usb_w8(h, data);
    uint8_t r = tty_usb_r8(h);
    if(r!=data) {
	    fprintf(stderr, "Wrong answer: %02x, should be %02x\n", r, data);
	    return -1;
    }
    return 0;
}

void tty_usb_w16(tty_usb_handle *h, uint16_t data)
{
    data = htobe16(data);
    tty_usb_write(h, &data, 2);
}

uint16_t tty_usb_r16(tty_usb_handle *h)
{
    uint16_t data;
    tty_usb_read(h, &data, 2);
    return be16toh(data);
}

int tty_usb_w16_echo(tty_usb_handle *h, uint16_t data)
{
    tty_usb_w16(h, data);
    if(tty_usb_r16(h)!=data) return -1;
    return 0;
}

void tty_usb_w32(tty_usb_handle *h, uint32_t data)
{
    data = htobe32(data);
    tty_usb_write(h, &data, 4);
}

uint32_t tty_usb_r32(tty_usb_handle *h)
{
    uint32_t data;
    tty_usb_read(h, &data, 4);
    return be32toh(data);
}

int tty_usb_w32_echo(tty_usb_handle *h, uint32_t data)
{
    tty_usb_w32(h, data);
    if(tty_usb_r32(h)!=data) return -1;
    return 0;
}

void tty_usb_w64(tty_usb_handle *h, uint64_t data)
{
    data = htobe64(data);
    tty_usb_write(h, &data, 8);
}

uint64_t tty_usb_r64(tty_usb_handle *h)
{
  uint64_t data;
  tty_usb_read(h, &data, 8);
  return be64toh(data);
}

int tty_usb_w64_echo(tty_usb_handle *h, uint64_t data)
{
    tty_usb_w64(h, data);
    if(tty_usb_r64(h)!=data) return -1;
    return 0;
}

