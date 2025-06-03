    //
// Created by aweter on 25-6-2.
//

#ifndef I2C_H
#define I2C_H

#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

    /* Includes ------------------------------------------------------------------*/
#include "main.h"

    /* USER CODE BEGIN Includes */

    /* USER CODE END Includes */

    extern I2C_HandleTypeDef hi2c1;

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

    void MX_I2C1_Init(void);

    /* USER CODE BEGIN Prototypes */

    /* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif //I2C_H
