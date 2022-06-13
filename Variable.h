#pragma once
#include <string>
#include <iostream>
using namespace std;
class Variable {
private:
	string name;
	string value;
public:
	Variable(string name,string value){
		this->name = name;
		this->value = value;
	}
	void setVar(string name, string value) {
		this->name = name;
		this->value = value;
	}
	string getName() {
		return name;
	}
	string getValue() {
		return value;
	}
};