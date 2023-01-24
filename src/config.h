#pragma once
#include <driver/gpio.h>

// https://www.google.com/url?sa=i&url=https%3A%2F%2Fwww.ebyte.com%2Fen%2Fdownpdf.aspx%3Fid%3D165&psig=AOvVaw3tHXV4ZN1hdFaEPRatKi2S&ust=1674672984224000&source=images&cd=vfe&ved=0CBEQjhxqFwoTCMD19JTx4PwCFQAAAAAdAAAAABAk

#define CONFIG_CC1101_CHANNEL 0
#define CONFIG_CC1101_FREQ_433 true
#define CONFIG_CC1101_SPEED_4800 true

#define CONFIG_C1101_RECEIVER false
#define CONFIG_C1101_TRANSMITTER true

#define HOST_ID 1

#define CONFIG_C1101_CSN_GPIO GPIO_NUM_5
#define CONFIG_C1101_SCK_GPIO GPIO_NUM_18
#define CONFIG_C1101_MOSI_GPIO GPIO_NUM_23
#define CONFIG_C1101_MISO_GPIO GPIO_NUM_19
#define CONFIG_C1101_GDO0_GPIO GPIO_NUM_15