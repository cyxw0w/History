/*
S -> aS			   1
S -> bA			   2
A -> d			   3
A -> ccA		   4
*/
#include<iostream>
#include<string>
using namespace std;

void pS();
void pA();
void pB();
void error();

string str;		//inputstring
string parse;	//leftparse

int index = 0;
char nextSymbol;
char get_nextSymbol() {
	nextSymbol = str[index++];
	return nextSymbol;
}
void pS() {
	if (nextSymbol == 'a') {//1
		get_nextSymbol();
		parse.push_back('1');
		pS();
	}
	else if (nextSymbol == 'b') {//2
		get_nextSymbol();
		parse.push_back('2');
		pA();
	}
	else
		error();
};
void pA() {
	if (nextSymbol == 'd') { //3
		parse.push_back('3');
		get_nextSymbol();		
	}
	else if (nextSymbol == 'c') {
		get_nextSymbol();
		pB();
	}
	else
		error();
};
void pB() {	//4
	if (nextSymbol == 'c') {
		get_nextSymbol();
		parse.push_back('4');
		pA();
	}	
	else
		error();
};
void init() {
	cout << "Enter a Sentence : ";
	cin >> str;
};
void accept() {
	cout << "Accept!!" << endl
	 << "LeftParse : " << parse << endl;
};
void error() {
	cout << "error!!" << endl;
};
int main() {
	init();
	get_nextSymbol();
	pS();
	if (nextSymbol == '$')
		accept();
	else
		error();

	return 0;
}