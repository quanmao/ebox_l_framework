/**
  ******************************************************************************
  * @file    bsp_ebox.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2018/07/31
  * @brief   硬件相关信息声明
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


/* Includes ------------------------------------------------------------------*/
#include "bsp_ebox.h"

//// 如果不使用object.cpp, 使用下列方式创建IO对象
#if USE_OBJECT == 0
// 如果不使用object.cpp, 使用下列方式创建IO对象
E_GPIO PA0(PA0_ID);
E_GPIO PA1(PA1_ID);
E_GPIO PA2(PA2_ID);
E_GPIO PA3(PA3_ID);
E_GPIO PA4(PA4_ID);
E_GPIO PA5(PA5_ID);
E_GPIO PA6(PA6_ID);
E_GPIO PA7(PA7_ID);
//E_GPIO PA8(PA8_ID);
E_GPIO PA9(PA9_ID);
E_GPIO PA10(PA10_ID);

E_GPIO PA13(PA13_ID);
E_GPIO PA14(PA14_ID);

E_GPIO PB1(PB1_ID);

E_GPIO PF0(PF0_ID);
E_GPIO PF1(PF1_ID);


//E_EXTI BTN(&PA8);
E_UART uart1(UART1);
#endif



