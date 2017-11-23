#!/bin/bash

mosquitto_sub -h m23.cloudmqtt.com -u xxx -P xxx -p xxx -t 'esp/test' | while read RAW_DATA
do
NIMI=$(echo $RAW_DATA | cut -f1 -d" ")
VIESTI=$(echo $RAW_DATA | cut -f 2 -d" ")


mysql -u esp8266 -pxxxxxxx <<TESTI
USE esp8266
INSERT INTO esps VALUES ("$NIMI", "$VIESTI")
TESTI

done
