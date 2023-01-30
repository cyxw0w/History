/*
S -> aS			   1
S -> bA			   2
A -> d			   3
A -> ccA		   4

parsing table
	a	b	d	c	$
S	1	2
A			3	4
*/
#include<iostream>
#include<string>
using namespace std;

void push(char);
void pop();
void printStack();
void checkStack(char);
void error();
void accept();

string str;		//inputstring
string parse;	//leftparse
string stack = "$S";	//stack

int step = 1;
int index = 0;
char nextSymbol;
char get_nextSymbol() {
	nextSymbol = str[index++];
	return nextSymbol;
}
void push(char sym) {
	stack.push_back(sym);
}
void pop() {
	stack.pop_back();
}
void printStack() {
	cout << step << "\t"
		<< stack << "\t\t"
		<< str.substr(index)
		<< "\t\t" << parse << endl;
}
void checkStack(char sym) {
	switch (sym) {
	case 'a':
		if (stack.back() == 'S') {
			push(sym);	
			parse.push_back('1');
			step++;
			printStack();

			pop();
			step++;
			printStack();
		}
		else
			error();
		break;
	case 'b':
		if (stack.back() == 'S') {			
			parse.push_back('2');
			pop();
			push('A');
			push('b');
			step++;
			printStack();

			pop();
			step++;
			printStack();
		}
		else
			error();
		break;
	case 'd':
		if (stack.back() == 'A') {
			parse.push_back('3');
			pop();
			push('d');			
			step++;
			printStack();
		}
		else
			error();
		break;
	case 'c':
		if (stack.back() == 'A') {
			push('c');
			step++;
			printStack();
		}
		else if (stack.back() == 'c') {
			push('c');
			step++;
			printStack();

			parse.push_back('4');
			pop();
			pop();
			step++;
			printStack();			
		}
		else
			error();	
		break;
	case '$':
		if (stack.back() == 'd') {
			pop();
			step++;
			printStack();
			accept();
		}
		else
			error();		
	}
}

void init() {
	cout << "Enter a Sentence : ";
	cin >> str;
	cout << "-----------------------------------------------------" << endl;
	cout << " Step\tStack\t\tInput\t\tParse" << endl;
	cout << "-----------------------------------------------------" << endl;
	printStack();
}
void accept() {
	cout << "Accept!!" << endl
		<< "LeftParse : " << parse << endl;
};
void error() {
	cout << "error!!" << endl;
};
int main() {
	init();
	int i = str.size();
	while (i--) {
		get_nextSymbol();
		checkStack(nextSymbol);
	}

	return 0;
}