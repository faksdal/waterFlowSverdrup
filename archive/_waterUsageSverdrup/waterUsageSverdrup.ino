/*******************************************************

  
    Calibration data from 30/9-2020:
        |_____________________________|
        |5098|pulses per 1000 ml water|
        |-----------------------------|
        |5022|pulses per 1000 ml water|
        |-----------------------------|
        |4995|pulses per 1000 ml water|
        |-----------------------------|
        |5043|pulses per 1000 ml water|
        |-----------------------------|
        |4972|pulses per 1000 ml water|
        |-----------------------------|

    Average:
        5026    pulses per 1000 ml water

********************************************************/

#include <EEPROM.h>
#include <LiquidCrystal.h>

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

double          pulsesPerLiter;
volatile double waterFlowTotal, waterFlowLast;

//volatile double pulses;

int lcd_key     = 0;
int adc_key_in  = 0;
int pageNum     = 0;
int displayOn   = 1;

long    runTime     = 0;
long    lastTime    = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);        // select the pins used on the LCD panel



// read the buttons
int readButtons(void)
{
    adc_key_in = analogRead(0);      // read the value from the sensor
    
    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    
    if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
    
    // For V1.1 us this threshold
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 250)  return btnUP;
    if (adc_key_in < 450)  return btnDOWN;
    if (adc_key_in < 650)  return btnLEFT;
    if (adc_key_in < 850)  return btnSELECT;

    // For V1.0 comment the other threshold and use the one below:
    /*
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 195)  return btnUP;
    if (adc_key_in < 380)  return btnDOWN;
    if (adc_key_in < 555)  return btnLEFT;
    if (adc_key_in < 790)  return btnSELECT;
    */
        
    return btnNONE;  // when all others fail, return this...
    
}



void readPulse(void)   //measure the quantity of square wave
{
  waterFlowTotal    += 1.0 / pulsesPerLiter;
  waterFlowLast     += 1.0 / pulsesPerLiter;
  //pulses++;

  displayPage(0);
}



void displayPage(int page)
{
    if(displayOn == 0){
        displayOn = 1;
        lcd.display();
    }

    lastTime = runTime;
    
    switch(page){
        case 0:     lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Total: ");
                    lcd.print(waterFlowTotal);
                    lcd.print(" L");
                    lcd.setCursor(0, 1);
                    lcd.print(" Last: ");
                    lcd.print(waterFlowLast);
                    lcd.print(" L");
                    lcd.print(" ");
                    break;

        case 1:     lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Page 1");
                    break;
        case 2:     lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Page 2");
                    break;
        case 3:     lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Page 3");
                    break;          
                    
        default:    break;                
    }
}



void setup(){
    lcd.begin(16, 2);

    lcd.clear();

    for(int i = 15; i > -1; i--){
        lcd.clear();
        lcd.setCursor(i, 0);
        lcd.print("Waterflow");
        delay(233);
    }
    lcd.setCursor(0, 1);
    lcd.print("Jon Leithe 2020");
    delay(2250);

    waterFlowTotal  = 0.0;
    waterFlowLast   = 0.0;
    pulsesPerLiter  = 5026.0;
    
    //pulses          = 0;
    
    
    attachInterrupt(0, readPulse, RISING);  //DIGITAL Pin 2: Interrupt 0
    delay(117);
    displayPage(pageNum);

    runTime = millis();
    lastTime = runTime;
}



void loop(){

    lcd_key = readButtons();

    switch (lcd_key){
        case btnUP:     /*
                        if(pageNum == 0)
                            pageNum = 3;
                        else
                            pageNum--;
                        displayPage(pageNum);
                        delay(1000);
                        */
                        break;
                        
        case btnDOWN:   /*
                        if(pageNum == 3)
                            pageNum = 0;
                        else
                            pageNum++;
                        displayPage(pageNum);
                        delay(1000);
                        */
                        break;
                        
        case btnSELECT: displayOn = 1;
                        lcd.display();
                        if(pageNum == 0){
                            waterFlowLast = 0.0;
                            displayPage(pageNum);
                            delay(333);
                        }
                        break;
                        
        case btnNONE:   //displayPage(pageNum);
                        break;
                        
        default:        break;    
    }

    runTime = millis();
    
    if(runTime - lastTime > 13000){
        displayOn = 0;
        lcd.noDisplay();
    }
    
}
