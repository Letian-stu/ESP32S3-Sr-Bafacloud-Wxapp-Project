# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/B_Code/espidf/esp32idf/esp-idf/components/bootloader/subproject"
  "D:/C_Github/ESP32S3-Sr-Bafacloud-Wxapp-Project/Software/ESP32_Project/build/bootloader"
  "D:/C_Github/ESP32S3-Sr-Bafacloud-Wxapp-Project/Software/ESP32_Project/build/bootloader-prefix"
  "D:/C_Github/ESP32S3-Sr-Bafacloud-Wxapp-Project/Software/ESP32_Project/build/bootloader-prefix/tmp"
  "D:/C_Github/ESP32S3-Sr-Bafacloud-Wxapp-Project/Software/ESP32_Project/build/bootloader-prefix/src/bootloader-stamp"
  "D:/C_Github/ESP32S3-Sr-Bafacloud-Wxapp-Project/Software/ESP32_Project/build/bootloader-prefix/src"
  "D:/C_Github/ESP32S3-Sr-Bafacloud-Wxapp-Project/Software/ESP32_Project/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/C_Github/ESP32S3-Sr-Bafacloud-Wxapp-Project/Software/ESP32_Project/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
