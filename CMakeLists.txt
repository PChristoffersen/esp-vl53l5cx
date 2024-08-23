idf_component_register(
    SRC_DIRS "src" "ext/vl53l5cx_udl_api/src"
    INCLUDE_DIRS "include" "ext/vl53l5cx_udl_api/inc"
    REQUIRES "esp_driver_i2c"
)