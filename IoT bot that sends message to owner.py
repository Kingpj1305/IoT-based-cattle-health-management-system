import pandas as pd
from urllib.request import urlopen
import json
import time
import pywhatkit
import winsound
# from win32com.client import Dispatch
import pyttsx3



READ_API_KEY='13PLSKBTZBM6OE0S'
CHANNEL_ID= '1776987'


while True:
    TS = urlopen("http://api.thingspeak.com/channels/%s/feeds/last.json?api_key=%s" \
                       % (CHANNEL_ID,READ_API_KEY))

    response = TS.read()
    data=json.loads(response)


    a = data['created_at']
    b = float(data['field1'])
    c = float(data['field2'])
    d = float(data['field3'])
    e = float(data['field4'])
    f = float(data['field5'])
    print(b)
    print(c)
    print(d)
    print(e)
    print(f)
    print("\n")
    number = '+919303576613'
    print("\n")
    if b!=0:
        if b<48 or b>84 or f<36 or f>40:
            message = "Cow's not well"
            pywhatkit.sendwhatmsg_instantly(number, message)
            TS.close()
            frequency = 2500

            duration = 5000
            winsound.Beep(frequency, duration)
            engine = pyttsx3.init()
            engine.say("cow is not well")

            engine.runAndWait()
            time.sleep(300)


    elif b == 0:
        message = "cow is not well"
        pywhatkit.sendwhatmsg_instantly(number, message)
        TS.close()
        frequency = 2500
        duration = 5000

        winsound.Beep(frequency, duration)
        engine = pyttsx3.init()
        engine.say(message)-
        engine.runAndWait()
        time.sleep(300)


    else:
        time.sleep(20)