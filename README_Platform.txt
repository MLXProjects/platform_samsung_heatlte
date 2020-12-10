How to build Module for Platform
- It is only for modules are needed to using Android build system.
- Please check its own install information under its folder for other module.

[Step to build]
1. Get android open source.
: version info - Android jellybean 4.4.2_r2

( Download site : http://source.android.com )

2. Copy module that you want to build - to original android open source
If same module exist in android open source, you should replace it. (no 
overwrite)


# It is possible to build all modules at once.


3. You should add module name to 'PRODUCT_PACKAGES' in 'build\target\product\
core.mk' as following case.

case 1) libasound : should add 'libasound' to PRODUCT_PACKAGES
case 2) libaudio_test : should add 'libaudio_test' to PRODUCT_PACKAGES
case 3) libexifa : should add 'libexifa' to PRODUCT_PACKAGES
case 4) libjpega : should add 'libjpega' to PRODUCT_PACKAGES
case 5) e2fsck : should add 'e2fsck' to PRODUCT_PACKAGES
case 6) blkid : should add 'blkid' to PRODUCT_PACKAGES
case 7) KeyUtils : should add 'libkeyutils' to PRODUCT_PACKAGES
case 8) libhyphenation : should add 'libhyphenation' to PRODUCT_PACKAGES
case 9) libwebcore : should add 'libwebcore' to PRODUCT_PACKAGES


ex.) [build\target\product\core.mk] - add all module name for all follow cases at once

PRODUCT_PACKAGES += \
    libasound \
    libaudio_test \
    libexifa \
    libjpega \
    e2fsck \
    blkid \
    libkeyutils \
    libhyphenation \
    libwebcore

4. You should add build option to 'build\target\board\generic\BoardConfig.mk' as following case.

case ) BOARD_USES_ALSA_AUDIO := true


5. excute build command
./build.sh user




