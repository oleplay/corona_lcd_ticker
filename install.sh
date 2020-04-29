#/bin/bash

sudo apt install libxslt-dev python3 python3-venv python3-pip -y

python3 -m venv Scraper/python/

source Scraper/python/bin/activate

pip install pySerial Scrapy RPi.GPIO

sudo tee /etc/cron.d/corona <<-EOF
@reboot /bin/bash "/home/pi/corona/output_button_loop.sh" > /home/pi/corona_loop.log
*/5 * * * * /bin/bash "/home/pi/corona/output.sh" > /home/pi/corona.log
EOF

sudo reboot
