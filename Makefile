include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk

PKG_NAME:=esp_controler
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

define Package/$(PKG_NAME)
	CATEGORY:=Base system
	TITLE:=$(PKG_NAME)
	DEPENDS:= +libubus +libubox +libblobmsg-json +libserialport
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/esp-controler $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*h $(1)/usr/include
	$(INSTALL_BIN) ./files/esp_controler.init $(1)/etc/init.d/esp_controler 
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/esp-controler $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*h $(1)/usr/include
	$(INSTALL_BIN) ./files/esp_controler.init $(1)/etc/init.d/esp_controler 
endef

$(eval $(call BuildPackage,$(PKG_NAME)))