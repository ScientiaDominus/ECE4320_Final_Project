#include <Arduino.h>
#include "SR04.h"
#include "pins.h"

SR04 sr04_left = SR04(left_echo,left_trig);
SR04 sr04_center = SR04(center_echo,center_trig);
SR04 sr04_right = SR04(right_echo,right_trig);

elapsedMillis ledTimer;

long dist_1, dist_2, dist_3;
float stop;
//long dir_1, dir_2, dir_3;
int p_Read;
int button;


float avg(long, long, long);
//long u_direct();
void distanceUpdate();
void monitorUpdate();

void setup() {
    // put your setup code here, to run once:
    button = 0;
    pinMode(Motor1_dirA, OUTPUT);
    pinMode(Motor1_dirB, OUTPUT);
    pinMode(Motor1_enable, OUTPUT);
    pinMode(Motor2_dirA, OUTPUT);
    pinMode(Motor2_dirB, OUTPUT);
    pinMode(Motor2_enable, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    analogWriteFrequency(Motor1_enable, 500);
    analogWriteFrequency(Motor2_enable, 500);
    Serial.begin(9600);
    delay(1000);
}

void loop() {
    // put your main code here, to run repeatedly:
    if(ledTimer >= 1000) { // heartbeat led
        digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
        ledTimer = 0;
    }
    distanceUpdate();
    monitorUpdate();
    stop = avg(dist_1, dist_2, dist_3);

    if(button == 1){
        if(stop >= 0.10)
        {
            digitalWrite(Motor1_enable, LOW);
            digitalWrite(Motor2_enable, LOW);
        }
        else if(p_Read <= 250){
            digitalWrite(Motor1_enable, LOW);
            digitalWrite(Motor2_enable, HIGH);
        }
        else if(((p_Read >= 450) && (p_Read <= 550)) && (dist_2 <= 50)){
            digitalWrite(Motor1_enable, LOW);
            digitalWrite(Motor2_enable, LOW);
        }
        else if((p_Read >= 750)){
            digitalWrite(Motor1_enable, HIGH);
            digitalWrite(Motor2_enable, LOW);
        }
        else if((p_Read >= 250) && (p_Read <= 750)){
            digitalWrite(Motor1_enable, HIGH);
            digitalWrite(Motor2_enable, HIGH);
        }
    }
    else {
        digitalWrite(Motor2_enable, LOW);
        digitalWrite(Motor1_enable, LOW);
    }
    if(dist_1 <= 30){
        digitalWrite(StMotor_dirA, LOW);
        digitalWrite(StMotor_dirB, HIGH);
        analogWrite(StMotor_enable, 255);
    }
    else if((dist_2 <= 50)){
        digitalWrite(StMotor_dirA, LOW);
        digitalWrite(StMotor_dirB, LOW);
        analogWrite(StMotor_enable, 0);
    }
    else if(dist_3 <= 30){
        digitalWrite(StMotor_dirA, HIGH);
        digitalWrite(StMotor_dirB, LOW);
        analogWrite(StMotor_enable, 255);
    }
    else {
        digitalWrite(StMotor_dirA, LOW);
        digitalWrite(StMotor_dirB, LOW);
        analogWrite(StMotor_enable, 0);
    }
}

float avg(long X, long Y, long Z){
    float dir = 0;
    dir = ((1.0f / X) + (1.0f / Y) + (1.0f / Z)) / 3;
    Serial.print("Average is ");
    Serial.println(dir);
    return dir;
}
/*long u_direct(){

}*/

void distanceUpdate() {
    long temp_1 = sr04_left.Distance();
    long temp_2 = sr04_center.Distance();
    long temp_3 = sr04_right.Distance();
    if (temp_1 >= 100){
        dist_1 = 100;
    }
    else if(temp_1 <= 0){
        dist_1 = 1;
    }
    else {
        dist_1 = temp_1;
    }
    if (temp_2 >= 100){
        dist_2 = 100;
    }
    else if(temp_2 <= 0){
        dist_2 = 1;
    }
    else {
        dist_2 = temp_2;
    }
    if (temp_3 >= 100){
        dist_3 = 100;
    }
    else if(temp_3 <= 0){
        dist_3 = 1;
    }
    else {
        dist_3 = temp_3;
    }
    //dist_1=sr04_left.Distance();
    //dist_2=sr04_center.Distance();
    //dist_3=sr04_right.Distance();
}
void monitorUpdate() {
    Serial.print("Ultrasonic Sensor 1: ");
    Serial.print(dist_1);
    Serial.println("cm");
    Serial.print("Ultrasonic Sensor 2: ");
    Serial.print(dist_2);
    Serial.println("cm");
    Serial.print("Ultrasonic Sensor 3: ");
    Serial.print(dist_3);
    Serial.println("cm");
    p_Read = analogRead(14);
    Serial.println(p_Read);
    button = digitalReadFast(40);
    Serial.println(button);
}