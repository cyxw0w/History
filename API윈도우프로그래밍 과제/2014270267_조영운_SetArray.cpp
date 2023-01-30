#include<iostream>
using namespace std;

class Set {
	char p[26]; // ���� ���ҵ��� ����� �迭
	int size; // ������ ����
public:
	Set() { size = 0; }
	Set(const char *s);
	friend ostream& operator << (ostream &out, Set s); //Set ��ü ����� ������
	Set operator+(char newElement);//���� �߰�
	Set operator| (Set op2); //������
	Set operator&(Set op2); //������
	friend bool operator <=(Set op1, Set op2); //op1�� op2�� �κ��������� ��
	friend Set operator~(Set s); // ������(��ü ���� = �ҹ��� ��ü)
	friend Set operator-(Set op1, Set op2); //������

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
// �������Լ��� ������ ������ �Լ� ����
Set::Set(const char *s) {
	this->size = 0;
	for (int i = 0; this->p[i] != NULL; i++) {
		for (int j = 0; this->p[j] != NULL; j++) // �ߺ� ó��
			if (this->p[j] == *(s + i)) //���� �迭�� ������ s���ڰ� ������ ���� ���ڷ� ��
				i++;			
		if (*(s + i) == NULL)	// ���� �̹� ���ڰ� NULL�� ��� for�� ����
			break;
		this->p[size] = *(s + i);
		this->size++;
	}
}
Set Set::operator+(char newElement) {
	for (int i = 0; this->p[i] != NULL; i++) //�ߺ� ó��
		if (newElement == this->p[i]) 
			return *this;	//�׳� ���� ������ ���ϰ� �����ع���
	this->p[size] = newElement;
	this->size++;	
	return *this;
}
Set Set::operator|(Set op2) {
	Set temp(*this);
	Set tmp = (*this & op2);
	Set tmp2;
	temp.size = this->size;
	tmp2 = op2 - tmp;	//��ĥ �����տ� ������ ����	
	for (int i = 0; i < tmp2.size; i++) 	// �� ���� ��ġ��
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
	Set tmp = (op1 & op2);	//������
	for (int i = 0; i < temp.size; i++) {
		for (int j = 0; j < tmp.size; j++) {
			if (temp.p[i] == tmp.p[j]) {	//������ �κ��� ������ �� �κ��� ����
				temp.p[i] = NULL;
				for (int s = i; s < temp.size; s++) // �ش� ���� ���� �������� �޸��� ������
					temp.p[s] = temp.p[s + 1];			
				temp.size--;	//�ش� ���� ũ�� ����
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
	if (con.size == op1.size)	//�������� ũ�Ⱑ �ش� ������ ũ��� ������ op2�� ���ϴ� op1�� ����ü�� �κ�������
		return true;
	else
		return false;
}

int main() {
	Set s1, s2("mathematics"), s3, s4;
	s3 = s3 + 't' + 'e' + 's' + 't' + 'i' + 'n' + 'g'; //���� ����
	//s3.sizeout(s3);
	cout << "s1 = " << s1 << "\ns2 = " << s2 << "\ns3 = " << s3 << "\n\n";
	s1 = s2 & s3;//������
	cout << s2 << " �� " << s3 << " = " << s1 << "\n\n";
	s1 = s2 | s3;//������
	cout << s2 << " �� " << s3 << " = " << s1 << "\n\n";
	s4 = s1 - s3;//������
	cout << s1 << " - " << s3 << " = " << s4 << "\n\n";
	s4 = ~s1; //������
	cout << "~" << s1 << " = " << s4 << "\n\n";
	if (s2 <= s1)//�κ��������� �˻�
		cout << s2 << " �� " << s1 << "\n\n";
	if (!(s2 <= s3))
		cout << s2 << " is not susbset of " << s3 << "\n\n";
	return 0;
}