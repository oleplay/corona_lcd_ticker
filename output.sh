#!/bin/bash

chmod +x Scraper/python/bin/activate
source Scraper/python/bin/activate

cd ./Scraper/python/tagesschau_corona/

now=$(date  "+%d-%m-%Y")

rm "current_articles ${now}.csv"
rm "cases ${now}.csv"
scrapy crawl corona_ticker -o "current_articles ${now}.csv" -t csv
scrapy crawl rki_corona -o "cases ${now}.csv" -t csv

cd ../../../

python Serialprint.py