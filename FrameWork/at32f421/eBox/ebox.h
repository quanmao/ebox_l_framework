/**
  ******************************************************************************
  * @file    ebox.h
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EBOX_H
#define __EBOX_H
// 引用外设
#include "ebox_gpio.h"
#include "ebox_base.h"
#include "ebox_uart.h"
//#include "ebox_exti.h"
//#include "ebox_gpios.h"
//#include "ebox_spi.h"
//#include "ebox_adc.h"
//#include "ebox_wdg.h"
//#include "ebox_iflash.h"
//#include "ebox_timer.h"
//#include "ebox_pwm.h"
//#include "ebox_in_capture.h"
//#include "ebox_i2c.h"
//#include "ebox_spi.h"
//#include "ebox_rtc.h"


// 引用库
//#include "vector.h"
//#include "linkedlist.h"
//#include "ebox_print.h"
//#include "WString.h"


#if USE_OBJECT
#include "ebox_object.h"
#else
//#include "bsp_ebox.h"
#endif

#endif

