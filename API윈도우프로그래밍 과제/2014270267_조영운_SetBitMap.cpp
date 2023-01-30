#include<iostream>
using namespace std;
class BitSet {
	int BitMap;
public:
	BitSet() { BitMap = 0; }
	BitSet(const char s[]);
	friend ostream& operator<<(ostream& out, BitSet s);
	BitSet operator+(char newElement); //���� �߰�
	BitSet operator|(BitSet op2); //������
	BitSet operator&(BitSet op2); //������
	friend bool operator <=(BitSet op1, BitSet op2); //op1�� op2�� �κ��������� ��
	friend BitSet operator~(BitSet s); // ������(��ü ���� = �ҹ��� ��ü)
	friend BitSet operator-(BitSet op1, BitSet op2); //������
};
ostream& operator<<(ostream& out, BitSet s) {
	if (!s.BitMap) {
		out << "{ }";
		return out;
	}
	int temp = s.BitMap;
	out << "{";
	for (int i = 0; i < 26; i++) {		//	���ĺ�
		if (temp & 1)		// ���° ��Ʈ�� 1�ΰ�? 0�̸� ���� 1�̸� �ش� i��° ���ĺ��� ����
			out << (char)('a' + i) << ", ";
		temp >>= 1;						// �������� ��Ʈ�����Ͽ� ��Ʈ���� �ִ� ���ڰ� ����Ű�� ���� �ٴٸ�
	}
	out << "\b\b}";
	return out;
}
// �������Լ��� ������ ������ �Լ� ����
BitSet::BitSet(const char s[]) {
	this->BitMap = 0;
	int stack, q = 0;
	char arr[26] = {};	// �ؿ� �Ͱ� �����ϳ�;;
	/*for (int r = 0; r < 26; r++)	//���ڿ� �η� �ʱ�ȭ
		arr[r] = NULL;*/
	
	for (int i = 0; s[i] != NULL; i++) {	//�̸� �ߺ��� ����
		for (int j = i+1; s[j] != NULL; j++) 
			if (s[i] == s[j]) { //�ߺ� �߽߰�
				i++;	//const str�̶� ���ο� �迭�� �־ ���					
				j = i + 1;
			}
		if (s[i] == NULL)
			break;
		arr[q] = s[i];
		q++;
	}
	for (int i = 0; s[i] != NULL; i++) {
		stack = 1;
		stack <<= (arr[i] - 'a');	//��Ʈ ����Ʈ �� �Ҵ�����ߵ�........ =====================
		BitMap += stack;
	}
}
BitSet BitSet::operator+(char newElement) {//���ڿ� arr�� testing�� ����
	static char arr[26] = {};		//����� testing�� tesing���� �ٲ���
	static int index = 0;	//+�����ڰ� ��� �ɹ��Լ��� ����Լ�ó�� �ҷ����� ���
		//testing ����
	index++;	//i�� 7����
	//�ش� ��ü�� if���� ����Ͽ� �̹��� ������ ���ڰ� arr�� �����ϴ��� �˻�
	for (int i = 0; i < index; i++) {
		if (arr[i] == newElement)	//���� ������ ���Ұ� ���� arr�� �����ϸ� �н�
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
		if ((i & 1) && (s & 1)) {	//������
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
		if ((i & 1) || (s & 1)) {	//������
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
		if ((i&1)&&!((i & 1) && (s & 1))) {	//s1���� �������� �� ������
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
	//�������� ��Ʈ�ʰ� �ڽ� ������ ��Ʈ���� ������ �κ�����	-> �̰Ŵ� �� �ȵ���
	if (!ex.BitMap)	//op1�� op2���� �������� ������ �������ΰ� �������� �ƴѰ�
		return true;
	else
		return false;
}
int main()
{
	BitSet s1, s2("mathematics"), s3, s4;
	s3 = s3 + 't' + 'e' + 's' + 't' + 'i' + 'n' + 'g';	//���� ����
	cout << "s1 = " << s1 << "\ns2 = " << s2 << "\ns3 = " << s3 << "\n\n";

	s1 = s2 & s3;		//������
	cout << s2 << " �� " << s3 << " = " << s1 << "\n\n";

	s1 = s2 | s3;		//������
	cout << s2 << " �� " << s3 << " = " << s1 << "\n\n";

	s4 = s1 - s3;		//������
	cout << s1 << " - " << s3 << " = " << s4 << "\n\n";

	s4 = ~s1;			//������
	cout << "~" << s1 << " = " << s4 << "\n\n";

	if (s2 <= s1)
		cout << s2 << " �� " << s1 << "\n\n";
	//s4 = s2 & s1;
	//cout << s4 << "="<< s2 << "\n\n";

	if (!(s2 <= s3))
		cout << s2 << " is not susbset of " << s3 << "\n\n";
	
	return 0;
}