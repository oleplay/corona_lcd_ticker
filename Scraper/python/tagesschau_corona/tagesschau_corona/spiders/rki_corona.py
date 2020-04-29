# -*- coding: utf-8 -*-

# Import the needed Python libraries
import scrapy
import datetime

# Map "Umlaute" to LCD compatible formatting
special_char_map = {ord('ä'):'ae', ord('ü'):'ue', ord('ö'):'oe', ord('ß'):'ss',ord('Ä'):'Ae', ord('Ü'):'Ue', ord('Ö'):'Oe'}

class RkiCoronaSpider(scrapy.Spider):
    # Spider name
    name = 'rki_corona'
    # Allow only access to rki.de (To counteract false redirects)
    allowed_domains = ['www.rki.de']
    

    def start_requests(self):
        
        
        # Static start URL
        start_urls = ['https://www.rki.de/DE/Content/InfAZ/N/Neuartiges_Coronavirus/Fallzahlen.html/']
        
        
        # Request the start URL and call parse function
        for url in start_urls:
            self.logger.info(url)
            yield scrapy.Request(url, callback = self.parse)
        
        
    # Extract all needed information from the site    
    def parse(self, response):
        
        # Iterate through every line of the table
        for line in response.xpath('//tbody//tr'):
            
            
            if not (line.xpath('.//td[1]/text()').extract()): # Check for "Gesamt" Edge case. Because there text is written in "Strong" tags
                if ("Gesamt" in line.xpath('.//td[1]/strong/text()').get()): # Confirm edge case
                    name = line.xpath('.//td[1]/strong/text()').get()
                    cases = line.xpath('.//td[2]/strong/text()').get()
                    diff = (line.xpath('.//td[3]/strong/text()').get())
                    cases_100 = line.xpath('.//td[4]/strong/text()').get()
                    deaths = line.xpath('.//td[5]/strong/text()').get()
            else:
                
                # move multiline entries to single line
                name = line.xpath('.//td[1]/text()').extract()
                name = (''.join(name)).replace('\n','')
                
                
                    
                
                cases = line.xpath('.//td[2]/text()').get()
                diff = (line.xpath('.//td[3]/text()').get())
                cases_100 = line.xpath('.//td[4]/text()').get()
                deaths = line.xpath('.//td[5]/text()').get()
                
           
                update = response.xpath('//div[@id="content"]//div[@class = "dateOfIssue"]//p/text()')[2].get()
                update = update.replace("Stand: ", '')
            
                # Check death counter for point (NRW is not delimited by point)
                deaths_no_point = deaths.replace('.','')
                
                if (int(deaths_no_point) > 1000 and not('.' in deaths)):
                    # Extract last 3 Digits from deaths
                    last=deaths[-3:]
                    # Combine Digits after last 3, dot and last 3 digits
                    deaths = deaths[0:(len(deaths)-3)]+'.'+last

                
            # Export data while replacing all "Umlaute"
            yield {
                'name' : name.translate(special_char_map),
                'cases' : cases,
                'diff' : diff,
                'deaths' : deaths,
                'update' : update,
            }