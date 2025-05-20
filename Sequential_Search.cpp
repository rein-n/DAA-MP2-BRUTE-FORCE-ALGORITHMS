#include <stdio.h>

//traditional sequential search function
int sequentialSearch(int arr[], int arr_length, int target);

//improved sequential search function (uses four-pointer, array-division, and flagging approach)
int sequentialSearch_improved(int arr[], int arr_length, int target);

int main(){
	int arr[10] = {1,2,3,4,5,6,7,8,9,10}; //array to test the searching 
	int arr_length = sizeof (arr)/sizeof(arr[0]); //size of the array
	int target; //target element at the array
	
	//prompts the user for the target element (for the demo)
	printf("Enter the number you want to search in the array: "); 
	scanf("%d", &target);
	
	//call the search function and store it in a variable
	int result = sequentialSearch(arr, arr_length, target); 
	
	if (result != -1){
		printf("\nTarget is found at index %d\n\n", result); //print the index of the element if found
	} else {
		printf("Target not found\n\n");
	}
	
	printf("Improved Sequential Search:\n"); 
	
	//function call
    sequentialSearch_improved(arr, arr_length, target); 
	
	return 0;
	
}

int sequentialSearch(int arr[], int arr_length, int target){
	int search_step = 0;//initialize the passes tracker
	
	printf("Traditional Sequential Search:\n");
	for (int i = 0; i < arr_length; i++){
		search_step++; 
		//print the element at each pass
		printf("Step %d: element: %d at index %d\n", search_step, arr[i], i);
		if (arr[i] == target){
			return i; //target found
		}
	}
	return -1; //element is not found 
}

int sequentialSearch_improved(int arr[], int arr_length, int target){
	int mid = arr_length / 2; //middle point of the array division
	int left_start = 0; //left start index of the first part
	int left_end = mid - 1; //left end index of the first part
	int right_start = mid; //right start index of the second part
	int right_end = arr_length - 1; //right end index of the second part
	int search_step = 0; //initialize the step/pass counter
	int found = 0; //flag for the index
	
    while ((left_start <= left_end || right_start <= right_end) && !found){
    	search_step++; 
    	printf("Step %d:\n", search_step); //track the step/pass count
    	
    	//check the four pointers for the index of the element and print if found
    	
    	if (left_start <= left_end){ //left start pointer
    		printf("Left start index %d: %d\n", left_start, arr[left_start]); 
    		if (arr[left_start++] == target){
    			printf("\nTarget is found at index %d\n", left_start - 1); 
    			found = 1;
    			break;
			}
		} 
		
		if (left_end >= left_start){ //left end pointer
			printf("Left end index %d: %d\n", left_end, arr[left_end]);
			if (arr[left_end--] == target){
				printf("\nTarget is found at index %d\n", left_end + 1); 
    			found = 1;
    			break;
			}
		}
		
		if (right_start <= right_end){ //right start pointer
			printf("Right start index %d: %d\n", right_start, arr[right_start]); 
			if (arr[right_start++] == target){
				printf("\nTarget is found at index %d\n", right_start - 1);
				found = 1;
				break;
			}
			
		}
		
		if (right_end >= right_start){ //right end pointer
			printf("Right end index %d: %d\n\n", right_end, arr[right_end]);
			if (arr[right_end--] == target){
				printf("\nTarget is found at index %d\n", right_end + 1);
				found = 1;
				break;
			}
		}
	}
	
	//element not found
	if (!found){
		printf("Target not found\n");
		return -1;
	}
	
	return 0;
}
