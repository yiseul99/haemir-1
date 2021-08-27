#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include <vector>
#include <iostream>
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#define RAD2DEG(x) ((x)*180./M_PI)
#include <sstream>

float g_optimal;
std::vector<float> distances;
std::vector< std::vector <float> > tmp;
int g_num;
int g_count;
float find_optimal_degree(const std::vector<float> distances);

float scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    int count = scan->scan_time / scan->time_increment;
	g_count = count ;
    
	distances.clear();


    for(int i = 0; i < count / 2 ; i++) 
	{
    float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);

    if (scan->ranges[i] > 2 || scan->ranges[i] == 0) 
	  {
		distances.push_back(1);
	  }                                  
      else 
	  {
		distances.push_back(0);
      }
   
    }
    return (find_optimal_degree(distances));
}

std::vector <std::vector <float> > make_second_array(const std::vector<float> distances)
{
    std::vector <float> nums(2);

    std::vector <float> a = {0, 0};

    std::vector <std::vector<float>> array_;
	
   

    int range = 0;
    int first_angle = 0;
    array_.clear();

	for (int i = 1; i < g_count ; i++) {
        
        if (distances[i - 1] == 1 && distances[i] == 0) {
            
            first_angle = i;
      }
        else if (distances[i - 1] == 0 && distances[i] == 1) {

            num[0] = range;
            num[1] = first_angle;
            array.push_back(nums);
            first_angle = 0;
            range = 0;
        }

        else if (distances[i - 1] == 0 && distances[i] == 0) {
            range += 1;
        }
    }
	
	return array_;
}

float select_angle(std::vector< std::vector<float> > array)
{
   float max_range;
   float max_first_angle;
   int i;

   max_range = array[0][0];
   max_first_angle = array[0][1];

   for(i=0; i < g_num - 1;i++)
   {
		if(max_range < array[i][0])
      {
            max_range = array[i][0];
            max_first_angle = array[i][1];
      }

   }
   printf("최대 각의 시작점 =%f 최대 각의 크기=%f\n",max_first_angle,max_range);
   return (max_first_angle - max_range / 2);
}

float find_optimal_degree(const std::vector<float> distances)
{

	float optimal;

   optimal = select_angle(make_second_array(distances));
   optimal = optimal / g_count * 360;
   tmp.clear();

   printf("optimal: %f\n",  optimal);
  // msg = std::to_string(optimal);
	
   g_optimal = optimal;
   
	
   return (optimal);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "seven");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, scanCallback);
	ros::Publisher angle_pub = n.advertise<std_msgs::Float64>("rotate_angle", 1000);
	ros::Rate loop_rate(1);
	while (ros::ok())
	{
		std_msgs::Float64 msg;

        float g_o;
        g_o = g_optimal;
		//std::stringstream ss;
		//ss << g_optimal  ;
		msg.data = g_o;

		ROS_INFO("%f", msg.data);

		angle_pub.publish(msg);

		ros::spinOnce();

		loop_rate.sleep();
	}
    return 0;
}