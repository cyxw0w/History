#include<iostream>
using namespace std;

void fl(float f) {
	if (f - ((int)f)) {	// �Ҽ� �κ��� ����
		if (f > 0) {	//���
			cout << (int)f << endl;
		}
		else {			//����
			cout << (int)f - 1 << endl;
		}
	}
	else {	//.0���� ������ ����
		cout << (int)f << endl;
	}
}
void ce(float f) {
	if (f - ((int)f)) {	// �Ҽ� �κ��� ����
		if (f > 0) {	//���
			cout << (int)f + 1 << endl;
		}
		else {			//����
			cout << (int)f << endl;
		}
	}
	else {	//.0���� ������ ����
		cout << (int)f << endl;
	}
}

int main() {
	float num;
	for (int i = 0; i < 4; i++) {
		cout << "�Ǽ� �Է� : " ;
		cin >> num;
		cout << "floor : ";
		fl(num);
		cout << "ceiling : ";
		ce(num);
	}

	return 0;
}