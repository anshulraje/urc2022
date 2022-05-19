#include <ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#define DIR1 10               //connections to be checked
#define PWM1 11
#define DIR2 6                   //1 6inch 2 4inch
#define PWM2 5
#define POT1 A4
int i=0;
int j=0;
int k=0;
int m=0;
int potent=0;
int maxl=0;
int minl=0;
#define ldrPin 22
int flagoff=0;
int flagon=0;
int direction6inch=0;
int count=0;
int count2=0;
ros::NodeHandle n;
geometry_msgs::Twist vels;
ros::Publisher pub1("velocitiesm", &vels);
int vel1,vel2,vel3,vel4,vel5;
void callback(const geometry_msgs::Twist& msg)
{
  int potent=analogRead(POT1);
  float x = msg.linear.x;  //
  if(x>0.8){
    digitalWrite(DIR1,LOW);//extension(blue-right) 
    direction6inch=0;
    vel1 = 255;
    vel2=0;
    i=2;
  }
  else if(x<-0.8){
    digitalWrite(DIR1,HIGH);//retraction
    direction6inch=1;
    vel1 = 255;
    vel2=0;
    i=1;
  }
  else{
    vel1=0;
  }
  float y = msg.linear.y;// extension(tape right)
  if(y>0.8){
    digitalWrite(DIR2,LOW);
    direction6inch=0;
    vel2 = 255;
    vel1=0;
    i=2;
  }
  else if(y<-0.8){
    digitalWrite(DIR2,HIGH);//retraction
    direction6inch=1;
    vel2 = 255;
    vel1=0;
    i=1;
  }
  else{
    vel2 = 0;
  }
int ldrstatus=digitalRead(ldrPin);
if(ldrstatus==1){
  flagoff=1;
}
if(ldrstatus==0){
  flagon=1;
}

if(flagoff==1){
  int ldrstatus2=digitalRead(ldrPin);
  if(ldrstatus2==0){
    if(direction6inch==1){
      count++;
      flagoff=0;
    }
    if(direction6inch==0){
      count--;
      flagoff=0;
    }
  }
}
if(flagon==1){
  int ldrstatus2=digitalRead(ldrPin);
  if(ldrstatus2==1){
      if(direction6inch==1){
      count++;
      flagon=0;
    }
    if(direction6inch==0){
      count--;
      flagon=0;
    }
  }
}
count2++;
  digitalWrite(PWM1,vel1);
  digitalWrite(PWM2,vel2);
  vels.angular.x=vel2;
  vels.angular.y=count2;
  vels.angular.z=count;
}
ros::Subscriber<geometry_msgs::Twist> sub1("actuators", callback); 
void setup(){
  pinMode(DIR1,OUTPUT);
  pinMode(PWM1,OUTPUT);
  pinMode(DIR2,OUTPUT);
  pinMode(PWM2,OUTPUT);
  pinMode(POT1,INPUT);
  pinMode(9,OUTPUT);
  digitalWrite(9,HIGH);

  n.initNode();
  n.subscribe(sub1);
  n.advertise(pub1);
}

void loop(){
  
  pub1.publish(&vels);
  n.spinOnce();
}
