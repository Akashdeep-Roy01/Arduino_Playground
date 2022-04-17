#include<SoftwareSerial.h>
SoftwareSerial BTSerial(11,12);//Rx|Tx

void setup()
{
    BTSerial.begin(9600);
    pinMode(13, OUTPUT);
}

void loop()
{
    if (BTSerial.available() > 0) {
        char ch = BTSerial.read();
        BTSerial.print("Received: ");
        BTSerial.println(ch);
        if (ch == 'a') {
            digitalWrite(13, HIGH);
        }
        else {
            digitalWrite(13, LOW);
        }
    }
}
