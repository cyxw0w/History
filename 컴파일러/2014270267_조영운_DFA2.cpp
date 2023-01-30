#include<iostream>
#include<string>
using namespace std;

const char FinalState1 = 'B';
const char FinalState2 = 'D';

static int index = 0;
static string symbol;
char state(char, char);
char s_get(string);
int main() {
	char currentstate = 'A';
	char nextsymbol;
	cout << "input (a,b string) : ";
	cin >> symbol;

	nextsymbol = s_get(symbol);
	while (symbol[index++] != NULL) {
		currentstate = state(currentstate, nextsymbol);
		nextsymbol = s_get(symbol);
	}
	if (currentstate == FinalState1 || currentstate == FinalState2)
		cout << "Accept" << endl;
	else
		cout << "FAIL" << endl;
	return 0;
}
//상태 확인, 심볼 값에 따른 다음 상태로 전환 후 상태값 return;
char state(char x, char n) {
	switch (x) {
	case 'A':	//start state : A, q0
		if (n == 'a')
			return 'B';
		else
			return 'A';
	case 'B':	//Final state : B, q1q2
		if (n == 'a')
			return 'D';
		else
			return 'C';
	case 'C':	//state : C, q1
		if (n == 'a')
			return 'E';
		else			
			break;
	case 'D':	//Final state : D, q0q1q2
		if (n == 'a')
			return 'D';
		else
			return 'E';
	case 'E':	//state : E, q0q1
		if (n == 'a')
			return 'D';
		else
			return 'A';
	}
	
}
char s_get(string s) {
	return s[index];
}