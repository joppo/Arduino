// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>
#include "iot_configs.h"
#include "sample.h"

#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>

DEFINE_ENUM_STRINGS(DEVICE_TWIN_UPDATE_STATE, DEVICE_TWIN_UPDATE_STATE_VALUES);


/*String containing Hostname, Device Id & Device Key in one of the formats:                 */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"                */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessSignature=<device_sas_token>"    */
static const char* connectionString = IOT_CONFIG_CONNECTION_STRING;

static char msgText[1024];
static char propText[1024];
static bool g_continueRunning;
#define DOWORK_LOOP_NUM     100

static void deviceTwinCallback(DEVICE_TWIN_UPDATE_STATE update_state, const unsigned char* payLoad, size_t size, void* userContextCallback)
{
    (void)userContextCallback;

    printf("Device Twin update received (state=%s, size=%u): \r\n", 
        ENUM_TO_STRING(DEVICE_TWIN_UPDATE_STATE, update_state), size);
    for (size_t n = 0; n < size; n++)
    {
        printf("%c", payLoad[n]);
    }
    printf("\r\n");
}

static void reportedStateCallback(int status_code, void* userContextCallback)
{
    (void)userContextCallback;
    printf("Device Twin reported properties update completed with result: %d\r\n", status_code);

    g_continueRunning = false;
}


void iothub_client_sample_device_twin_run(void)
{
    // Result checks are omitted to simplify the sample.
    // Please verify all returns on your production code.    
    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;
    g_continueRunning = true;

    IOTHUB_CLIENT_TRANSPORT_PROVIDER protocol = MQTT_Protocol;

    if (platform_init() != 0)
    {
        (void)printf("Failed to initialize the platform.\r\n");
    }
    else
    {
        if ((iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, protocol)) == NULL)
        {
            (void)printf("ERROR: iotHubClientHandle is NULL!\r\n");
        }
        else
        {
            bool traceOn = true;
            // This json-format reportedState is created as a string for simplicity. In a real application
            // this would likely be done with parson (which the Azure IoT SDK uses) or a similar tool.
            const char* reportedState = "{ 'device_property': 'new_value'}";
            size_t reportedStateSize = strlen(reportedState);

            (void)IoTHubClient_LL_SetOption(iotHubClientHandle, OPTION_LOG_TRACE, &traceOn);

            // Check the return of all API calls when developing your solution. Return checks ommited for sample simplification.

            (void)IoTHubClient_LL_SetDeviceTwinCallback(iotHubClientHandle, deviceTwinCallback, iotHubClientHandle);
            (void)IoTHubClient_LL_SendReportedState(iotHubClientHandle, (const unsigned char*)reportedState, reportedStateSize, reportedStateCallback, iotHubClientHandle);

            do
            {
                IoTHubClient_LL_DoWork(iotHubClientHandle);
                ThreadAPI_Sleep(100);
            } while (g_continueRunning);

            for (size_t index = 0; index < DOWORK_LOOP_NUM; index++)
            {
                IoTHubClient_LL_DoWork(iotHubClientHandle);
                ThreadAPI_Sleep(100);
            }
            LogInfo("IoTHubClient_LL_Destroy starting");

            IoTHubClient_LL_Destroy(iotHubClientHandle);
            LogInfo("IoTHubClient_LL_Destroy okay");
        }
        platform_deinit();
    }
}

void sample_run(void)
{
    iothub_client_sample_device_twin_run();
}
