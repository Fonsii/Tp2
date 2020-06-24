#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <functional>
#include <type_traits>

template <typename T>
class List {
public:
	List(const T h, const T t) : head(h), tail(t) {};
	List(const T h) : head(h) {};
	T head;
	T tail;

	template<typename V>
	T operator()(std::function<T(T, T)> func) {
		return func(head, tail);
	}

	friend std::ostream& operator<<(std::ostream& str, List<T> list) {
		str << "(";
		str << list.head;
		str << " ";
		str << list.tail;
		str << ")";
		return str;
	}
	
	std::string toString() {
		std::ostringstream ss;
		ss << "(";
		if (isSingleValue((*this).head)) {
			ss << (*this).head;
		}
		else {
			ss << (*this).getHead();
		}		
		ss << " ";
		if (isSingleValue((*this).tail)) {
			ss << (*this).tail;
		}
		else {
			ss << (*this).getTail();
		}
		ss << ")";
		return ss.str();
	}
	std::string getHead() {
		return "(" + (*this).head + ")";
	}
	std::string getTail() {
		return "(" + (*this).tail + ")";
	}
private:
	bool isSingleValue(std::string command) {
		bool result = true;
		if (command.size() >= 1) {
			if (command.find(' ') != std::string::npos) {
				result = false;
			}
		}
		return result;
	}
};

template<typename T>
List<T> list(const T h, const T t) {
	return List<T>(h, t);
}

template<typename T>
List<T> list(const T h) {
	return List<T>(h);
}


