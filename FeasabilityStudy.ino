/*

  This code is for my feasability study project for PHYS 231 at UNC, SPRING 2018.

  The code below is meant to monitor current during a charge.
  This feasability study was done with the hopeful idea of eventually charging a
  2012 Nissan Leaf with a portable battery as opposed to an outlet or station.

  The current is controlled mechanically using a relay.
  Current is measured by determining the voltage drop accross a "known resistor".
  Once the appropriate current range is reached power is then sent to the relay to allow current to flow to the device.
  Before entering the device, another "known resistor" is placed to maintain measurement of current.
  Once the voltage drop reaches zero, power to the relay is turned back off.

  Created by Carlynn Ferguson April 2018

*/

//pins used to read voltage before and after the "known resistors"
int before_pin1 = A0;
int after_pin1 = A1;
int before_pin2 = A2;
int after_pin2 = A3;
int check_pin = A4;

// blank variables to store values
float before_value1 = 0;
float after_value1 = 0;
float before_value2 = 0;
float after_value2 = 0;
float I1 = 0;
float Vdrop1 = 0;
float I2 = 0;
float Vdrop2 = 0;
float batterycheck = 0;

//set variables
float vbattery = 3; // currently using two AA or 1.5V batteries in series, so 3 volts
float resistor = 0.02; //ohms; both resistors are of the same value
float current = 150; // since I = V/R, if R = 100 Ohms then I = 0.03 Amps

// V = 240 and I = 30 needed for the car
// super low R value needed to maintain that; i.e. 1 Ohm
// need to figure out how to get 240 V battery
// could put light/ thin batteries in parallel and in series to multiply current and voltage

//define the output to communicate with the relay
int relay = 8;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(relay, OUTPUT);

  Serial.println(F("Will Start in 5 Seconds"));
  Serial.println(F(" "));
  delay(5000);

  // start with the RELAY off
  digitalWrite(relay, LOW);
  Serial.println(F("Checking Current..."));
  Serial.println(F(" "));
  delay (1000);

  // read the values from R1:
  //for analogRead, *vbattery/1023.0
  before_value1 = analogRead(before_pin1) * vbattery / 1023.0;
  after_value1 = analogRead(after_pin1) * vbattery / 1023.0;
  
  //check battery voltage via V drop across R
 Vdrop1 = (before_value1) - (after_value1); //volts
  Serial.println(F("Battery Voltage Check:"));
  Serial.println(Vdrop1);
  Serial.println(F(" "));
  delay(1000);
  
  I1 = (Vdrop1) / (resistor); //amps
  Serial.println(F("Initial Current (Amps):"));
  Serial.println(I1);
  Serial.println(F(" "));
  delay(1000);

  //turn on relay if current value is:
  if ((abs(I1 - current) < 0.05 * current) & (I1 <= current)) {     //5% tolerance, may need to decrease with higher current
    digitalWrite(relay, HIGH);
    Serial.println(F("Charging"));
    Serial.println(F(" "));
    delay(1000);
  }
}

void loop() {
  
  /*if (batterycheck < 0.5 * vbattery) {
    digitalWrite(relay, LOW);
    Serial.println(F(" "));
    Serial.println(F("END CHARGE"));
    Serial.println(F(" "));
    Serial.println(F("Battery is > 50% discharged"));
    Serial.println(F("Charging not necessarily complete"));
    delay(1000);
    exit(0);
  }*/

  // read the values from R2:
  before_value2 = analogRead(before_pin2) * vbattery / 1023.0;
  after_value2 = analogRead(after_pin2) * vbattery / 1023.0;

  // calculate the voltage drop to check current before charge
  Vdrop2 = (before_value2) - (after_value2); //volts
  Serial.println(F("Voltage Drop:"));
  Serial.println(Vdrop2);
  Serial.println(F(" "));
  delay (1000);

  I2 = (Vdrop2) / (resistor); //amps

  //turn on relay off current value is too high
  if ((abs(I2 - current) < 0.05 * current) & (I2 <= current)) { //5% tolerance, maybe decrease with higher I
    digitalWrite(relay, LOW);
    Serial.println(F(" "));
    Serial.println(F("CURRENT TOO HIGH"));
    delay(1000);
    digitalWrite(relay, LOW);
    Serial.println(F(" "));
    Serial.println(F("END CHARGE"));
    Serial.println(F(" "));   
    delay(1000);
  }
  
  Serial.println(F("Current Value:"));
  Serial.println(I2);
  Serial.println(F(" "));
  delay (1000);

  //if Vdrop is negative, capacitor is pushing voltage back
  /*if (Vdrop2 < 0) {
    digitalWrite(relay, LOW);
    Serial.println(F(" "));
    Serial.println(F("END CHARGE"));
    Serial.println(F(" "));
    delay(1000);
    Serial.println(F("Voltage Negative"));
    delay(1000);
    exit(0);
  }*/
  
  //if Vdrop is zero, capacitor is already charged
  /*if (Vdrop2 == 0) {
    digitalWrite(relay, LOW);
    Serial.println(F(" "));
    Serial.println(F("END CHARGE"));
    Serial.println(F(" "));
    Serial.println(F("Voltage = 0"));
    delay(1000);
    exit(0);
  }*/

  //if Vdrop is 95% voltage of battery, turn off relay
  if (Vdrop2 >= 0.95 * (vbattery)) {
    digitalWrite(relay, LOW);
    Serial.println(F(" "));
    Serial.println(F("END CHARGE"));
    Serial.println(F(" "));
    delay(1000);
    Serial.println(F("~95% Battery Capacity"));
    delay (1000);
    exit(0);
  }
}
