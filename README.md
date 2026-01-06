# Adafruit_MPR121_Lick_Detection
- Arduino scripts modified for use with 2 connected Adafruit MPR121 boards

- Modifications were made to improve capacitive spiking for home-cage mouse lick detection

- Python logging script for saving timestamps and all 24 sensor readings

# Current Versions
- v1: This is the original Adafruit MPR121test library modified to have 2 boards and use the Autoconfig feature. The Autoconfig feature enables baseline and sensitivity calibrations.
- v2: This version attempts to use Autoconfig to initially calibrate the setup and then lock those changes by disabling Autoconfig. New touch and release thresholds are manually set. However, this did not seem to improve signal detection.
- v3: This is the currently recommended version of the setup. Chip parameters are frozen and baselineData and filteredData are directly used for capacitance calculations. There is an optional baseline restoration filter that can be used by printing 'licksignal' instead of 'raw'.
