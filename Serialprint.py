# Import the needed Python libraries
import serial
import time
import csv
import datetime

# Get current date as (30-04-2020)
today = datetime.date.today().strftime("%d-%m-%Y")


# Open the cases file from rki_corona scraper
with open((r'.\Scraper\python\tagesschau_corona\cases '+today+".csv"),newline='') as csvfile:
    
    # Parse csv file
    rki_cases  = csv.reader(csvfile, delimiter=";", quotechar='"')
    
    # Iterate through all lines of file
    for row in rki_cases: 
        # print(row)
        
        # Extract data for NRW  
        if ("Nordrhein" in row[0]):
            nrw_cases = row[1]
            nrw_diff = row[2]
            nrw_deaths = row[3]
            # print (nrw_cases, nrw_diff)
        
        # Extract data for Germany
        elif ("Gesamt" in row[0]):
            de_cases = row[1]
            de_diff = row[2]
            de_deaths = row[3]
            # print (de_cases, de_diff)

# Open the current_articles file from corona_ticker scraper        
with open((r'.\Scraper\python\tagesschau_corona\current_articles '+today+".csv")) as csvfile:
    # Parse csv file
    articles = csv.reader(csvfile, delimiter=";", quotechar='"')
    
    # Skip first entry from csv (legend)
    articles = iter(articles) 
    next(articles)
    
    # Create needed Arrays
    headlines = []
    headline_mvt = []
    title = []
    description=[]
    pubtime = []
    pubdate = []
    
    # Extract data and append them to the specific array
    for row in articles:
        headlines.append(row[0])
        
        # Split of the important part of the headline
        headline_mvt.append( str(row[0]).split("|")[1])
        # print (headline_mvt[0])
        title.append(row[1])
        description.append(row[2])
        pubtime.append(row[3])
        pubdate.append(row[4])
                
                        
# Establish Serial connection
ser = serial.Serial('COM4',9600) 

# Check for opened serial port
if ser.isOpen():
    ser.close()

# Open serial port and wait for Arduino reset
ser.open()
ser.isOpen()
time.sleep(3)

# Combine needed data into single String delimited by '~'
data = nrw_cases + "~" + nrw_diff + "~" + nrw_deaths + "~" + de_cases + "~" + de_diff + "~" + de_deaths + "~" + title[0] + "~" + pubtime[0] + "~" + headline_mvt[0]
print(data)


# Send data to Arduino over serial
ser.write( (data + '\n').encode() )

ser.close()




# print (ser.readline())
# time.sleep(0.1)

# while ser.inWaiting
# val = ser.readline
# print (val)
# if headlines[0] in str(val):
    # print("ijdfiojio"
        
    

# if out != '':

