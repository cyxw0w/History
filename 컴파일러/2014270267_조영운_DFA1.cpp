#include<iostream>
#include<string>
using namespace std;

const int FinalState = 3;
static int index = 0;
static string symbol;
int state(int, int);
int s_get(string);
int main() {
	int currentstate = 1 , nextsymbol;	
	cout << "input (Binary string) : ";
	cin >> symbol;
		
	nextsymbol = s_get(symbol);		
	while(symbol[index++] != NULL) {		
		currentstate = state(currentstate,nextsymbol);
		nextsymbol = s_get(symbol);
		}
	if (currentstate == FinalState)
		cout << "Accept" << endl;
	else
		cout << "FAIL" << endl;
	return 0;
}
//상태 확인, 심볼 값에 따른 다음 상태로 전환 후 상태값 return;
int state(int x, int n) {
	
	switch (x) {
	case 1:	//start state : p
		if (n == 0)
			return 2;
		else
			return 1;
	case 2:	//state : q
		if (n == 0)
			return 3;
		else
			return 1;
	case 3:	//Final state : r
		if (n == 0)
			return 3;
		else
			return 3;
	}
}
int s_get(string s) {	
	return s[index]-48;
}