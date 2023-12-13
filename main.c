#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "sampling.h"

/**

* @breif: Create a .csv file based on the generated data

*/
void create_csv(char* file_name, THREE_AXIS_INFO* data, int* steps){
	// Create/Write the file
	FILE *fp = fopen(file_name, "w");
	// The first line
	fprintf(fp, "\"X-Accelerate(m^2/s)\",\"Y-Accelerate(m^2/s)\",\"Z-Accelerate(m^2/s)\"\n");
	// Write the data
	for(int k = 0; k < DATA_SIZE; k++){
		fprintf(fp, "%lf,%lf,%lf\n", data[k].ax,
							   		 data[k].ay,
							   		 data[k].az);
	}
	// Steps
	for(int k = 0; k < 5; k++){
		fprintf(fp, "\"%d Minute(s) Steps: \",%d\n", k + 1, steps[k]);
	}
	// Close the file
	fclose(fp);
	return;
}

/**

* @breif: Create a new data file and return the steps

*/
int* simulate(void){ 
	// Generate a group of data during 5 minutes
	THREE_AXIS_INFO* ori_data = Data_Generate();
	// Filter the data
	THREE_AXIS_INFO* data = Data_Filter(ori_data);
	// Pick a primary axis
	int primary_axis = PickPrimaryAxis(data);
	// Get thresholds
	double* thresholds = Data_Threshold(ori_data, primary_axis);
	// Dynamic Thresholds Output
	// Create/Write the file
	FILE *fp = fopen("Thresholds.csv", "w");
	// First Line Thresholds
	fprintf(fp, "\"%s\",\"%s\"\n", "Thresholds", "Primary Axis");
	for(int i = 0; i < DATA_SIZE/Dynamic_Max_Count; i++){
		if(i==0){
			fprintf(fp, "%lf,%d\n", thresholds[i],primary_axis);
		}
		else{
			fprintf(fp, "%lf\n", thresholds[i]);
		}
	}
	// close it
	fclose(fp);
	// Get the steps
	int* steps = Get_Steps(data, primary_axis, thresholds);
	// Output Original Data
	create_csv("Original_Data.csv", ori_data, steps);
	// Output Filtered Data 
	create_csv("Filtered_Data.csv", data, steps);
	// Print to the screen
	for(int k = 0; k < 5; k++){
		printf("%d Minute(s) Steps: %d\n", k+1, steps[k]);
	}
	return steps;
}

int main(){
	// Simulate for once
	simulate();
	return 0;
}

