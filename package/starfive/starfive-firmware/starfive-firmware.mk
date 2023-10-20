################################################################################
#
# starfive-firmware
#
################################################################################
STARFIVE_FIRMWARE_LICENSE = Proprietary

# AP6256 Bluetooth
ifeq ($(BR2_PACKAGE_STARFIVE_FIRMWARE_AP6256_BLUETOOTH),y)
define STARFIVE_FIRMWARE_INSTALL_AP6256_BLUETOOTH
	@echo "install AP6256 BLUETOOTH firmware ..."
	$(INSTALL) -m 0755 -D $(STARFIVE_FIRMWARE_PKGDIR)/ap6256-bluetooth/BCM4345C5.hcd $(TARGET_DIR)/lib/firmware/
	$(INSTALL) -m 0755 -D $(STARFIVE_FIRMWARE_PKGDIR)/ap6256-bluetooth/S90bluetooth $(TARGET_DIR)/etc/init.d/
endef
endif

# RTL8852BU Bluetooth
ifeq ($(BR2_PACKAGE_STARFIVE_FIRMWARE_RTL8852BU_BLUETOOTH),y)
define STARFIVE_FIRMWARE_INSTALL_RTL8852BU
	@echo "install RTL8852BU bluetooth firmware ..."
	$(INSTALL) -m 0755 -D $(STARFIVE_FIRMWARE_PKGDIR)/rtl8852bu-bluetooth/rtl8852bu_config $(TARGET_DIR)/lib/firmware/
	$(INSTALL) -m 0755 -D $(STARFIVE_FIRMWARE_PKGDIR)/rtl8852bu-bluetooth/rtl8852bu_fw $(TARGET_DIR)/lib/firmware/
endef
endif

# AP6256 SDIO Wifi
ifeq ($(BR2_PACKAGE_STARFIVE_FIRMWARE_AP6256_SDIO_WIFI),y)
define STARFIVE_FIRMWARE_INSTALL_AP6256_SDIO_WIFI
	@echo "install AP6256_SDIO_WIFI firmware ..."
	$(INSTALL) -m 0755 -D $(STARFIVE_FIRMWARE_PKGDIR)/ap6256-sdio-wifi/fw_bcm43456c5_ag.bin $(TARGET_DIR)/lib/firmware/
	$(INSTALL) -m 0755 -D $(STARFIVE_FIRMWARE_PKGDIR)/ap6256-sdio-wifi/nvram_ap6256.txt $(TARGET_DIR)/lib/firmware/
endef
endif

# ECR6600U USB Wifi
ifeq ($(BR2_PACKAGE_STARFIVE_FIRMWARE_ECR6600U_USB_WIFI),y)
define STARFIVE_FIRMWARE_INSTALL_ECR6600U
	@echo "install ECR6600U firmware ..."
	$(INSTALL) -m 0755 -D $(STARFIVE_FIRMWARE_PKGDIR)/ECR6600U-usb-wifi/ECR6600U_transport.bin $(TARGET_DIR)/lib/firmware/
endef
endif

define STARFIVE_FIRMWARE_INSTALL_TARGET_CMDS
	$(STARFIVE_FIRMWARE_INSTALL_AP6256_BLUETOOTH)
	$(STARFIVE_FIRMWARE_INSTALL_RTL8852BU)
	$(STARFIVE_FIRMWARE_INSTALL_AP6256_SDIO_WIFI)
	$(STARFIVE_FIRMWARE_INSTALL_ECR6600U)

endef

$(eval $(generic-package))
