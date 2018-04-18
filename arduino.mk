ARDUINO_DIR = Command
ARDUINO_ZIP = Command.zip
ARDUINO_OBJECTS = Command/command.cpp Command/command.h

.PHONY: arduino clean-arduino

arduino: $(ARDUINO_ZIP)

$(ARDUINO_DIR):
	mkdir -p $@

$(ARDUINO_DIR)/command.cpp: src/command.c | $(ARDUINO_DIR)
	cp $< $@

$(ARDUINO_DIR)/command.h: lib/command.h | $(ARDUINO_DIR)
	cp $< $@

$(ARDUINO_ZIP): $(ARDUINO_OBJECTS) | $(ARDUINO_DIR)
	zip -r $@ $(ARDUINO_DIR)

clean-arduino:
	-rm -rf $(ARDUINO_ZIP)
	-rm -rf $(ARDUINO_DIR)
