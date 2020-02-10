TOP_BUILD_DIR = build

all:
	make -C med_panel/ all
	make -C radio_morse_panel all
	mkdir --parents $(TOP_BUILD_DIR)
	cp */build/*.hex $(TOP_BUILD_DIR)
	
clean:
	
	make -C med_panel clean
	make -C radio_morse_panel clean
	rm -rf $(TOP_BUILD_DIR)
