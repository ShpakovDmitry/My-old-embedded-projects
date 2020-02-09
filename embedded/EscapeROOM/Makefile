_1_SUB_PROJ_DIR = med_panel
_2_SUB_PROJ_DIR = radio_morse_panel
_1_SUB_PROJ_BUILD_DIR = build
_2_SUB_PROJ_BUILD_DIR = build
TARGET_BUILD_FILE = hex
TOP_BUILD_DIR = build



all:
	make -C $(_1_SUB_PROJ_DIR) all
	make -C $(_2_SUB_PROJ_DIR) all
	mkdir --parents $(TOP_BUILD_DIR)
	cp $(_1_SUB_PROJ_DIR)/$(_1_SUB_PROJ_BUILD_DIR)/*.$(TARGET_BUILD_FILE) $(TOP_BUILD_DIR)
	cp $(_2_SUB_PROJ_DIR)/$(_2_SUB_PROJ_BUILD_DIR)/*.$(TARGET_BUILD_FILE) $(TOP_BUILD_DIR)
	
clean:
	
	make -C $(_1_SUB_PROJ_DIR) clean
	make -C $(_2_SUB_PROJ_DIR) clean
	rm -rf $(TOP_BUILD_DIR)
