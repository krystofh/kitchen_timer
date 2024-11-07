/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Zephyr-specific libs
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/pwm.h>
// Shell libs
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/version.h>

// Own code
#include "display_driver.h"
#include "timer.h"
#include "event_handler.h"
#include "sound_player.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 20
// Define the alias for the onboard LED
#define STATUS_LED_NODE DT_NODELABEL(status_led)
/*
 * Ensure that an overlay for USB serial has been defined.
 */
BUILD_ASSERT(DT_NODE_HAS_COMPAT(DT_CHOSEN(zephyr_console), zephyr_cdc_acm_uart),
			 "Console device is not ACM CDC UART device");

LOG_MODULE_REGISTER(main, CONFIG_MAIN_LOG_LEVEL); // registers the log level for the module MAIN specified in Kconfig

const struct gpio_dt_spec status_led = GPIO_DT_SPEC_GET(STATUS_LED_NODE, gpios); // onboard LED device
const struct pwm_dt_spec buzzer = PWM_DT_SPEC_GET(DT_ALIAS(buzzer_dev));		 // buzzer for sound

// initialise all LED devices
int init_leds()
{
	int ret;
	if (!gpio_is_ready_dt(&status_led))
	{
		return 1;
	}
	ret = gpio_pin_configure_dt(&status_led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return 1;
	}
	return 0;
}

int main(void)
{
	LOG_INF("Program starting\n"); // example info message

	// Init sound
	init_sound(&buzzer);

	// Init the LED devices in logic 1 state
	if (init_leds())
	{
		LOG_ERR("LEDs could not be initialised!");
	}
	else
	{
		LOG_INF("LED devices initialised properly");
	}

	// Shell config
	const struct device *dev;
	uint32_t dtr = 0;
	dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));
	if (!device_is_ready(dev) || usb_enable(NULL))
	{
		return 0;
	}
	uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
	k_sleep(K_MSEC(100));
	LOG_INF("USB device configured and connected");

	// Button config
	if (init_buttons())
	{
		LOG_ERR("Button init failed!");
	}
	else
	{
		LOG_INF("Button init successful!");
	}

	// Display init
	if (init_display())
	{
		LOG_ERR("Display init failed!");
	}
	else
	{
		LOG_INF("Display init successful!");
	}

	// Wait in this loop for shell commands or process messages
	while (true)
	{
		/* If we have an LED, match its state to the button's. */
		k_msleep(SLEEP_TIME_MS); /* sleep x ms*/
	}
	return 0;
}
