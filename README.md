# ESP32-S3 Yet Another Sketchpad

A demo project for ESP32-S3 using LVGL to implement a sketchpad on a 320 x 240 TFT display.

This is a sibling project to [YetAnotherStockPriceTracker](https://github.com/tommokmok/esp32s3_YetAnotherStockPricceTracker) and uses the same hardware.

Almost 90% of the code is referenced from [lv_100ask_sketchpad](https://github.com/100askTeam/lv_lib_100ask/tree/master/src/lv_100ask_sketchpad), but updated for LVGL v9.2.2 and fixed for compatibility with ESP32-S3.

## Project Core Values

- For learning purposes.
- The implementation is kept simple and straightforward.
- No fancy stuff,  make it work.

## How This Project May Help You

- If you want LVGL to use external PSRAM for large buffers, add the following to your `platformio.ini`. This lets LVGL use the standard C memory allocation API. When the allocated memory size is >4k, it will automatically use external PSRAM by default. This works with LVGL v9.2.2 and PlatformIO ESP32 Arduino framework (v2.0.5?).

    ```c
    -D 'LV_USE_STDLIB_MALLOC=LV_STDLIB_CLIB'
    ```

- Espressif and PlatformIO are no longer collaborating, so PlatformIO does not support ESP32 Arduino framework versions >v2.0.xx. If you want to use the latest ESP32 Arduino framework (>v3), please use the Arduino IDE or the PIO Arduino VS Code extension.

    > Reference: [Using ESP-IDF v5.x with Arduino on ESP32S3](https://community.platformio.org/t/using-esp-idf-v5-x-with-arduino-on-esp32s3/45597)

## Screenshots & Demo

<a href="https://ibb.co/LXDT5wT2"><img src="https://i.ibb.co/LXDT5wT2/sketchpad.gif" alt="sketchpad" border="0"></a>

## Features

- Simple sketchpad to draw on the screen
- Button to clear the screen
- Button to change the line width

## Development Timeline

| Date       | Milestone/Note   |
|------------|------------------|
| 2025-07-16 | Project started  |
| 2025-07-21 | v1 release       |


## Getting Started

### Prerequisites

- VS Code with the [PlatformIO](https://platformio.org/) extension
- ESP32-S3 board + 2.8" TFT LCD
    - Hardware purchased from Taobao. More info: [ESP32-LVGL开发板](http://wiki.waaax.cn/boards/ESP32/ESP32-LVGL%E5%BC%80%E5%8F%91%E6%9D%BF.html)
    > **Note:** This dev board is not recommended due to hardware issues. It may reset automatically, especially when connected to a PC.
    - ESP32-S3-N8R8
    - LCD Driver: ST7989, 320 x 240 pixels
    - Touch Driver: XPT2046

### Building & Flashing

1. Clone this repository:
    ```sh
    git clone https://github.com/tommokmok/esp32s3_YetAnotherSketchpad
    ```

2. Open the project in PlatformIO (VS Code).

3. Build and upload to your ESP32-S3 board.

## File Structure

All libraries are placed in the local `lib/` directory for convenience.

**Pros:**
- No need to download libraries from the web when opening the project for the first time.
- You can modify the source if you need specific functions.

**Cons:**
- Larger repository size.

```
esp32s3_YetAnotherSketchpad/

├── lib/                            # External and user libraries
│   ├── lvgl/                       # LVGL graphics library (if added locally)
│   ├── LovyanGFX/                  # LovyanGFX display library (if added locally)
│   ├── hal/                        # Hardware Abstraction Layer (display, touch, etc.)
│   ├── appController/              # Application controller logic (state management, UI flow)
│   └── sketchpad/                  # Sketchpad implementation (drawing logic, UI components)
├── src/                            # Source code
│   ├── main.cpp                    # Main application entry point
│   └── ...                         # Other source files
├── platformio.ini                  # PlatformIO project configuration
├── README.md                       # This documentation

```

- **lib/lvgl/**: LVGL graphics library (added locally).
- **lib/LovyanGFX/**: LovyanGFX display library (added locally).
- **lib/hal/**: Hardware Abstraction Layer for display, touch, and other peripherals.
- **lib/appController/**: Application controller logic, such as state management and UI flow.
- **lib/sketchpad/**: Sketchpad UI implementation.
- **src/main.cpp**: Main application logic, including LVGL initialization and sketchpad setup.

> **Note:** If you install libraries via the PlatformIO Library Manager, they will be managed automatically. If you add libraries manually, place them in the `lib/` directory as shown above.

## Implementation Details

### 1. Porting from 100ask code

The original source code is available at: https://github.com/100askTeam/lv_lib_100ask/tree/master/src/lv_100ask_sketchpad.

The main tasks were porting the code to the current hardware and updating it for LVGL v9.2.2.

> Note: LVGL v9.1 does not work for this example, nothing will be shown on the canvas.

#### Issues while porting to the ESP32-S3 dev board

- <code style="color : red">"error: field 'img' has incomplete type"</code>
    - This occurs because the compiler can't find some `struct` definitions.
    - Solution: Add the following include for internal definitions, especially if you are customizing widgets:
        ```c
        #include "lvgl/src/lvgl_private.h"
        ```

- <code style="color : red">No colorwheel in lvgl >v9.0</code>
    - The colorwheel widget was removed in LVGL v9.0 and above. Disable related functions.
    - See: https://github.com/lvgl/lvgl/issues/4429#issuecomment-2051520229

- <code style="color : red">'dram0_0_seg' overflowed</code>
    - The sketchpad needs a canvas object and buffer, which requires 240x320x2 bytes.
    - PlatformIO uses ESP-IDF v4.4 by default, which lacks some PSRAM features.
    - Add the following definition in `platformio.ini` to use standard C `malloc` for memory allocation. When the allocation is >1k, it will use external RAM by default:
        ```c
        -D 'LV_USE_STDLIB_MALLOC=LV_STDLIB_CLIB'
        ```

    - Default SDK config location on Windows:
        ```c
        // The default sdkconfig location in Windows
        %homepath%\.platformio\packages\framework-arduinoespressif32\tools\sdk\esp32s3

        // Default PSRAM usage definitions
        CONFIG_SPIRAM_USE_MALLOC=y
        CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL=4096
        ```

## Customization

If you have different LCD or touch hardware, update the `lvgl` and `LovyanGFX` configurations accordingly.

- For LVGL, add configuration options in `platformio.ini`.
- For LovyanGFX, edit `include/custom_disp.hpp`.

## License

MIT
