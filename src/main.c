/* The example of CC1101
 *
 * This sample code is in the public domain.
 */

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include <cc1101.h>

#include "config.h"

#define TAG "MAIN"

#if CONFIG_C1101_RECEIVER
// Get signal strength indicator in dBm.
// See: http://www.ti.com/lit/an/swra114d/swra114d.pdf
int rssi(char raw)
{
	uint8_t rssi_dec;
	// TODO: This rssi_offset is dependent on baud and MHz; this is for 38.4kbps and 433 MHz.
	uint8_t rssi_offset = 74;
	rssi_dec = (uint8_t)raw;
	if (rssi_dec >= 128)
		return ((int)(rssi_dec - 256) / 2) - rssi_offset;
	else
		return (rssi_dec / 2) - rssi_offset;
}

// Get link quality indicator.
int lqi(char raw)
{
	return 0x3F - raw;
}

void rx_task(void *pvParameter)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
	CCPACKET packet;
	while (1)
	{
		if (packet_available())
		{
			if (receiveData(&packet) > 0)
			{
				ESP_LOGI(pcTaskGetName(0), "Received packet...");
				if (!packet.crc_ok)
				{
					ESP_LOGE(pcTaskGetName(0), "crc not ok");
				}
				ESP_LOGI(pcTaskGetName(0), "packet.lqi: %d", lqi(packet.lqi));
				ESP_LOGI(pcTaskGetName(0), "packet.rssi: %ddBm", rssi(packet.rssi));

				if (packet.crc_ok && packet.length > 0)
				{
					ESP_LOGI(pcTaskGetName(0), "packet.length: %d", packet.length);
					ESP_LOGI(pcTaskGetName(0), "data: %s", (const char *)packet.data);
				}
			} // end receiveData
		}		// end packet_available
		vTaskDelay(1);
	} // end while

	// never reach here
	vTaskDelete(NULL);
}
#endif //CONFIG_C1101_RECEIVER

#if CONFIG_C1101_TRANSMITTER
void tx_task(void *pvParameter)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
	char message[64];
	CCPACKET packet;
	while (1)
	{
		sprintf(message, "Hello World %d", xTaskGetTickCount());
		// We also need to include the 0 byte at the end of the string
		packet.length = strlen(message) + 1;
		ESP_LOGD(pcTaskGetName(0), "packet.length=%d", packet.length);
		strncpy((char *)packet.data, message, packet.length);
		sendData(packet);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	} // end while

	// never reach here
	vTaskDelete(NULL);
}
#endif //CONFIG_C1101TRANSMITTER

void app_main()
{
	uint8_t freq;
#if CONFIG_CC1101_FREQ_433
	freq = CFREQ_433;
	ESP_LOGW(TAG, "Set frequency to 433MHz");
#elif CONFIG_CC1101_FREQ_868
	freq = CFREQ_868;
	ESP_LOGW(TAG, "Set frequency to 868MHz");
#elif CONFIG_CC1101_FREQ_915
	freq = CFREQ_915;
	ESP_LOGW(TAG, "Set frequency to 915MHz");
#elif CONFIG_CC1101_FREQ_918
	freq = CFREQ_918;
	ESP_LOGW(TAG, "Set frequency to 918MHz");
#endif

	uint8_t mode;
#if CONFIG_CC1101_SPEED_4800
	mode = MODE_LOW_SPEED;
	ESP_LOGW(TAG, "Set speed to 4800bps");
#elif CONFIG_CC1101_SPEED_38400
	mode = MODE_HIGH_SPEED;
	ESP_LOGW(TAG, "Set speed to 38400bps");
#endif

	// init(CFREQ_433, 0);
	esp_err_t ret = init(freq, mode);
	if (ret != ESP_OK)
	{
		ESP_LOGE(TAG, "CC1101 not installed");
		while (1)
		{
			vTaskDelay(1);
		}
	}

	uint8_t syncWord[2] = {199, 10};
	setSyncWordArray(syncWord);
	ESP_LOGW(TAG, "Set channel to %d", CONFIG_CC1101_CHANNEL);
	setChannel(CONFIG_CC1101_CHANNEL);
	disableAddressCheck();
#if CONFIG_CC1101_POWER_HIGH
	ESP_LOGW(TAG, "Set PA_LongDistance");
	setTxPowerAmp(PA_LongDistance);
#endif

#if CONFIG_C1101_TRANSMITTER
	xTaskCreate(&tx_task, "tx_task", 1024 * 3, NULL, 1, NULL);
#endif
#if CONFIG_C1101_RECEIVER
	xTaskCreate(&rx_task, "rx_task", 1024 * 3, NULL, 1, NULL);
#endif
}
