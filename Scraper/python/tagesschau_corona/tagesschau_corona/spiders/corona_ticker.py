# -*- coding: utf-8 -*-
# Import the needed Python libraries
import datetime
import re
import scrapy
import logging
import locale

# Get current date
now = datetime.datetime.now()


# Initialize a regex to check for month names in sentence
desc_re_month = re.compile(r"(\. )(?=\b(?:Januar|Februar|März|April|Mai|Juni|Juli|August|September|Oktober|November|Dezember)\b)",re.I)
# Regex for not selecting dots with trailing numbers
desc_re = re.compile(r"(?:\.+([^0-9]))",re.I)

# Map "Umlaute" to LCD compatible Style
special_char_map = {ord('ä'):'ae', ord('ü'):'ue', ord('ö'):'oe', ord('ß'):'ss',ord('Ä'):'Ae', ord('Ü'):'Ue', ord('Ö'):'Oe'}

class CoronaTickerSpider(scrapy.Spider):
    # Spider name
    name = 'corona_ticker'
    # Allow only access to tagesschau.de (To counteract false redirects)
    allowed_domains = [
        'www.tagesschau.de']
    
    
    def start_requests(self):
        
        # Get current date in the format: 20200421
        currentdate = now.strftime("%Y%m%d")
        
        # Limiting the "Meldungsarchiv" to the currentdate    
        url = "http://www.tagesschau.de/archiv/meldungsarchiv100~_date-"+currentdate+".html"
        
        
        # self.logger.info(url)
        
        # Request the "meldungsarchiv" URL and call the parse function
        yield scrapy.Request(url, callback=self.parse)



    # Search for the current Liveblog article, because there is no logical naming scheme for the uptodate URL
    def parse(self, response):
        
        # self.logger.info(response.url)
        
        # Extract every article from the webpage and iterate until the liveblog was found
        for article in response.xpath('//*[@id="content"]//h2[text() = "365-Tage-Meldungsarchiv"]/..//ul[@class = "list"]//li//a[@href]'):
            
            # Get link of the selected article
            href = "http://www.tagesschau.de"+ article.xpath('./@href').get()
            href_text = article.xpath('./text()').get()
            
            # self.logger.info(href + href_text)
            
            # Check if selected article is the liveblog
            if("corona" in href.lower() and "blog" in href_text.lower()):
                
                # Replace the html version with the rssfeed and call the parse_blog function 
                yield scrapy.Request(href.replace(".html", "~rss2feed.xml"), callback = self.parse_blog)
            
                
    # Extract all needed information from the liveblog
    def parse_blog(self, response):

        # Get the article headline
        headline = (response.xpath('//channel/title/text()').extract_first())
        
        # Iterate through every post
        for post in response.xpath('//channel/item'):
            
            # Get post title
            title = (post.xpath('./title/text()').extract_first())
            
            # Get post date and format it accordingly
            pub_date = (post.xpath('./pubDate/text()').extract_first())
            
            pub_date = datetime.datetime.strptime(pub_date, '%a, %d %b %Y %H:%M:%S %z')
            
            # Scraping locale idea to complicated on *nix Systems
            # locale.setlocale(locale.LC_ALL, 'en_US') #Windows
            # locale.setlocale(locale.LC_ALL, 'en_US.utf8') #Linux
            
            # locale.setlocale(locale.LC_ALL, 'de_DE') # Windows
            # locale.setlocale(locale.LC_ALL, 'de_DE.utf8') #Linux
            
            pub_time = pub_date.strftime("%H:%M")
            pub_date = pub_date.strftime('%d.%m.%Y')
            

            
            # Skip only title entries (previous day, Videos etc.)
            if ((post.xpath('./description/text()').extract_first() is not None)):
                
                # Skip the closing entry
                if(("beenden" in (post.xpath('./title/text()').extract_first()).lower() and "blog" in (post.xpath('./title/text()').extract_first()).lower())):
                    continue
                
                description = post.xpath('./description/text()').extract_first()
                
                # Replace '.' in front of months with '//'
                description = re.sub(desc_re_month, '//', description)
                
                # Search for first sentence
                if(desc_re.search(description)):
                    
                    # Get first sentece
                    description = re.split(r'(?:\.+([^0-9]))',description)[0]
                    description = description+'.'
                
                description = description.replace("//",'. ')
            
            else:
                continue
            
            
            # Export data while replacing all "Umlaute"
            yield{
                'headline': headline.translate(special_char_map),
                'title' : title.translate(special_char_map),
                'description': description.translate(special_char_map),
                'pubTime': pub_time.translate(special_char_map),
                'pubDate' :  pub_date.translate(special_char_map),
            }
            

