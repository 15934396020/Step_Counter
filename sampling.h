#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "DataInfo.h"

// Sample every 20 ms
#define Sampling_Cycle 20
// Dynamic Maximum/Minimum Sampling Period
#define Dynamic_Max_Count 50
// A step is considered to be taken when n consecutive samples exceed the threshold value
#define Walking_Continuous_Period 6

/**

* @breif: Get threshold array

*/
double* Data_Threshold(THREE_AXIS_INFO* data, int primary_axis){
	// Initialize an array to store the generated data
	static double thresholds[DATA_SIZE/Dynamic_Max_Count];
	// Generate the data
	int j = 0;
	int k = 0;
	double maxx, minn;
	maxx = -90; minn = 90;
	for(int i = 0; i < DATA_SIZE; i++){
		// Find the maximum and minimum value
		switch(primary_axis){
			case X_AXIS:{
				maxx = maxx >= data[i].ax ? maxx : data[i].ax;
				minn = minn <= data[i].ax ? minn : data[i].ax;
				break;
			}
			case Y_AXIS:{
				maxx = maxx >= data[i].ay ? maxx : data[i].ay;
				minn = minn <= data[i].ay ? minn : data[i].ay;
				break;
			}
			default:{
				maxx = maxx >= data[i].az ? maxx : data[i].az;
				minn = minn <= data[i].az ? minn : data[i].az;
				break;
			}
		}
		k++;
		// Get a mean value per Dynamic_Max_Count steps
		if(k%Dynamic_Max_Count==0){
			k = 0;
			// Store the threshold
			thresholds[j] = (maxx+minn)/2;
			j++;
			maxx = -90; minn = 90;
		}
	}
	// Return the data
	return thresholds;
}

/**

* @breif: Get the steps 

*/
int* Get_Steps(THREE_AXIS_INFO* data, int primary_axis, double* thresholds){
	// Initialize an array to store the steps
	static int ans[5];
	int steps = 0;
	// Whether it's in a high level
	bool high_level = false;
	int high_times = 0;
	// Count for every minute
	int k = 0;
	// Compare current value with threshold
	for(int i = 0; i < DATA_SIZE; i++){
		double tmp;
		// Find the value
		switch(primary_axis){
			case X_AXIS:{
				tmp = data[i].ax;
				break;
			}
			case Y_AXIS:{
				tmp = data[i].ay;
				break;
			}
			default:{
				tmp = data[i].az;
				break;
			}
		}
		// Compare with the threshold
		if(tmp>=thresholds[i/Dynamic_Max_Count]){
			if(!high_level){
				high_level = true;
				high_times = 0;
			}
			high_times++;
		}
		else{
			if(high_level){
				high_level = false;
				if(high_times>=Walking_Continuous_Period){
					steps++;
				}
			}
		}
		// Judge whether 1 minute has gone
		k++;
		if(k%3000==0){
			ans[k/3000-1] = steps;
		}
	}
	// Return the data
	return ans;
}

