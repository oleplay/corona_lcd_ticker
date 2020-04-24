#include <Arduino.h>

#include <Wire.h>

#include <LiquidCrystal.h>



// Credit to https://arduino.stackexchange.com/a/1237
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

LiquidCrystal lcd(10, 8, 5, 4, 3, 2);
LiquidCrystal lcd2(10, 9, 5, 4, 3, 2);
// LiquidCrystal lcd2(12, 11, 6, 4, 3, 2);

// Initialize needed Variables
String data;
String nrw_cases, nrw_diff, nrw_deaths, de_cases, de_diff, de_deaths, title, pubtime, headline_mvt;
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(40, 2);
  lcd2.begin(40,2);

  lcd.setCursor(0,0);
  lcd.print("Hello World");
  lcd.setCursor(0,1);
  lcd.print("Hello World");

  lcd2.setCursor(0,0);
  lcd2.print("Hello World");
  lcd2.setCursor(0,1);
  lcd2.print("Hello World");
  // initialize the serial communications:
  Serial.begin(9600);
}

void loop() {
  // Check for opened serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
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
    
    delay(10);

    
    // Serial.println(test);
    // lcd.print(test);
  }



}




// Code for printing out a fixed string in line one of 16x02 LCD, and a scrolling
// string with unlimited length in line two.

/* For my fellow beginners: In this code, we will make use of the fact
 *  that LCD can store 80 characters (40 in each line). Therefore, in order
 *  to better visualize the algorithm, imagine your LCD as 40x2 instead of
 *  16x2. At any given moment it displays a chosen 16x2 portion of it, but we can move
 *  the area that is being displayed with scrollDisplayLeft and scrollDisplayRight
 *  functions (in this code we will only use scrollDisplayLeft). The way
 *  scrollDisplayLeft works is as follows: In the starting position, LCD displays
 *  between position (0,0) and position (15,1). If you run scrollDisplayLeft function
 *  once, LCD will display between (1,0) and (16,1), and so on. This, combined with
 *  the ability to print into the positions outside of the visible are of the screen, is
 *  the basis of the code. Scrolling a string longer than 40 characters causes the string
 *  to leak into other lines, which makes the LCD display unwanted parts of the string.
 *  In order to get around the 40 character limit per line,
 *  we will divide the string that we want to scroll, into 16-character-long string
 *  chunks, and print two of them at any given time, starting from (0,1) and ending
 *  at (31,1). So, initially, the first chunk will be visible on line two. We will then run
 *  scrollDisplayLeft sixteen times, and have the second chunk display in its entirety.
 *  We will run the code enough times that all the chunks that make up our original
 *  string will run one after the other, creating a smooth scroll.
 */

// #include <Wire.h>
// #include <LiquidCrystal_I2C.h> // Written for LCD with I2C, include you relevant libraries.
// const byte lcdAddr = 0x27;
// const byte lcdCols = 16;
// const byte lcdRows = 2;
// LiquidCrystal_I2C lcd(lcdAddr, lcdCols, lcdRows);

// void setup() {
//   Serial.begin(115200);
//   lcd.begin(16, 2);
//   lcd.init();
//   lcd.backlight();
// }

// void loop () {

//   String Line1_fixed = "ShowerThoughts:"; //This is the string that will be fixed in the first line of LCD.
//   String Line2_scrolling_base = "Making fun of a fat person at the gym is like making fun of a homeless person that is looking for a job. / It would really suck getting kidnapped with a cold. If they taped your mouth shut, you really wouldn't be able to breathe. / A lot of jobs in the IT field could basically be retitled to 'Professional Googler'."; //This intentionally long string will be scrolled in the second line of LCD.

//   lcd.clear();
//   lcd.print(Line1_fixed); //Print first line.
//   delay(500); //Optional delay. If you omit this, the second line starts scrolling in from bottom right of the display (15,1), rather than entering from outside of the display (16,1). I find the latter visually more appealing, but it is optional.

//   String Line2_scrolling =  "                "; // Defining a string consisting of sixteen spaces.
//   Line2_scrolling += Line2_scrolling_base; // Adding sixteen spaces in front of the string that we want displayed. This is done so that line 2 will initially be empty in the display. Now we can display the string "Line2_scrolling".

//   int Line2_length = Line2_scrolling.length(); // Length of our string (including the sixteen spaces that we added in front).
//   int Line2_ChunkNumber = floor(Line2_length / 40 + 2); // Number of sixteen-character chunks in the string, plus one for the last chunk that is less than sixteen characters long.
//   int Line2_LastChunkLength = Line2_length - ((Line2_ChunkNumber - 2) * 40); // Length of the last chunk, which is usually less than sixteen.
//   int Line2_ChunkLength; // Will be initialized as 16 below, which is the usual length of chunks. But it will be changed if the last chunk is smaller in length.
//   if (Line2_LastChunkLength == 0) { // If the string has a character length of a multiple of sixteen, Line2_ChunkNumber is reduced by one, in order to prevent redundant "for" cycles.
//     Line2_ChunkNumber--;
//   }

//   //  Printing every variable on Serial, in case we want to see and verify them. The only remaning variable, Line2_ChunkLength, will be printed later on because it isn't initialized yet.
//   //  Serial.print("Line2_length: ");
//   //  Serial.println(Line2_length);
//   //  Serial.print("Line2_ChunkNumber: ");
//   //  Serial.println(Line2_ChunkNumber);
//   //  Serial.print("Line2_LastChunkLength: ");
//   //  Serial.println(Line2_LastChunkLength);

//   String chunk[Line2_ChunkNumber]; // Create an array that can hold all the divided chunks of our scrolling string inside it.

//   for (int j = 0; j < Line2_ChunkNumber - 1; j++) { // Assign all chunks inside chunk[].
//     chunk[j] = Line2_scrolling.substring(j * 40 , j * 40 + 40);
//   }

//   for (int k = 0; k < Line2_ChunkNumber - 1; k++) {
//     lcd.clear();
//     lcd.print(Line1_fixed); // Print it again to avoid flicker.
//     lcd.setCursor(0, 1);
//     lcd.print(chunk[k]); // Display the string with the chunk number "k", starting from (0,1). This is the entire visible screen area of line 2 of the LCD.
//     lcd.print(chunk[k + 1]); // Print the string with the chunk number "k+1", starting from (16,1). This is the area that is immediately to the right of the visible area of line 2 of the LCD.
//     if (k != Line2_ChunkNumber - 2) { // If we are not dealing with the last chunk, set the number of iterations in the next "for" cycle to 16. If it is the last chunk and if the length of the last chunk isn't zero, set the number of iterations in the next "for" cycle to size of last chunk.
//       Line2_ChunkLength = 40;
//     }
//     else if (Line2_LastChunkLength != 0) {
//       Line2_ChunkLength = Line2_LastChunkLength;
//     }

//     for (int i = 1; i < Line2_ChunkLength + 1; i++) {
//       lcd.scrollDisplayLeft(); // Scroll display left by one position.
//       lcd.setCursor(i, 0); // Set cursor on the first line to allign with the scrolling string in the second line of LCD, in order to print the fixed string on the first line above where scrolling string starts. Currently in the algorithm, scrolling string starts at (i,1), hence setting the cursor to (i,0).
//       lcd.print(Line1_fixed); // Print the fixed string so that its starting position alligns with the starting position of the scrolling string.
//       delay(300); // Delay for visual ease of reading.
//     }
//   }
//   //  Printing out the final variable to Serial for verification.
//   //  Serial.print("Line2_ChunkLength: ");
//   //  Serial.println(Line2_ChunkLength);
//   //  Serial.println("-----------------");
// }