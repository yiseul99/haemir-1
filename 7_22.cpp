#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include <vector>
#include <iostream>

#define RAD2DEG(x) ((x)*180./M_PI)

std::vector<float> distances;
std::vector< std::vector <float> > tmp;
int g_num;
int g_count;
float find_optimal_degree(const std::vector<float> distances);

float scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    int count = scan->scan_time / scan->time_increment;
	g_count = count;
    int a = 0;
    int b = 0;
    printf("[YDLIDAR INFO]: I heard a laser scan %s[%d]:\n", scan->header.frame_id.c_str(), count);
    printf("[YDLIDAR INFO]: angle_range : [%f, %f]\n", RAD2DEG(scan->angle_min), RAD2DEG(scan->angle_max));
    
    for(int i = 0; i < count; i++) 
	{
      float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
   
   
          //printf("[LDS INFO]: angle-distance : [%f, %f, %i]\n", degree, scan->ranges[i], i);
      if (scan->ranges[i] > 1 ) 
	  {
						distances.push_back(1);
//                        printf("angle-distance : [%f, %d]\n",distances[i], a );
                        a++;
	  }                                  
      else 
	  {
						distances.push_back(0);
  //                      printf("angle-distance_n : [%f, %d]\n",distances[i], b );
                        b++;
      }
   
    }
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
	for (int i = 1; i < g_count ; i++)
   {
      if (distances[i - 1] == 1 && distances[i] == 1)
      {
        cnt++;
      }
      else if (distances[i - 1] == 1 && distances[i] == 0)
      {
         
		 nums[0] = cnt;
		 nums[1] = i;
         array.push_back(nums);
         cnt = 0;
         g_num++;
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
   max = array[0][0];
   num = array[0][1];
      // 배열 첫번째 값을 최소 최대값으로 설정
   for(i=0; i < g_num - 1;i++)
   {
		if(max < array[i][0])
      {
            max = array[i][0];
            num = array[i][1];
      }
               // 최대값과 비교해 더 크면 최대값에 저장
   }
   printf("num=%f max=%f",num,max);
   return (num - max / 2);
}

float find_optimal_degree(const std::vector<float> distances)
{

   float optimal;

   optimal = select_angle(make_second_array(distances));
   tmp.clear();
   printf("optimal: %f\n",  optimal);
   return (optimal);
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "ydlidar_client");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, scanCallback);
    ros::spin();

    return 0;
}
