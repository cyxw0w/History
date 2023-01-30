#include<iostream>
using namespace std;

//void sum(int);
void a_sum(int);


int main() {
	int num;
	cout << "Hello World!!" << endl;
	cout << "max multipication number = ";
	cin >> num;
	a_sum(num);
	return 0;
}

void a_sum(int a) {
	int count[10];
	for (int s = 0; s < 10; s++)
		count[s] = 0;
	
	int i = 1, index = 0;	
	//count[index] = 1;
	if (a / 4 >= 1) {
		for (i = 1; i <= a / 4; i++) {
			count[i] = 4 * i;
		}
		if ((a % 4)) {
			count[i] = (4 * (i-1)) + (a % 4);
		}		
	}
	else
		count[1] = a;

	while (count[index+1] != 0) {
		for (int x = 1; x <= i; x++) {
			for (i = count[index] + 1; i <= count[index+1]; i++) { // i x X = i*X
				if (x <= i) {
					cout << i << " x " << x
						<< " = " << x * i << "\t";
				}
				else
					cout << "\t\t";
			}
			cout << endl;
		}
		index++;
		//i = count[index];
	}
}