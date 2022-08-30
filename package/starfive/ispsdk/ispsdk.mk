################################################################################
#
# ispsdk:
#   ispsdk package will generate isp sdk app to contrl the sensor image quality
#   reference to the build script soft_3rdpart/ispsdk/build.sh
#
################################################################################
ISPSDK_VERSION := 2.0.0
ISPSDK_SITE = $(TOPDIR)/../soft_3rdpart/ispsdk/DDKSource
ISPSDK_SITE_METHOD = local
ISPSDK_SUPPORTS_IN_SOURCE_BUILD = NO
ISPSDK_DEPENDENCIES = jpeg libv4l libdrm

# default CMAKE_INSTALL_PREFIX is $(TARGET_DIR)/usr, change to below
# note: not have -DCROSS_COMPILE
ISPSDK_CONF_OPTS = -DRUN_PLATFORM="RISCV" \
					-DCI_LOG_LEVEL=4 \
					-DARCH_RISCV="Y" \
					-DCMAKE_INSTALL_PREFIX="/root" \
					-DLINUX_KERNEL_BUILD_DIR="$(TOPDIR)/../work/linux" \
					-DDRM_INCLUDE_DIR="$(STAGING_DIR)/usr/include/drm"

$(eval $(cmake-package))
