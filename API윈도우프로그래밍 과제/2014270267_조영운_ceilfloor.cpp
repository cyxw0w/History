#include<iostream>
using namespace std;

void fl(float f) {
	if (f - ((int)f)) {	// 소수 부분이 존재
		if (f > 0) {	//양수
			cout << (int)f << endl;
		}
		else {			//음수
			cout << (int)f - 1 << endl;
		}
	}
	else {	//.0으로 끝나는 정수
		cout << (int)f << endl;
	}
}
void ce(float f) {
	if (f - ((int)f)) {	// 소수 부분이 존재
		if (f > 0) {	//양수
			cout << (int)f + 1 << endl;
		}
		else {			//음수
			cout << (int)f << endl;
		}
	}
	else {	//.0으로 끝나는 정수
		cout << (int)f << endl;
	}
}

int main() {
	float num;
	for (int i = 0; i < 4; i++) {
		cout << "실수 입력 : " ;
		cin >> num;
		cout << "floor : ";
		fl(num);
		cout << "ceiling : ";
		ce(num);
	}

	return 0;
}