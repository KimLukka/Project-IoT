#!/bin/bash

#Get MQTT broker info from text file into variables.
filename1=/home/projekti/bin/mqtt.txt
server=$(cat $filename1 | cut -f1 -d" ")
user1=$(cat $filename1 | cut -f 2 -d" ")
password1=$(cat $filename1 | cut -f 3 -d" ")
port=$(cat $filename1 | cut -f 4 -d" ")

#Using Mosquitto client to subscribe to a topic, import server information from previous text file.
mosquitto_sub -h $server -u $user1 -P $password1 -p $port -t 'esp/test' | while read RAW_DATA
do
#Cut message information into separate variable separated by a delimiter.
NIMI=$(echo $RAW_DATA | cut -f1 -d" ")
VIESTI=$(echo $RAW_DATA | cut -f 2 -d" ")

#Get MYSQL database username and password from text file into variables.
filename2=/home/projekti/bin/user_password.txt
user2=$(cat $filename2 | cut -f1 -d" ")
password2=$(cat $filename2 | cut -f 2 -d" ")

mysql -u $user2 -p$password2 <<TESTI
USE esp8266
INSERT INTO esps (mode, action) VALUES ("$NIMI", "$VIESTI")

TESTI

done
