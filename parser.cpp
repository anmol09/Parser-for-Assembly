#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;


void setArray(char c[]) {
	for (int i = 0; c[i] != '\0'; i++) {
		c[i] = '\0';
	}
}

void cpyArray(char initial[], char final[]) {
	int i;

	for (i = 0; final[i] != '\0'; i++) {
		initial[i] = final[i];
	
	}

	initial[i] = '\0';
}

bool cmpArray(char c[], char c_new[]) {
	for (int i = 0; (c[i] != '\0' || c_new[i] != '\0'); i++) {
		if (c[i] != c_new[i]) {
			return -1;
		}
	}
	return 0;
}