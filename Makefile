LOCAL_APP_DIR = $(HOME)/.local/share/applications
DESKTOP_TARGET = $(LOCAL_APP_DIR)/sVisualizer.desktop
ASSETS_TARGET = /usr/share/icons/sv.png

all:
	gcc main.c -o sv `sdl2-config --cflags --libs` -lSDL2_image

install:
	gcc main.c -o sv `sdl2-config --cflags --libs` -lSDL2_image
	mv sv /usr/local/bin/
	chmod +x desktop/sVisualizer.desktop
	if [ -f "$(DESKTOP_TARGET)" ]; then rm -rf "$(DESKTOP_TARGET)"; fi
	if [ -f "$(ASSETS_TARGET)" ]; then rm -rf "$(ASSETS_TARGET)"; fi
	cp assets/sv.png /usr/share/icons/
	cp desktop/sVisualizer.desktop $(LOCAL_APP_DIR)

remove:
	rm -rf /usr/local/bin/sv
	rm -rf /usr/share/icons/sv.png
	rm -rf $(LOCAL_APP_DIR)/desktop/sVisualizer.desktop
