//header
#include "ros/ros.h" 
#include "std_msgs/Int16.h" 
#include "std_msgs/String.h"
#include <cstdlib> 
#include <sstream> 
#include <cctype>

int strs2num (char* str);                         //function for change from char* to int;

int main(int argc, char **argv)                      //main function
{ 
    ros::init(argc, argv, "PC");        //initialize the main function

    ros::NodeHandle n;                                // initialize node
 
    std_msgs::Int16 msg;              //definition a new "msg", type is Int16(half of int)
    std::stringstream ss;             //definition a new 'ss', type is stringstream
    //initialize publicher
    ros::Publisher chatter_pub = n.advertise<std_msgs::Int16>("Servo_control", 1000); 
      
    ros::Rate loop_rate(10);              //publish rate 10 Hz 
    ss << argv[1] << argv[2];             //assembly first and second input in one sting; 
    int len = ss.str().size();                 //get the length of string ss;
    char numofcmd[4];
    int k =0;
   //definition a new string; string is a accomplished class
    std::string str_check = argv[1];          
    
    //get the number in 'ss'
    for(int j =0;j<len;j++){ 
        if (isdigit(ss.str()[j])) {           //whether the input is a number, if ture, put the number into array
        numofcmd[k]=ss.str()[j];    
        k++;}}
    
    // check if the servo name contains a number
    int num_check = 0;
    for(int k=0;k<str_check.size();k++) if(isdigit(str_check[k])) num_check =1;
	
    int angle_of_cmd = strs2num(argv[2]);    // third input is the angle;
    int servo_of_cmd = numofcmd[0] - '0';    //get the servo, char turn to int;
    
    //check if the input more or less than 3 arguments, enter also be counted
    if (argc != 3){
	ROS_WARN("Please input the right form: servox angle");
	return 1;} 
    
    //check the input, ...str_check.find, find a str in str_check, when dont, return string::npos = -1.
    if (len > 9 || servo_of_cmd>6 || servo_of_cmd < 1 || str_check.find("servo") == str_check.npos|| num_check == 0) {
	ROS_WARN("Please input the right servoname or number(1-6) or angle!(0-180)");
	return 1;}

    //decide which servo should be used, and check if the command over the angle limit.
    switch (servo_of_cmd) {
	case 1:
	    if (angle_of_cmd<24 || angle_of_cmd > 158){
		ROS_WARN("limit of servo1's angle is between 40 and 150!");		
		return 1;}
	    break;
	case 2:
	    if (angle_of_cmd <0 || angle_of_cmd > 180){
		ROS_WARN("limit of servo2's angle is between 0 and 180!");		
		return 1;}
	    break;
	case 3:
	    if (angle_of_cmd <0 || angle_of_cmd > 180){
		ROS_WARN("limit of servo3's angle is between 0 and 180!");		
		return 1;}
	    break;
	case 4:
	    if (angle_of_cmd <0 || angle_of_cmd > 180){
		ROS_WARN("limit of servo4's angle is between 0 and 180!");		
		return 1;}
	    break;
	case 5:
	    if (angle_of_cmd <0 || angle_of_cmd > 180){
		ROS_WARN("limit of servo5's angle is between 0 and 180!");		
		return 1;}
	    break;
	case 6:
	    if (angle_of_cmd <0 || angle_of_cmd > 180){
		ROS_WARN("limit of servo6's command is between 0 and 180!");		
		return 1;}
	    break;}

    //prompt about the command
    if (servo_of_cmd != 6) ROS_INFO("servo%d will rotate to %d degree.", servo_of_cmd, angle_of_cmd);
	
	//servo6 is a continuous servo,
    else if (angle_of_cmd < 91) ROS_INFO("servo6 will clockwise rotate with speed %d.\n with 91 degree servo6 will be stoped", 91-angle_of_cmd);
    else if (angle_of_cmd > 91) ROS_INFO("servo6 will anticlockwise rotate with speed %d.\n with 91 degree servo6 will be stoped", angle_of_cmd-91);
    else ROS_INFO("servo6 will stop!");
    
    int num = strs2num(numofcmd);    	//change the data type from char array to int.
    
    while (ros::ok()) 
    { 
       msg.data = num;                  //transmit the data to the message object which will be sent

        chatter_pub.publish(msg);       //publish message
    
        ros::spinOnce();                //for subscrition, it's no use here but am best to put
        
        loop_rate.sleep(); }            // sleep for the time remaining to hit 10Hz publish rate
 
    return 0; 
    }

//change the data type from char array to int
int strs2num(char* str){
     int num;
     std::stringstream str2num(str);
     str2num>>num;
     return num;}

