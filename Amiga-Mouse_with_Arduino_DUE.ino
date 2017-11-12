/*
  Amiga-Mouse_with_Arduino_DUE 

An Arduino DUE in USB "Host Mode" reads the Movment of an USB-Mouse and emulates a Ball Mouse for an Amiga.
An OTG-Adapter is required.

Due an USB-Mouse consumes more Power than a Ball Mouse, do not use the Joystick Port to power the Arduino and Mouse !
(Some Datasheets say max. 50 mA. It may work but can damage the Amiga)

This program bases on http://arduino.cc/en/Tutorial/MouseController
It was and shall remain public domain :-)

Connect the USB-Mouse on "Native USB" Port with OTG-Adapter.
Power the Arduino and Mouse over the "Programming Port" or other Source (not the Joystick Port).

At the Computer (male):
5   4   3   2   1
  9   8   7   6

1 V—pulse
2 H—pulse
3 VQ—pulse
4 HQ—pulse
5 BUTTON 3(M)   //connected to Ground when pressend
6 BUTTON 1(L)   //connected to Ground when pressend
7 +5V           // 50 mA max ! DO NOT USE to power the Arduino and Mouse !
8 GND
9 BUTTON 2(R)   //connected to Ground when pressend

   H=Y , V=X

Connect the Arduino Pins with the Joystick 1 / Mouse Connector as follow.
*/

//Pins on Arduino DUE
const int MouseDB9Pin1 = 43;
const int MouseDB9Pin2 = 45;
const int MouseDB9Pin3 = 47;
const int MouseDB9Pin4 = 49;
const int MouseDB9Pin5 = 0;
const int MouseDB9Pin6 = 51;
const int MouseDB9Pin7 = 0;
const int MouseDB9Pin8 = 0;
const int MouseDB9Pin9 = 53;


// Require mouse control library
#include <MouseController.h>

// Initialize USB Controller
USBHost usb;

// Attach mouse controller to USB
MouseController mouse(usb);

// variables for mouse button states
boolean   leftButton = false;
boolean middleButton = false;
boolean  rightButton = false;


//Mouse Wheels
int MouseWheelH = 0;
int MouseWheelV = 0;

int MouseXchange = 0;
int MouseYchange = 0;


void WheelH (int Position){
    switch (Position) {
        case 0: {
            digitalWrite(MouseDB9Pin1, LOW );
            digitalWrite(MouseDB9Pin3, HIGH );
            } break;

        case 1: {
            digitalWrite(MouseDB9Pin1, LOW );
            digitalWrite(MouseDB9Pin3, LOW );
            } break;

        case 2: {
            digitalWrite(MouseDB9Pin1, HIGH);
            digitalWrite(MouseDB9Pin3, LOW );
            } break;

        case 3: {
            digitalWrite(MouseDB9Pin1, HIGH);
            digitalWrite(MouseDB9Pin3, HIGH);
            } break;
    } //switch
} //WheelH()


void WheelV (int Position){
    switch (Position) {
        case 0: {
            digitalWrite(MouseDB9Pin2, LOW );
            digitalWrite(MouseDB9Pin4, HIGH );
            } break;

        case 1: {
            digitalWrite(MouseDB9Pin2, LOW );
            digitalWrite(MouseDB9Pin4, LOW );
            } break;

        case 2: {
            digitalWrite(MouseDB9Pin2, HIGH);
            digitalWrite(MouseDB9Pin4, LOW );
            } break;

        case 3: {
            digitalWrite(MouseDB9Pin2, HIGH);
            digitalWrite(MouseDB9Pin4, HIGH);
            } break;
    } //switch
} //WheelV()


void mouseMoved1Step(int StepsX, int StepsY) {
    if (StepsY<0)  {
        for (int StepCounter=0; StepCounter>=StepsY; StepCounter-- ){
            MouseWheelH--;
            if (MouseWheelH > 3) { MouseWheelH = 0; }
            if (MouseWheelH < 0) { MouseWheelH = 3; }
            WheelH(MouseWheelH);
//            delay(2);
        } //for
    } //if
    
    if (StepsY>0)  {
        for (int StepCounter=0; StepCounter<=StepsY; StepCounter++ ){
            MouseWheelH++;
            if (MouseWheelH > 3) { MouseWheelH = 0; }
            if (MouseWheelH < 0) { MouseWheelH = 3; }
            WheelH(MouseWheelH);
//            delay(2);
        } //for
    } //if


    if (StepsX<0)  {
        for (int StepCounter=0; StepCounter>=StepsX; StepCounter-- ){
            MouseWheelV--;
            if (MouseWheelV > 3) { MouseWheelV = 0; }
            if (MouseWheelV < 0) { MouseWheelV = 3; }
            WheelV(MouseWheelV);
//            delay(2);
        } //for
    } //if
    
    if (StepsX>0)  {
        for (int StepCounter=0; StepCounter<=StepsX; StepCounter++ ){
            MouseWheelV++;
            if (MouseWheelV > 3) { MouseWheelV = 0; }
            if (MouseWheelV < 0) { MouseWheelV = 3; }
            WheelV(MouseWheelV);
//            delay(1);
        } //for
    } //if
} //mouseMoved1Step()


void mouseMoved() {
    MouseXchange = mouse.getXChange() ;
    MouseYchange = mouse.getYChange() ;

    mouseMoved1Step(MouseXchange, MouseYchange); //StepsX,StebsY

    MouseXchange = 0;
    MouseYchange = 0;
} //mouseMoved()


// This function intercepts mouse movements while a button is pressed
void mouseDragged() {
    MouseXchange = mouse.getXChange() ;
    MouseYchange = mouse.getYChange() ;

    mouseMoved1Step(MouseXchange, MouseYchange); //StepsX,StebsY

    MouseXchange = 0;
    MouseYchange = 0;
} //mouseDragged()


// This function intercepts mouse button press
void mousePressed() {
    if (mouse.getButton(LEFT_BUTTON)){
        leftButton = true;
        digitalWrite(MouseDB9Pin6, LOW);
    } //if
  
    if (mouse.getButton(MIDDLE_BUTTON)){
        middleButton = true;
        digitalWrite(MouseDB9Pin5, LOW);
    } //if

    if (mouse.getButton(RIGHT_BUTTON)){
        rightButton = true;
        digitalWrite(MouseDB9Pin9, LOW);
    } //if
} //mousePressed()


// This function intercepts mouse button release
void mouseReleased() {
    if (!mouse.getButton(LEFT_BUTTON) && leftButton==true) {
        leftButton = false;
        digitalWrite(MouseDB9Pin6, HIGH);
    } //if

    if (!mouse.getButton(MIDDLE_BUTTON) && middleButton==true) {
        middleButton = false;
        digitalWrite(MouseDB9Pin5, HIGH);
    } //if

    if (!mouse.getButton(RIGHT_BUTTON) && rightButton==true) {
        rightButton = false;
        digitalWrite(MouseDB9Pin9, HIGH);
    } //if
} //mouseReleased()


void setup()
{
    pinMode(MouseDB9Pin1, OUTPUT);      
    pinMode(MouseDB9Pin2, OUTPUT);      
    pinMode(MouseDB9Pin3, OUTPUT);      
    pinMode(MouseDB9Pin4, OUTPUT);      
    pinMode(MouseDB9Pin5, OUTPUT);      
    pinMode(MouseDB9Pin6, OUTPUT);      
//  pinMode(MouseDB9Pin7, OUTPUT);  // +5V : 50 mA max ! DO NOT USE to power the Arduino and Mouse !
//  pinMode(MouseDB9Pin8, OUTPUT);  // GND
    pinMode(MouseDB9Pin9, OUTPUT);

    digitalWrite(MouseDB9Pin1, LOW);
    digitalWrite(MouseDB9Pin2, LOW);
    digitalWrite(MouseDB9Pin3, LOW);
    digitalWrite(MouseDB9Pin4, LOW);
    digitalWrite(MouseDB9Pin5, HIGH);  //Button M not pressed
    digitalWrite(MouseDB9Pin6, HIGH);  //Button L not pressed
//  digitalWrite(MouseDB9Pin7, - );  // +5V
//  digitalWrite(MouseDB9Pin8, - );  // GND
    digitalWrite(MouseDB9Pin9, HIGH);  //Button R not pressed
} //setup()


void loop()
{
    //Process USB tasks
    usb.Task();
} //loop()
