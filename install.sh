#/bin/bash

# Install needed Packages
sudo apt install libxslt-dev python3 python3-venv python3-pip -y

# Create Virtual Environment
python3 -m venv Scraper/python/

# Activate venv
source Scraper/python/bin/activate

# Install needed Python Libraries
pip install pySerial Scrapy RPi.GPIO

# Create Crontab entries to automatically execute Scripts
sudo tee /etc/cron.d/corona <<-EOF
@reboot /bin/bash "/home/pi/corona/output_button_loop.sh" > /home/pi/corona_loop.log
*/5 * * * * /bin/bash "/home/pi/corona/output.sh" > /home/pi/corona.log
EOF

sudo reboot
