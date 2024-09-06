# Ultra Lite Driver (ULD) for VL53L5CX multi-zone sensor

[![Driver Version](https://img.shields.io/badge/driver_version-2.0.0-blue)](https://www.st.com/en/embedded-software/stsw-img023.html)

## Description
This is a slightly modified version of the STMicroelectronics ultra lite driver (ULD) for VL53L5CX. from:
- [https://www.st.com/en/embedded-software/stsw-img023.html](https://www.st.com/en/embedded-software/stsw-img023.html)

## Changes
- Renamed platform.h to vl53l5cx_platform.h
- Added `extern "C" {...` to all header files to enabled using them from c++ code


## Documentation
VL53L5CX ULD user manual UM2884 is avaliable [here](https://www.st.com/resource/en/user_manual/um2884-a-guide-to-using-the-vl53l5cx-multizone-timeofflight-ranging-sensor-with-a-wide-field-of-view-ultra-lite-driver-uld-stmicroelectronics.pdf).


## License:
[BSD-3-Clause](LICENSE.txt)
