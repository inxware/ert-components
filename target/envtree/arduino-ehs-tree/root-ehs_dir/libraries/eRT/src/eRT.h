/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file eRT.h
 * 
 * The API for running eRT in the Arduino environment. This API provides functions 
 * to initialize and execute the eRT system within an Arduino sketch, allowing for
 * real-time operations and task management.
 *
 * @author: inx limited
 *
 */

#ifndef ERT_ARDUIONO_H
#define ERT_ARDUIONO_H

/**
 * Include Arduino dependencies.
 */
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <Arduino_LSM6DS3.h>

/**
 * @brief Initializes and sets up the eRT system.
 * 
 * This function should be called from the Arduino `setup()` to configure 
 * and initialize the eRT system. If you wish to use the debug features, 
 * make sure to initialize Serial before calling this function.
 */
void eRT_setup();

/**
 * @brief Executes the main eRT loop.
 * 
 * This function should be called from the Arduino `loop()` to continuously 
 * run the eRT system. It handles the periodic tasks and real-time operations.
 */
void eRT_loop();

/**
 * @brief Connects to a WiFi network for eRT.
 * 
 * This function attempts to connect the device to a WiFi network using the provided
 * SSID and password. It returns `true` if the connection is successful, and `false`
 * if the connection fails. Ensure that WiFi hardware is initialized before calling 
 * this function.
 * 
 * @param ssid The SSID (name) of the WiFi network.
 * @param password The password for the WiFi network.
 * 
 * @return `true` if the connection is successful, `false` otherwise.
 */
bool eRT_wifi(const char* ssid, const char* password);

#endif // ERT_ARDUIONO_H
