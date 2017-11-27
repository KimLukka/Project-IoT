#!/bin/bash

mosquitto_sub -h m14.cloudmqtt.com -u fbaeyadu -P 8IEoPV8J5yDI -p 17353 -t 'esp/test' | while re$
do
NIMI=$(echo $RAW_DATA | cut -f1 -d" ")
VIESTI=$(echo $RAW_DATA | cut -f 2 -d" ")

filename=/home/projekti/bin/user_password.txt
user=$(cat $filename | cut -f1 -d" ")
password=$(cat $filename | cut -f 2 -d" ")

mysql -u $user -p$password <<TESTI
USE esp8266
INSERT INTO esps (mode, action) VALUES ("$NIMI", "$VIESTI")

TESTI

done
