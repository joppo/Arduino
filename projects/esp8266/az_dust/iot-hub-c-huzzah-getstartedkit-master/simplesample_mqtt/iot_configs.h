// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef IOT_CONFIGS_H
#define IOT_CONFIGS_H

/**
 * WiFi setup
 */
#define IOT_CONFIG_WIFI_SSID            "M-Tel_D7D0"
#define IOT_CONFIG_WIFI_PASSWORD        "48575443EFD7D02A"

/**
 * Find under Microsoft Azure IoT Suite -> DEVICES -> <your device> -> Device Details and Authentication Keys
 * String containing Hostname, Device Id & Device Key in the format:
 *  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"
 */
#define IOT_CONFIG_CONNECTION_STRING    "HostName=SmolyanskaHub.azure-devices.net;DeviceId=1A-SM-PWM-01C;SharedAccessKey=/9/jiRV6DUHtOGNoNbqUUKqWVbWpRsvqu3mM9zYLUPw="

/**
 * Choose the transport protocol
 */
#define IOT_CONFIG_MQTT                 // uncomment this line for MQTT
// #define IOT_CONFIG_HTTP              // uncomment this line for HTTP

#endif /* IOT_CONFIGS_H */
