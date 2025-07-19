/**
 ******************************************************************************
 * @file    main.c
 * @author  tommok
 * @version V1.0
 * @date    16-Jul-2025
 * @brief   Default main function.
 ******************************************************************************
 */
#include <Arduino.h>
#include "app_hal.h"
#include "appController.h"


void setup()
{
  
    Serial.begin(115200);
    hal_setup();
    app_controller_init();
    Serial.println("Setup done!");
    // void* ptr1 = malloc(1*1024);
    // void* ptr2 = malloc(1*1024);
    // void* ptr3 = malloc(5*1024);

    // Serial.printf("ptr1 = %08x\r\n", ptr1);
    // Serial.printf("ptr2 = %08x\r\n", ptr2);
    // Serial.printf("ptr3 = %08x\r\n", ptr3);
}

void loop()
{
    hal_loop(); /* Do not use while loop in this function */
    app_controller_run(); /* Run the application controller */
}


