
#include <ros.h> 
#include <Wire.h>
#include <std_msgs/Float64.h> 
#include <std_msgs/String.h>
#include <string.h>
#include <MLX90393.h> 
#include <Servo.h>
Servo servo_left;
Servo right;
Servo left;

MLX90393 mlx;
MLX90393::txyz data; 
std_msgs::Float64 angle; 
std_msgs::Float64 msg; 
ros::NodeHandle  nh;
byte motor_left = 8;
byte motor_right = 10;

String str;
float way_degree;
float rotate_angle;
float c = 0.5;
void messageCb(const std_msgs::Float64& msg)
{
 // Serial.println("msg.data");
  way_degree = msg.data;
}

float vel_plus(float rotate_angle)
{
    float velocity;
    velocity = 1600 + c * rotate_angle;
    return (velocity);
}

float vel_minus(float rotate_angle)
{
    float velocity;
    velocity = 1600 - c * rotate_angle;
    return (velocity);
}


ros::Subscriber <std_msgs::Float64> sub("gps_xy", messageCb);


void setup()
{
  nh.initNode(); 
  Serial.begin(9600);
  Serial.println("MLX90393 Read Example");
  Wire.begin();
  byte status = mlx.begin();

  //Report status from configuration
  Serial.print("Start status: 0x");
  if(status < 0x10) Serial.print("0"); 
  Serial.println(status, BIN);
  
  mlx.setGainSel(1);
  mlx.setResolution(0, 0, 0); //x, y, z
  mlx.setOverSampling(0);
  mlx.setDigitalFiltering(0);
  left.attach(motor_left);
  right.attach(motor_right);

  left.writeMicroseconds(1500); // send "stop" signal to ESC.
  right.writeMicroseconds(1500); // send "stop" signal to ESC.

  nh.subscribe(sub);
  delay(7000); // delay to allow the ESC to recognize the stopped signal
}




void loop()
{
  String str;
  
  float h_angle; 
  mlx.readData(data); //Read the values from the sensor
  float x = float(data.x) -365 ;
  float y = float(data.y) - 357 ;
  float heading = atan2(y, x);

  if(heading < 0)
  heading += 2 * M_PI;
  delay(10);
  Serial.print("heading:\t");
  delay(10);
  Serial.println(heading * 180/M_PI);

  nh.spinOnce(); 
  

  int signal = heading;
  float degree;
  
  heading = heading * 180/M_PI;
   
 
   rotate_angle = way_degree - heading;



if (0<= h_angle && h_angle <= 90 && 0<= way_degree && way_degree <= 90) {
    
    if (h_angle <= way_degree) {

        float rotate_angle = h_angle - way_degree ; 

        left.writeMicroseconds(vel_plus(rotate_angle));
        right.writeMicroseconds(vel_minus(rotate_angle));}

    else {

        float rotate_angle = way_degree - h_angle;

        left.writeMicroseconds(vel_minus(rotate_angle));
        right.writeMicroseconds(vel_plus(rotate_angle)); }}


else if (0<= h_angle && h_angle <= 90 && 270<= way_degree && way_degree <= 360) {

        float rotate_angle = 360 + (h_angle + way_degree);

        left.writeMicroseconds(vel_plus(rotate_angle));
        right.writeMicroseconds(vel_minus(rotate_angle)); }


else if (270<= h_angle && h_angle <= 360 && 0 <= way_degree && way_degree <= 90) {
    
        float rotate_angle = 360 - (h_angle - way_degree);

        left.writeMicroseconds(vel_minus(rotate_angle));
        right.writeMicroseconds(vel_plus(rotate_angle)); }

    
else if (270<= h_angle && h_angle <= 360 && 270<= way_degree && way_degree <= 360) {
    
    if (h_angle <= way_degree) {

        float rotate_angle = way_degree - h_angle ;

        left.writeMicroseconds(vel_minus(rotate_angle));
        right.writeMicroseconds(vel_plus(rotate_angle)); }


    else {

        float rotate_angle = h_angle;

        left.writeMicroseconds(vel_plus(rotate_angle));
        right.writeMicroseconds(vel_minus(rotate_angle)); }
}
   delay(1000);
}