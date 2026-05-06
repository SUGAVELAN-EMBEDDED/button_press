# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/sugavelan/zephyrproject/modules/hal/espressif/components/partition_table"
  "/home/sugavelan/zephyrproject/button_press_driver/build/esp-idf/build"
  "/home/sugavelan/zephyrproject/button_press_driver/build/zephyr/soc/soc/xtensa/espressif_esp32/esp32/EspPartitionTable-prefix"
  "/home/sugavelan/zephyrproject/button_press_driver/build/zephyr/soc/soc/xtensa/espressif_esp32/esp32/EspPartitionTable-prefix/tmp"
  "/home/sugavelan/zephyrproject/button_press_driver/build/zephyr/soc/soc/xtensa/espressif_esp32/esp32/EspPartitionTable-prefix/src/EspPartitionTable-stamp"
  "/home/sugavelan/zephyrproject/button_press_driver/build/zephyr/soc/soc/xtensa/espressif_esp32/esp32/EspPartitionTable-prefix/src"
  "/home/sugavelan/zephyrproject/button_press_driver/build/zephyr/soc/soc/xtensa/espressif_esp32/esp32/EspPartitionTable-prefix/src/EspPartitionTable-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/sugavelan/zephyrproject/button_press_driver/build/zephyr/soc/soc/xtensa/espressif_esp32/esp32/EspPartitionTable-prefix/src/EspPartitionTable-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/sugavelan/zephyrproject/button_press_driver/build/zephyr/soc/soc/xtensa/espressif_esp32/esp32/EspPartitionTable-prefix/src/EspPartitionTable-stamp${cfgdir}") # cfgdir has leading slash
endif()
