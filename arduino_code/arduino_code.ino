// headers
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <Servo.h>

ros::NodeHandle nh; // initialization of the node handle
std_msgs::String servo_pos;
ros::Publisher pub("Servo_position",&servo_pos);// initialization of the publisher

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
int DSD = 15; //Default Servo Delay 

const int selectPins[3] = {2, 4, 7}; // S0~2, S1~4, S2~7 
const int zInput = A0; // Connect common (Z) to A0 (analog input)
int val = 0;
//Declaration of functions
int get_firstnum(int num, int len);
int get_angle(int num, int len);
int get_length(int num);
int* get_position_voltage(void);
String add_string(int posnum);
 
void servo_control(const std_msgs::Int16& cmd_msg){
  Servo servo2cmd;
  int len = get_length(cmd_msg.data); // get the length of cmd for later function
  int firstnum = get_firstnum(cmd_msg.data,len);// get the firstnum of cmd to differentiate which servo will be used.
  int angle = get_angle(cmd_msg.data,len);
  //decide which servo will be used
  switch (firstnum){
    case 1:
      servo2cmd = servo1;
      break;
    case 2:
      servo2cmd = servo2;
      break;
    case 3:
      servo2cmd = servo3;
      break;
    case 4:
      servo2cmd = servo4;
      break;
    case 5:
      servo2cmd = servo5;
      break;
    case 6:
      servo6.write(angle);// servo6 is different, rotate 360 degree
      break;}
  //Drive the servo to the specified angle
  
  //int pos = get_position_voltage()[firstnum-1];   //from calculate result.
  int pos = servo2cmd.read();       // for the angle test from built-in function
  
  // servo execution except servo6
  if (firstnum != 6){
    if(angle <= pos){
      for(int i =pos; i>=angle;i--){
      servo2cmd.write(i);
      delay(DSD);}}
    else {for(int i =pos; i<=angle;i++){
      servo2cmd.write(i);
      delay(DSD);}}}}
      
 //initialization of the subscriber
ros::Subscriber <std_msgs::Int16> sub("Servo_control", &servo_control);
 
void setup(){
    servo1.attach(3);servo2.attach(5);servo3.attach(6);
    servo4.attach(9);servo5.attach(10);servo6.attach(11);
    servo6.write(91);
    
    //initialize pinmode for multiplexer
    for (int i=0; i<3; i++){
      pinMode(selectPins[i], OUTPUT);
      digitalWrite(selectPins[i], HIGH);}
    pinMode(zInput, INPUT); // Set up Z as an input
    
    // initialize ROS node handle
    nh.initNode();
    nh.advertise(pub);// publish
    nh.subscribe(sub);}// subscribe
    
void loop(){      
    //assembly the all the servo's position and voltage in one command
    int position_servos[6];
    int voltage_servos[6];
    
    //obtain the position information and put them into the first six of array
    
    //for(int k =0; k<6;k++) position_servos[k]=get_position_voltage()[k];  //for the calculate result
    
    position_servos[0] = servo1.read();
    position_servos[1] = servo2.read();
    position_servos[2] = servo3.read();
    position_servos[3] = servo4.read();
    position_servos[4] = servo5.read();   //for the servo angle test by built-function

    //obtain the voltage information and put them into the last six of array
    for(int j =0; j<6;j++) voltage_servos[j]=get_position_voltage()[j+6];
    
    //assembly the information in one string
    String end_pos = add_string(position_servos[0])+add_string(position_servos[1])+add_string(position_servos[2])+add_string(position_servos[3])+add_string(position_servos[4])/*+add_string(position_servos[5])*/;
    String end_voltage = add_string(voltage_servos[0])+add_string(voltage_servos[1])+add_string(voltage_servos[2])+add_string(voltage_servos[3])+add_string(voltage_servos[4])/*+add_string(voltage_servos[5])*/;
  
    //assembly the two string
    String end_msg =end_pos+"    "+end_voltage;
    
    //change the type of command from string to char*
    char* pos_vol = (char*)end_msg.c_str();
    servo_pos.data = pos_vol;

    pub.publish(&servo_pos);//publish a message
    nh.spinOnce();
    delay(1000);}


int get_firstnum(int num, int len){
  int firstnum;
  switch (len){
    case 2 :
       firstnum = num/10 % 10;
       break;
    case 3 :
       firstnum = num/100 % 10;
       break;
    case 4 :
       firstnum = num/1000 % 10;
       break;}
  return firstnum;}



int get_angle(int num, int len){
  int angle;
  switch (len){
    case 2: 
      angle = num % 10;
      break;
    case 3:
      angle = num % 100;
      break;
    case 4:
      angle = num % 1000;
      break;}
  return angle;}
  
  
int get_length(int num){
  int len= 0;
  while(num){
    num /= 10;
      len++;}
  return len;}

// The selectMuxPin function sets the S0, S1, and S2 pins, 
// accordingly, given a pin from 0-5.
void selectMuxPin(byte pin){
  for (int i=0; i<3; i++){
    if (pin & (1<<i))
      digitalWrite(selectPins[i], HIGH);
    else
      digitalWrite(selectPins[i], LOW);}} 

String add_string(int posnum){
  String text1 = " ";
  String text2= posnum+text1;
  return text2;}
  
int* get_position_voltage(void){
  int value[12];
  for (byte pin=0; pin<=5; pin++){
      selectMuxPin(pin); // Select one at a time
      int inputValue = analogRead(zInput); // and read Z
          inputValue = map(inputValue,0,1023,0,5000);
          value[pin] = (3000-inputValue)/18;   // calculation equation
          value[pin] += 26;                    // calculation equation
          value[pin+6] = inputValue;}
  return value;}
