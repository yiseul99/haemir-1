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
std::vector<float> real_distances;
std::vector< std::vector <float> > tmp;
int g_num;
int g_count;
float min;
float find_optimal_degree(const std::vector<float> distances);
float min_distance(const std::vector<float> real_distances);


float scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    int count = scan->scan_time / scan->time_increment;
	g_count = count;
    int a = 0;
    int b = 0;
    min = 0;
    
	distances.clear();
	real_distances.clear();

	for(int i = 0 ; i <= count ; i++) 
	{
      float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
       
   
		if (scan->ranges[i] > 0.7 || scan->ranges[i] == 0) 
	  {
		  distances.push_back(1);
		  
	  }                                  
		else 
      {
		  distances.push_back(0);
      }
		std::cout << "scan->ranges[" << degree  << "] = " << scan->ranges[i] << std::endl;
      
		if (scan -> ranges[i] > 0.12)
	{
		real_distances.push_back(scan->ranges[i]);
		min = min_distance(real_distances);
	}
      
   
    }

		std::cout << scan -> ranges[0] << std::endl;
		printf("min : %f  \n",min);
    return (find_optimal_degree(distances));
}

std::vector <std::vector <float> > make_second_array(const std::vector<float> distances)
{
	std::vector <float> nums(2);

   std::vector <float> a = {0, 0};

   std::vector <std::vector<float>> array;
	int k = 0;
   int cnt = 0;
    int c = 0;
    int b = 0;
    int d = 0;
    int f = 0;
   int total = 0;
   g_num = 0;
	array.clear();
	tmp.clear();
std::cout << "g_count is " << g_count << std::endl;
	for (int i = 1; i < g_count  ; i++)
	{
      if (distances[i - 1] == 1 && distances[i] == 1)
      {
        cnt++;
      }
      else if (distances[i - 1] == 1 && distances[i] == 0 && cnt != 0)
      {
         
		 nums[0] = cnt;
		 nums[1] = i;
	
	std::cout << "range is " << cnt / g_count * 360 << " , angle is " << i / g_count * 360<< std::endl; 
         array.push_back(nums);
         g_num++;
      }
 else if (distances[i - 1] == 0 && distances[i] == 1)
      {
         cnt = 0;
      }
	}
	
	tmp = array;
	array.clear();

	
	return tmp;
}

float select_angle(std::vector< std::vector<float> > array)
{
   float max;
   float num;
   int i;
        //배열크기,입력값,최소값,최대값
	
   max = 0;
   num = 0;

      // 배열 첫번째 값을 최소 최대값으로 설정
	std::cout << "g_num is " << g_num << std::endl;
   for(i=0; i < g_num ;i++)
   {
		if(max < array[i][0])
      {
            max = array[i][0];
            num = array[i][1];
		std::cout << "max is " << array[i][0] / g_count * 360 << " , num is " << array[i][1] / g_count * 360<< std::endl; 
      }
		else{
	std::cout << "max is " << array[i][0] / g_count * 360 << " , num is " << array[i][1] / g_count * 360<< std::endl; 
		}
               // 최대값과 비교해 더 크면 최대값에 저장
   }

	num = num / g_count * 360;
	max = max / g_count * 360;

   printf("num=%f max=%f\n",num,max);
   return (num - max / 2);
}

float find_optimal_degree(const std::vector<float> distances)
{

	float optimal;
   optimal = select_angle(make_second_array(distances));
   //optimal = optimal / g_count * 360;
   tmp.clear();
   printf("optimal: %f\n",  optimal);
  // msg = std::to_string(optimal);
	
   g_optimal = optimal;
   
	
   return (optimal);
}


float min_distance(const std::vector<float> real_distances)
{
	float min = *min_element(real_distances.begin(), real_distances.end());
	return min;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "seven");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, scanCallback);
	ros::Publisher angle_pub = n.advertise<std_msgs::Float64>("rotate_angle", 1000);
	ros::Publisher min_pub = n.advertise<std_msgs::Float64>("minimum_distance", 1000);
	ros::Rate loop_rate(1);
	while (ros::ok())
	{
		std_msgs::Float64 msg;
		std_msgs::Float64 mini;

        float g_o;
        g_o = g_optimal;
		//std::stringstream ss;
		//ss << g_optimal  ;
		msg.data = g_o;
		mini.data = min;

		ROS_INFO("%f", msg.data);
		ROS_INFO("%f", mini.data);

		angle_pub.publish(msg);
		min_pub.publish(mini);

		ros::spinOnce();

		loop_rate.sleep();
	}
    return 0;
}
