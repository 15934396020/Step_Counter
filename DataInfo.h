#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define DATA_SIZE 15000
#define FILTER 5
#define pi 3.1415926535
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

typedef struct {
	double ax;
	double ay;
	double az;
} THREE_AXIS_INFO;

/**

* @breif: Generate a random seed from weighted summary of time information

*/
int random_seed_get(){
	time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Split the time information to pieces
    int year = timeinfo->tm_year + 1900;
    int month = timeinfo->tm_mon + 1;
    int day = timeinfo->tm_mday;
    int hour = timeinfo->tm_hour;
    int minute = timeinfo->tm_min;
    int second = timeinfo->tm_sec;
    
    return 10*year+20*month+50*day+80*hour+100*minute+200*second;
}

/**

* @breif: Generate data(angles) in 5 minutes with a sampling cycle 20ms

*/
THREE_AXIS_INFO* Data_Generate(void){
	// Initialize an array to store the generated data
	static THREE_AXIS_INFO data_info[DATA_SIZE];
	// Set random seed by current time to ensure the data will not be identical
	srand(random_seed_get());
	// Generate the data
	for(int i = 0; i < DATA_SIZE; i++){
		// Generate an total accelerate
		// Walking: 0.5~1.5; Running: 2.0~4.0
		// So we assuume that 0.5 <= a <= 4
		double a = (double)((double)(50+rand()%(350+1))/100);
		// The accelerate at which the body swings back and forth when walking
		data_info[i].ax = (double)((double)(rand()%(50+1))/100);
		data_info[i].ax *= (rand()%(100+1)>=50) ? 1 : (-1);
		// The accelerate at which the body swings left and right when walking
		data_info[i].ay = (double)((double)(rand()%(50+1))/100);
		data_info[i].ay *= (rand()%(100+1)>=50) ? 1 : (-1);
		// Ensure the current accelerate is more than sqrt(ax^2+ay^2)
		while(a*a <= (data_info[i].ax*data_info[i].ax+data_info[i].ay*data_info[i].ay)){
			a = (double)((double)(50+rand()%(350+1))/100);
		}
		// The accelerate at which the body spins when walking
		data_info[i].az = sqrt(a*a-data_info[i].ax*data_info[i].ax-data_info[i].ay*data_info[i].ay);
	}
	// Return the data
	return data_info;
}

/**

* @breif: Filter the original data to eliminate noise

*/
THREE_AXIS_INFO* Data_Filter(THREE_AXIS_INFO* data){
	// Initialize an array to store the generated data
	static THREE_AXIS_INFO data_filtered[DATA_SIZE];
	// Generate the data
	int j = 0;
	int k = 0;
	double sum_x, sum_y, sum_z;
	sum_x = sum_y = sum_z = 0;
	for(int i = 0; i < DATA_SIZE; i++){
		// Sum data up temporarily
		sum_x += data[i].ax;
		sum_y += data[i].ay;
		sum_z += data[i].az;
		k++;
		// Get a mean value per FILTER steps
		if(k%FILTER==0){
			k = 0;
			for(int m = FILTER*j; m < FILTER*(j+1); m++){
				data_filtered[m].ax = sum_x/FILTER;
				data_filtered[m].ay = sum_y/FILTER;
				data_filtered[m].az = sum_z/FILTER;
			}
			j++;
			sum_x = sum_y = sum_z = 0;
		}
	}
	// Return the data
	return data_filtered;
}

/**

* @breif: Pick up one primary axis by comparing variances

*/
int PickPrimaryAxis(THREE_AXIS_INFO* data){
	double sum_x, sum_y, sum_z;
	sum_x = sum_y = sum_z = 0;
	// Sum them up
	for(int i = 0; i < DATA_SIZE; i++){
		sum_x += data[i].ax;
		sum_y += data[i].ay;
		sum_z += data[i].az;
	}
	// Get the mean value
	sum_x /= DATA_SIZE;
	sum_y /= DATA_SIZE;
	sum_z /= DATA_SIZE;
	// Store the variance
	double var_x, var_y, var_z;
	var_x = var_y = var_z = 0;
	// Calculate it
	for(int i = 0; i < DATA_SIZE; i++){
		var_x += (data[i].ax-sum_x)*(data[i].ax-sum_x);
		var_y += (data[i].ay-sum_y)*(data[i].ay-sum_y);
		var_z += (data[i].az-sum_z)*(data[i].az-sum_z);
	}
	var_x /= DATA_SIZE;
	var_y /= DATA_SIZE;
	var_z /= DATA_SIZE;
	// Find the max one
	double max_var = var_x;
	max_var = max_var >= var_y ? max_var : var_y;
	max_var = max_var >= var_z ? max_var : var_z;
	// Pick the primary axis
	if(max_var==var_x){
		return X_AXIS;
	}
	else if(max_var==var_y){
		return Y_AXIS;
	}
	else{
		return Z_AXIS;
	}
}

