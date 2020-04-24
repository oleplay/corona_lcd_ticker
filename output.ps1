.\Scraper\python\Scripts\Activate.ps1

Set-Location .\Scraper\python\tagesschau_corona\

Remove-Item "current_articles $(get-date -f dd-MM-yyyy).csv"
Remove-Item "cases $(get-date -f dd-MM-yyyy).csv"
scrapy crawl corona_ticker -o "current_articles $(get-date -f dd-MM-yyyy).csv" -t csv
scrapy crawl rki_corona -o "cases $(get-date -f dd-MM-yyyy).csv" -t csv

Set-Location ..\..\..\

python Serialprint.py