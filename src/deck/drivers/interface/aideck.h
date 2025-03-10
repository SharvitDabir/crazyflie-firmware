/**
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2021 Bitcraze AB
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * aideck.h: AI-deck/CPX interface
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

#define ESP_BITSTREAM_SIZE 610576
#define AIDECK_UART_TRANSPORT_MTU (100)

// This enum is used to identify source and destination for CPX routing information
typedef enum {
  CPX_T_STM32 = 1, // The STM in the Crazyflie
  CPX_T_ESP32 = 2, // The ESP on the AI-deck
  CPX_T_HOST = 3,  // A remote computer connected via Wifi
  CPX_T_GAP8 = 4   // The GAP8 on the AI-deck
} CPXTarget_t;

typedef enum {
  CPX_F_SYSTEM = 1,
  CPX_F_CONSOLE = 2,
  CPX_F_CRTP = 3,
  CPX_F_WIFI_CTRL = 4,
  CPX_F_APP = 5,
  CPX_F_TEST = 0x0E,
  CPX_F_BOOTLOADER = 0x0F,
} CPXFunction_t;

typedef struct {
  CPXTarget_t destination;
  CPXTarget_t source;
  bool lastPacket;
  CPXFunction_t function;
} CPXRouting_t;

typedef struct {
  CPXRouting_t route;
  uint16_t dataLength;
  uint8_t data[AIDECK_UART_TRANSPORT_MTU-2];
} CPXPacket_t;

/**
 * @brief Receive a CPX packet from the ESP32
 *
 * This function will block until a packet is availale from CPX. The
 * function will return all packets routed to the STM32.
 *
 * @param packet received packet will be stored here
 */
void cpxReceivePacketBlocking(CPXPacket_t * packet);

/**
 * @brief Send a CPX packet to the ESP32
 *
 * This will send a packet to the ESP32 to be routed using CPX. This
 * will block until the packet can be queued up for sending.
 *
 * @param packet packet to be sent
 */
void cpxSendPacketBlocking(const CPXPacket_t * packet);

/**
 * @brief Send a CPX packet to the ESP32
 *
 * This will send a packet to the ESP32 to be routed using CPX.
 *
 * @param packet packet to be sent
 * @param timeoutInMS timeout before giving up if packet cannot be queued
 * @return true if package could be queued for sending
 * @return false if package could not be queued for sending within timeout
 */
bool cpxSendPacket(const CPXPacket_t * packet, uint32_t timeoutInMS);

/**
 * @brief Initialize CPX routing data.
 *
 * Initialize values and set lastPacket to true.
 *
 * @param source The starting point of the packet
 * @param destination The destination to send the packet to
 * @param function The function of the content
 * @param route Pointer to the route data to initialize
 */
void cpxInitRoute(const CPXTarget_t source, const CPXTarget_t destination, const CPXFunction_t function, CPXRouting_t* route);
