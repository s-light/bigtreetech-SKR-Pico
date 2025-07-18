// this usess the library from
// https://github.com/janelia-arduino/MPR121

#include <Arduino.h>
#include <MPR121.h>

#include "Constants.h"

MPR121 mpr121;

void setup() {
    delay(1000);
    Serial.begin(constants::baud);
    while (!Serial);
    Serial.println();
    Serial.println("MPR121-TestTouch.ino");

    Serial.println("setup...");
    Serial.println("setupSingleDevice");
    mpr121.setupSingleDevice(*constants::wire_ptr, constants::device_address,
                             constants::fast_mode);

    Serial.println("setAllChannelsThresholds");
    mpr121.setAllChannelsThresholds(constants::touch_threshold,
                                    constants::release_threshold);
    Serial.println("setDebounce");
    mpr121.setDebounce(constants::device_address, constants::touch_debounce,
                       constants::release_debounce);
    Serial.println("setBaselineTracking");
    mpr121.setBaselineTracking(constants::device_address,
                               constants::baseline_tracking);
    Serial.println("setChargeDischargeCurrent");
    mpr121.setChargeDischargeCurrent(constants::device_address,
                                     constants::charge_discharge_current);
    Serial.println("setChargeDischargeTime");
    mpr121.setChargeDischargeTime(constants::device_address,
                                  constants::charge_discharge_time);
    Serial.println("setFirstFilterIterations");
    mpr121.setFirstFilterIterations(constants::device_address,
                                    constants::first_filter_iterations);
    Serial.println("setSecondFilterIterations");
    mpr121.setSecondFilterIterations(constants::device_address,
                                     constants::second_filter_iterations);
    Serial.println("setSamplePeriod");
    mpr121.setSamplePeriod(constants::device_address, constants::sample_period);

    Serial.println("startChannels");
    mpr121.startChannels(constants::physical_channel_count,
                         constants::proximity_mode);

    Serial.println("entering loop...");
}

void loop() {
    delay(constants::loop_delay);
    if (!mpr121.communicating(constants::device_address)) {
        Serial.println("mpr121 device not commmunicating!\n");
        return;
    }

    uint8_t channel_count = mpr121.getChannelCount();
    Serial.print("channel_count: ");
    Serial.println(channel_count);
    uint8_t running_channel_count = mpr121.getRunningChannelCount();
    Serial.print("running_channel_count: ");
    Serial.println(running_channel_count);

    uint16_t touch_status = mpr121.getTouchStatus(constants::device_address);
    Serial.print("touch_status: ");
    Serial.println(touch_status, BIN);
    if (mpr121.overCurrentDetected(touch_status)) {
        Serial.println("Over current detected!\n");
        mpr121.startChannels(constants::physical_channel_count,
                             constants::proximity_mode);
        return;
    }
    bool any_touched = mpr121.anyTouched(touch_status);
    Serial.print("any_touched: ");
    Serial.println(any_touched);
    bool device_channel_touched =
        mpr121.deviceChannelTouched(touch_status, constants::channel);
    Serial.print("device_channel_touched: ");
    Serial.println(device_channel_touched);

    uint16_t out_of_range_status =
        mpr121.getOutOfRangeStatus(constants::device_address);
    Serial.print("out_of_range_status: ");
    Serial.println(out_of_range_status, BIN);

    bool channel_touched = mpr121.channelTouched(constants::channel);
    Serial.print("channel_touched: ");
    Serial.println(channel_touched);

    uint16_t channel_filtered_data =
        mpr121.getChannelFilteredData(constants::channel);
    Serial.print("channel_filtered_data: ");
    Serial.println(channel_filtered_data);

    uint16_t channel_baseline_data =
        mpr121.getChannelBaselineData(constants::channel);
    Serial.print("channel_baseline_data: ");
    Serial.println(channel_baseline_data);

    Serial.println();
}