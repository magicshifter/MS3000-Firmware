./arduino.sh verify

if [ $? -eq 0 ]         # Test exit status
then
  echo "Ardino success!"
  ./espFlash.sh

  if [ $? -ne 0 ]        
  then
    ./espFlash.sh
  fi
  if [ $? -ne 0 ]  
  then
    ./espFlash.sh
  fi
# TODO: loop and then readback :)
else  
  echo "why U no work Arduinino?"
fi
