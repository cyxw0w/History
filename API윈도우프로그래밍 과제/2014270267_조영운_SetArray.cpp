#include<iostream>
using namespace std;

class Set {
	char p[26]; // 실제 원소들이 저장될 배열
	int size; // 원소의 갯수
public:
	Set() { size = 0; }
	Set(const char *s);
	friend ostream& operator << (ostream &out, Set s); //Set 객체 출력을 연산자
	Set operator+(char newElement);//원소 추가
	Set operator| (Set op2); //합집합
	Set operator&(Set op2); //교집합
	friend bool operator <=(Set op1, Set op2); //op1이 op2의 부분집합인지 평가
	friend Set operator~(Set s); // 여집합(전체 집합 = 소문자 전체)
	friend Set operator-(Set op1, Set op2); //차집합

	void sizeout(Set os) {
		cout << os.size << endl;
	};
};
ostream& operator << (ostream &out, Set s) {
	if (!s.size) {
		cout << "{ }";
		return out;
	}
	out << "{";
	for (int i = 0; i < s.size; i++)
		out << s.p[i] << ", ";
	out << "\b\b}";
	return out;
}
// 생성자함수와 나머지 연산자 함수 정의
Set::Set(const char *s) {
	this->size = 0;
	for (int i = 0; this->p[i] != NULL; i++) {
		for (int j = 0; this->p[j] != NULL; j++) // 중복 처리
			if (this->p[j] == *(s + i)) //기존 배열에 삽입할 s문자가 같을때 다음 문자로 ㄱ
				i++;			
		if (*(s + i) == NULL)	// 만약 이번 문자가 NULL일 경우 for문 종료
			break;
		this->p[size] = *(s + i);
		this->size++;
	}
}
Set Set::operator+(char newElement) {
	for (int i = 0; this->p[i] != NULL; i++) //중복 처리
		if (newElement == this->p[i]) 
			return *this;	//그냥 넣을 생각도 못하게 리턴해버림
	this->p[size] = newElement;
	this->size++;	
	return *this;
}
Set Set::operator|(Set op2) {
	Set temp(*this);
	Set tmp = (*this & op2);
	Set tmp2;
	temp.size = this->size;
	tmp2 = op2 - tmp;	//합칠 피집합에 교집합 제거	
	for (int i = 0; i < tmp2.size; i++) 	// 두 집합 합치기
		temp.p[temp.size++] = tmp2.p[i];
	return temp;
}

Set Set::operator&(Set op2) {
	Set temp;
	int i, s, j = 0;
	for (s = 0; s < op2.size; s++) {
		for (i = 0; i < this->size; i++) {
			if (this->p[i] == op2.p[s]) {
				temp.p[j] = this->p[i];
				j++;
				temp.size++;
			}
		}
	}
	return temp;
}

Set operator-(Set op1, Set op2) {
	Set temp = op1;
	Set tmp = (op1 & op2);	//교집합
	for (int i = 0; i < temp.size; i++) {
		for (int j = 0; j < tmp.size; j++) {
			if (temp.p[i] == tmp.p[j]) {	//교집합 부분이 나오면 그 부분을 지움
				temp.p[i] = NULL;
				for (int s = i; s < temp.size; s++) // 해당 번지 부터 끝에까지 쭈르르 앞으로
					temp.p[s] = temp.p[s + 1];			
				temp.size--;	//해당 집합 크기 줄임
			}
		}	
	}
	return temp;
}

Set operator~(Set s) {
	Set Union("abcdefghijklmnopqrstuvwxyz");
	return (Union - s);
}

bool operator <=(Set op1, Set op2) {
	Set con = (op1 & op2);
	if (con.size == op1.size)	//교집합의 크기가 해당 집합의 크기와 같으면 op2에 속하는 op1은 그자체로 부분집합임
		return true;
	else
		return false;
}

int main() {
	Set s1, s2("mathematics"), s3, s4;
	s3 = s3 + 't' + 'e' + 's' + 't' + 'i' + 'n' + 'g'; //원소 삽입
	//s3.sizeout(s3);
	cout << "s1 = " << s1 << "\ns2 = " << s2 << "\ns3 = " << s3 << "\n\n";
	s1 = s2 & s3;//교집합
	cout << s2 << " ∩ " << s3 << " = " << s1 << "\n\n";
	s1 = s2 | s3;//합집합
	cout << s2 << " ∪ " << s3 << " = " << s1 << "\n\n";
	s4 = s1 - s3;//차집합
	cout << s1 << " - " << s3 << " = " << s4 << "\n\n";
	s4 = ~s1; //여집합
	cout << "~" << s1 << " = " << s4 << "\n\n";
	if (s2 <= s1)//부분집합인지 검사
		cout << s2 << " ⊆ " << s1 << "\n\n";
	if (!(s2 <= s3))
		cout << s2 << " is not susbset of " << s3 << "\n\n";
	return 0;
}