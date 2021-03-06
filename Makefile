################################################################################
#
# definitions and flags
#
PORT		= usb-0000
DEVICE		= atmega328p
PROGRAMMER	= atmelice_isp
BAUD		= 115200

OPTIMIZE	= s
INCLUDEPATH	= -I . -I ./src

CFLAGS		= -Wall -O$(OPTIMIZE) $(INCLUDEPATH) $(INCLUDELIBRARY)
AVRGCC		= avr-gcc $(CFLAGS) -mmcu=$(DEVICE)
OUTPUTFILE	= wfs
OBJ			= obj/twi328P.o obj/ssd1306.o obj/main.o obj/yfs401.o
################################################################################

default: link upload clean

link: $(OBJ)
	$(AVRGCC) $(OBJ) -o $(OUTPUTFILE).elf
	avr-objcopy -j .text -j .data -O ihex $(OUTPUTFILE).elf $(OUTPUTFILE).hex
	avr-size --format=avr --mcu=$(DEVICE) $(OUTPUTFILE).elf

obj/main.o: main.cpp
	$(AVRGCC) -c main.cpp -o obj/main.o

obj/twi328P.o: src/twi328P/twi328P.cpp src/twi328P/twi328P.h
	$(AVRGCC) -c src/twi328P/twi328P.cpp -o obj/twi328P.o
	
obj/yfs401.o: src/yfs401/yfs401.cpp src/yfs401/yfs401.h
	$(AVRGCC) -c src/yfs401/yfs401.cpp -o obj/yfs401.o

obj/ssd1306.o: src/ssd1306/ssd1306.cpp src/ssd1306/ssd1306.h src/ssd1306/ssd1306CommandSet.h
	$(AVRGCC) -c src/ssd1306/ssd1306.cpp -o obj/ssd1306.o

#obj/init.o: src/ssd1306/init.cpp src/ssd1306/ssd1306.h
#	$(AVRGCC) -c src/ssd1306/init.cpp -o obj/init.o
	


#obj/main.o: main.cpp
#	@echo "obj/main.o"
#	$(AVRGCC) -c main.cpp -o obj/main.o

upload:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(OUTPUTFILE).hex:i
	
clean:
	rm obj/*.o $(OUTPUTFILE).elf $(OUTPUTFILE).hex
################################################################################	
#
# target: dependencies
#	action


