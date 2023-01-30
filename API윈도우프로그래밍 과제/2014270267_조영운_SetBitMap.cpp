#include<iostream>
using namespace std;
class BitSet {
	int BitMap;
public:
	BitSet() { BitMap = 0; }
	BitSet(const char s[]);
	friend ostream& operator<<(ostream& out, BitSet s);
	BitSet operator+(char newElement); //원소 추가
	BitSet operator|(BitSet op2); //합집합
	BitSet operator&(BitSet op2); //교집합
	friend bool operator <=(BitSet op1, BitSet op2); //op1이 op2의 부분집합인지 평가
	friend BitSet operator~(BitSet s); // 여집합(전체 집합 = 소문자 전체)
	friend BitSet operator-(BitSet op1, BitSet op2); //차집합
};
ostream& operator<<(ostream& out, BitSet s) {
	if (!s.BitMap) {
		out << "{ }";
		return out;
	}
	int temp = s.BitMap;
	out << "{";
	for (int i = 0; i < 26; i++) {		//	알파벳
		if (temp & 1)		// 몇번째 비트가 1인가? 0이면 없음 1이면 해당 i번째 알파벳이 존재
			out << (char)('a' + i) << ", ";
		temp >>= 1;						// 우측으로 비트연산하여 비트위에 있는 숫자가 가리키는 것은 다다름
	}
	out << "\b\b}";
	return out;
}
// 생성자함수와 나머지 연산자 함수 정의
BitSet::BitSet(const char s[]) {
	this->BitMap = 0;
	int stack, q = 0;
	char arr[26] = {};	// 밑에 것과 동일하네;;
	/*for (int r = 0; r < 26; r++)	//문자열 널로 초기화
		arr[r] = NULL;*/
	
	for (int i = 0; s[i] != NULL; i++) {	//미리 중복을 제거
		for (int j = i+1; s[j] != NULL; j++) 
			if (s[i] == s[j]) { //중복 발견시
				i++;	//const str이라 새로운 배열에 넣어서 사용					
				j = i + 1;
			}
		if (s[i] == NULL)
			break;
		arr[q] = s[i];
		q++;
	}
	for (int i = 0; s[i] != NULL; i++) {
		stack = 1;
		stack <<= (arr[i] - 'a');	//비트 시프트 후 할당해줘야됨........ =====================
		BitMap += stack;
	}
}
BitSet BitSet::operator+(char newElement) {//문자열 arr에 testing을 저장
	static char arr[26] = {};		//저장된 testing을 tesing으로 바꿔줌
	static int index = 0;	//+연산자가 계속 맴버함수를 재귀함수처럼 불러내는 방식
		//testing 저장
	index++;	//i는 7까지
	//해당 객체에 if문을 사용하여 이번에 들어오는 문자가 arr에 존재하는지 검사
	for (int i = 0; i < index; i++) {
		if (arr[i] == newElement)	//새로 들어오는 원소가 기존 arr에 존재하면 패스
			return *this;
		else
			arr[index] = newElement;
	}	
	int stack = 1;
	stack <<= (newElement - 'a');
	this->BitMap += stack;

	return *this;	
}
BitSet BitSet::operator&(BitSet op2) {
	BitSet temp;	//s1
	int i = this->BitMap; //s2
	int s = op2.BitMap;	//s3
	int stack;
	for(int j = 0; j<26 ; j++){		
		if ((i & 1) && (s & 1)) {	//교집합
			stack = 1;
			stack <<= j;
			temp.BitMap += stack;
		}
		i >>= 1;
		s >>= 1;
	}	
	return temp;
}
BitSet BitSet::operator|(BitSet op2) {
	BitSet temp;	//s1
	int i = this->BitMap; //s2
	int s = op2.BitMap;	//s3
	int stack;
	for (int j = 0; j < 26; j++) {
		if ((i & 1) || (s & 1)) {	//합집합
			stack = 1;
			stack <<= j;
			temp.BitMap += stack;
		}
		i >>= 1;
		s >>= 1;
	}
	return temp;
}
BitSet operator-(BitSet op1, BitSet op2) {
	BitSet temp;	//s4
	int i = op1.BitMap; //s1
	int s = op2.BitMap;	//s3
	int stack;
	for (int j = 0; j < 26; j++) {
		if ((i&1)&&!((i & 1) && (s & 1))) {	//s1에서 교집합을 뺀 나머지
			stack = 1;
			stack <<= j;
			temp.BitMap += stack;
		}
		i >>= 1;
		s >>= 1;
	}
	return temp;
}

BitSet operator~(BitSet s) {
	BitSet Union("abcdefghijklmnopqrstuvwxyz");
	return (Union - s);
}

bool operator <=(BitSet op1, BitSet op2) {
	BitSet ex = op1 - op2;
	//교집합의 비트맵과 자신 집합의 비트맵이 같으면 부분집합	-> 이거는 왜 안되지
	if (!ex.BitMap)	//op1에 op2와의 교집합을 뺐을때 공집합인가 공집합이 아닌가
		return true;
	else
		return false;
}
int main()
{
	BitSet s1, s2("mathematics"), s3, s4;
	s3 = s3 + 't' + 'e' + 's' + 't' + 'i' + 'n' + 'g';	//원소 삽입
	cout << "s1 = " << s1 << "\ns2 = " << s2 << "\ns3 = " << s3 << "\n\n";

	s1 = s2 & s3;		//교집합
	cout << s2 << " ∩ " << s3 << " = " << s1 << "\n\n";

	s1 = s2 | s3;		//합집합
	cout << s2 << " ∪ " << s3 << " = " << s1 << "\n\n";

	s4 = s1 - s3;		//차집합
	cout << s1 << " - " << s3 << " = " << s4 << "\n\n";

	s4 = ~s1;			//여집합
	cout << "~" << s1 << " = " << s4 << "\n\n";

	if (s2 <= s1)
		cout << s2 << " ⊆ " << s1 << "\n\n";
	//s4 = s2 & s1;
	//cout << s4 << "="<< s2 << "\n\n";

	if (!(s2 <= s3))
		cout << s2 << " is not susbset of " << s3 << "\n\n";
	
	return 0;
}