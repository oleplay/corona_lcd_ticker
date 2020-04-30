// Import needed
#include <Arduino.h>

#include <Wire.h>

#include <LiquidCrystal.h>

// Init LCD Objects
// Two are needed because an 40x04 LCD utilizes two LCD Controller, one for 2 Rows
LiquidCrystal lcd(9, 7, 5, 4, 3, 2); // First two rows
LiquidCrystal lcd2(9, 8, 5, 4, 3, 2); // Last two rows

// Initialize needed Variables
String header = "+++Corona News Blog+++";
String names[]= {"Lasse","Andy","Ole"};

String nrw_cases, nrw_diff, nrw_deaths, de_cases, de_diff, de_deaths, title, pubtime, headline_mvt, pubdate, lastupdate;
String data;

// Custom characters. Made with https://maxpromer.github.io/LCD-Character-Creator/
byte Heart[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};

byte diff[] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};

byte death[] = {
  B00100,
  B00100,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};

// define LCD Size
int lcdcol = 40;
int lcdrow = 2;

int position = 0;

boolean ascroll = false;


// Credit goes to https://arduino.stackexchange.com/a/1237
// Used to split String into multiple by using delimiter
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {
        0,
        -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// Get data over Serial
void getData()
{
    // Wait for serial availability
    while (!(Serial.available()))
    {
    }
    // Wait for the entire data to arrive
    delay(30);

    // Save transmitted data in data
    data = Serial.readStringUntil('\n');

    // Extract data into different Variables
    nrw_cases = getValue(data, '~', 0);
    nrw_diff = getValue(data, '~', 1);
    nrw_deaths = getValue(data, '~', 2);
    de_cases = getValue(data, '~', 3);
    de_diff = getValue(data, '~', 4);
    de_deaths = getValue(data, '~', 5);
    title = getValue(data, '~', 6);
    pubtime = getValue(data, '~', 7);
    headline_mvt = getValue(data, '~', 8);
    pubdate = getValue(data, '~', 9);
    lastupdate = getValue(data, '~', 10);



    // Merge publication time and title
    title = pubtime + " | " + title;
    // Merge Date and last update time
    lastupdate = pubdate + ' '+ lastupdate;

}


// Print case data on LCD
void print_cases()
{

    lcd2.clear();
    // Merge case description with case numbers
    nrw_cases = "NRW: " + nrw_cases;
    nrw_diff = ": " + nrw_diff;
    nrw_deaths = ": " + nrw_deaths;

    // calculate total String length and number of whitespaces needed
    int len = nrw_cases.length() + nrw_diff.length()+1 + nrw_deaths.length()+1;
    int spaces = (40 - len) / 2;

    // Uniformly space the Strings and print to LCD
    lcd2.setCursor(0, 1);
    lcd2.print(nrw_cases);
    int diff_cursor_pos = ((nrw_cases.length() +1)+ spaces);
    lcd2.setCursor(diff_cursor_pos, 1);
    // Print Custom char
    lcd2.print(char(1));
    lcd2.print(nrw_diff);
    lcd2.setCursor(lcdcol - (nrw_deaths.length()+1), 1);
    lcd2.print(char(2));
    lcd2.print(nrw_deaths);

    // Merge case description with case numbers
    de_cases = "DE : " + de_cases;
    de_diff = ": " + de_diff;
    de_deaths = ": " + de_deaths;

    // calculate total String length and number of whitespaces needed

    // len = de_cases.length() + de_diff.length() +1 + de_deaths.length()+1 ;
    // spaces = (40 - len) / 2;

    // Uniformly space the Strings and print to LCD
    lcd2.setCursor(0, 0);
    lcd2.print(de_cases);
    // Equally Place diff in the middle based on Position off NRW Diff
    lcd2.setCursor(diff_cursor_pos, 0);
    // lcd2.setCursor(((de_cases.length() +1)+ spaces), 0);
    lcd2.print(char(1));
    lcd2.print(de_diff);
    lcd2.setCursor(lcdcol - (de_deaths.length()+1), 0);
    lcd2.print(char(2));
    lcd2.print(de_deaths);

}

// Print Header, Date and time
void write_header(){
    lcd.setCursor(0,0);
    lcd.print(header);
    lcd.setCursor((lcdcol-lastupdate.length()),0);
    lcd.print(lastupdate);
}
// Initialize the LCD and print start String
void lcd_init()
{
    // Define the LCD size
    lcd.begin(lcdcol, lcdrow);
    lcd2.begin(lcdcol, lcdrow);

    // Create custom characters
    lcd2.createChar(0,Heart);
    lcd2.createChar(1, diff);
    lcd2.createChar(2, death);
    
    // Clear both screens
    lcd.clear();
    lcd2.clear();
}

// Print start screen 
void lcd_start_screen(){

    // Print start screen
    // Write headline in the middle of display
    lcd.setCursor((40-header.length())/2, 0);
    lcd.print(header);
    lcd.setCursor(0,1);
    lcd.print("Warte auf Daten...");

    String madeby[] = {"Made with ", " by:"};

    

    // Write text in the middle of display
    lcd2.setCursor((40-(madeby[0].length()+1+madeby[1].length()))/2,0);
    lcd2.print(madeby[0]);

    // Print custom Heart
    lcd2.print(char(0));
    lcd2.print(madeby[1]);

    

    // Calculate distance to equally space the names
    int spaces = (40-(names[0].length() + names[1].length() + names[2].length()))/2;
    
    // Write names to the left, middle and right
    lcd2.setCursor(0,1);
    lcd2.print(names[0]);
    lcd2.setCursor((names[0].length())+spaces,1);
    lcd2.print(names[1]);
    lcd2.setCursor((lcdcol-names[2].length()),1);
    lcd2.print(names[2]);

}

// Display title and scroll display
void display_data(String data)
{

    lcd.setCursor(0, 1);
    int hilfe = position + 40;
    
    // Check for short String and only print it --> No Scrolling needed
    if (data.length() < 40)
    {
        lcd.print(data);
        return;
    }

    // Move through whole String
    for (; position < hilfe; position++)
    {

        // Print char from String according to current index
        lcd.print(data[position]);

        // Add delay while scrolling for better readability
        if (ascroll)
        {
            // We found 350 milliseconds to be the sweetspot
            delay(350);
        }

        // Delay printing of each character for better readability and looks
        delay(150);

        // Enable autoscroll when end of row was reached  
        if (position > lcdcol && ascroll == false)
        {
            lcd.autoscroll();
            ascroll = true;
        }

        // Reset LCD when end String was reached
        if ((position > (data.length())))
        {

            // Set aux. Variables
            position = -1;
            hilfe = 40;

            // Disable autoscroll
            lcd.noAutoscroll();
            ascroll = false;

            // Delay erasing of screen for better readability
            delay(1000);
            
            // Clear screen and print Headline
            lcd.clear();
            write_header();
            lcd.setCursor(0, 1);
        }
    }
}

// Executed on startup and reset --> Executed on every serial connect
void setup()
{
    // Open serial port with 9600 Baud
    Serial.begin(9600);

    // Call LCD initialization method
    lcd_init();

    // Display the wait for data Screen
    lcd_start_screen();

    // Get serial data
    getData();

    // Reprint Headline
    lcd.clear();
    lcd2.clear();
    write_header();
    
    // Print case data
    print_cases();
}


// Gets constantly repeated
void loop()
{
    // Display the title and Scroll
    display_data(title);
}