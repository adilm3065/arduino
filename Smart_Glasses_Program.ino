#define trigPin 3

#define echoPin 2

#define motor 5

#define buzzer 11


void setup()
{
pinMode(trigPin, OUTPUT);

pinMode(echoPin, INPUT);

pinMode(motor, OUTPUT);

pinMode(buzzer,OUTPUT);

}

void loop()

{

long duration, distance;

digitalWrite(trigPin, LOW);

delayMicroseconds(2);

digitalWrite(trigPin, HIGH);

delayMicroseconds(10);

digitalWrite(trigPin, LOW);

duration = pulseIn(echoPin, HIGH);

distance = (duration/2) / 29.1;

if (distance < 70)     // This is the distance in cm. you can change the value

{
tone(buzzer, 1000);  

digitalWrite(motor,HIGH);    // When the the distance is below 70cm the buzzer will beep

//digitalWrite(buzzer,HIGH);

} else

{
noTone(buzzer);

digitalWrite(motor,LOW);     // when the distance is greater than 70cm the buzzer won't beep

//digitalWrite(buzzer,LOW);


} delay(500);

}
