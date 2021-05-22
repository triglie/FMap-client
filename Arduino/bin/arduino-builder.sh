docker rm ArduinoBuilder
echo "Insert the Arduino port (Example: ttyACM0): "
read PORT
docker run -e PORT=$PORT --name ArduinoBuilder --privileged -it youdontneedspotify:ArduinoRadio
docker rm ArduinoBuilder