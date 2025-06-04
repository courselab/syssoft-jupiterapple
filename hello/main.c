/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2025 TeOSobrino <teosobrinogit@gmail.com>
 *
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
 *  and includes modifications made by the following author(s):
 *  TeOSobrino <teosobrinogit@gmail.com>
 */

#include "stdio.h"
void __satck_chk_fail(void)
{
    return;
}

int main(void)
{
  printf ("Hello World\n\r");
  int time = get_time();
  int hours = (time >> 8); //hours in BCD format
  hours = ((hours >> 4) * 10) + (hours  & 0x0F);

  int minutes = (time & 0x0FF); //minutes in BCD format
  minutes = ((minutes >> 4) * 10) + (minutes  & 0x0F);
  char hour[7];
  hour[0] = (hours/10) + '0';
  hour[1] = (hours%10) + '0';
  hour[2] = ':';
  hour[3] = (minutes/10) + '0';
  hour[4] = (minutes%10) + '0';
  hour[5] = 0x00;
  hour[6] = '\0';

  printf("BIOS time: ");
  printf(hour);

  return 0;
}





