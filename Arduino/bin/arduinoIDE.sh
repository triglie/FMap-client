docker rm ArduinoIDE
echo "Insert the Arduino port (Example: ttyACM0): "
read PORT
docker run -it --privileged -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY -e PORT=$PORT \
       -h $HOSTNAME -v $HOME/.Xauthority:/home/triglie/.Xauthority --name ArduinoIDE youdontneedspotify:ArduinoRadio