#!/bin/bash

# Move to working directory
cd /home/pi/corona

# Enable python venv
source Scraper/python/bin/activate


# Move to Scraper directory
cd ./Scraper/python/tagesschau_corona/

# Get current date
now=$(date  "+%d-%m-%Y")

# Remove old files
rm "current_articles ${now}.csv"
rm "cases ${now}.csv"

# Crawl webpages, extract data and save to file with date
scrapy crawl corona_ticker -o "current_articles ${now}.csv" -t csv
scrapy crawl rki_corona -o "cases ${now}.csv" -t csv

# Move back to original dir
cd ../../../

# Call Script to transmit data to Arduino
python Serialprint.py
