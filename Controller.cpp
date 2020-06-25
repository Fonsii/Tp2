#include "Controller.h"

#include <windows.h>
#include <iostream>
#include <locale>
#include <memory>
#include <vector>
#include <iterator>
#include <map>
#include <string>
#include <stack>
#include <utility>
#include <algorithm>

typedef List<std::string> list_ptr;

std::map <std::string, list_ptr> listRegistry;

/**
*@brief metodo utilizado para saber si es un dato atomico.
*@param command de tipo std::string que representa el comando.
*@return true si es atomico, false si no lo es.
*/
bool isAtomic(std::string command) {
	bool result = true;
	if (command.size() > 2) {
		if (command.find('(') != std::string::npos) {
			result = false;
		}
	}
	return result;
}

/**
*@brief metodo utilizado para saber si es un valor de una lista.
*@param command de tipo std::string que representa el comando.
*@return true si es un solo valor, false si no lo es.
*/
bool isSingleValue(std::string command) {
	bool result = true;
	if (command.size() >= 1) {
		if (command.find(' ') != std::string::npos) {
			result = false;
		}
	}
	return result;
}

/*
*@brief metodo que borra los parentesis y espacios para guardar los datos en una lista.
*@param str tipo std::string con la string ingresada por el usuario.
*@return std::string de valores.
*/
std::string cleanString(std::string str) {
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	str.erase(std::remove(str.begin(), str.end(), '('), str.end());
	str.erase(std::remove(str.begin(), str.end(), ')'), str.end());
	return str;
}

/*
*@brief metodo que detecta si la cola es nula para imprimir solo la cabeza.
*@param command de tipo std::string que representa el comando.
*@return la cabeza sin el espacio de la cola. 
*/
std::string isOne(std::string command) {
	for (int i = 0; i < command.size(); i++) {
		if (command[i] == ' ') {
			int nextI = i + 1;
			if (command[nextI] == ')') {
				command = cleanString(command);
			}
		}
	}
	return command;
}

/*
*@brief metodo que recorta una string.
*@param str tipo std::string que se va a recortar, indexStart tipo int para saber a partir de que posicion la recorta.
*@return std::string recortada.
*/
std::string stringTrimmed(std::string str, int indexStart) {
	if (!isAtomic(str)) {
		str = str.substr(indexStart, str.size() - 2);
	}
	return str;
}

/*
*@brief metodo que imprime lista.
*@param command de tipo std::string que representa el comando.
*@return objeto lista.
*/
auto getList(std::string command) {
	std::string commandTrimmed = command.substr(1, command.size() - 2);
	std::string head;
	std::string tail;

	if (isAtomic(commandTrimmed)) {
		if (isSingleValue(commandTrimmed)) {
			head = commandTrimmed;
			return list(head);
		}
		else {
			size_t pos = commandTrimmed.find(" ");
			head = commandTrimmed.substr(0, pos);
			tail = commandTrimmed.substr(++pos, commandTrimmed.size());
		}
	}
	else {
		int index = 0;
		int difCont = 0;
		bool foundMid = false;
		bool foundSpace = false;
		while (index < commandTrimmed.size() && !foundMid) {
			if (commandTrimmed[index] == '(') {
				difCont++;
			}
			if (commandTrimmed[index] == ')') {
				difCont--;
				foundSpace = false;
			}
			if (commandTrimmed[index] == ' ') {
				foundSpace = true;
			}
			if (difCont == 0 && foundSpace) {
				foundMid = true;
			}
			else {
				index++;
			}
		}		
		
		head = stringTrimmed(commandTrimmed.substr(0, index), 1);
		tail = stringTrimmed(commandTrimmed.substr(++index, commandTrimmed.size()), 1);	
	}	
	//std::cout << head << std::endl;
	//std::cout << tail << std::endl;
	return list(head, tail);
}

/*
*@brief metodo que interpreta un comando.
*@param command de tipo std::string que representa el comando.
*@return string de ejecucion del comando.
*/
std::string getCommandOrder(std::string command) {
	std::string data;
	bool statementFound = false;
	std::stack<std::string> vCommand;
	while (command.size()> 4 && !statementFound) {
		std::string commandHeader = command.substr(0, 3);
		if (commandHeader == "cab" || commandHeader == "col") {
			vCommand.push(commandHeader);
			command = command.substr(4, command.size()-5);
		}
		else {
			statementFound = true;
		}
	}
	// check if the statement exist.
	auto itr = listRegistry.find(command);
	if (itr != listRegistry.end()) {
		// Now with the stack command and the statement whe can build the data.
		auto tempList = itr->second;
		while (!vCommand.empty())
		{
			std::string tempCommand = vCommand.top();
			vCommand.pop();
			if (tempCommand == "cab") {
				tempList = getList(tempList.getHead());
			}
			else if (tempCommand == "col") {				
				tempList = getList(tempList.getTail());
			}
		}
		data = tempList.toString();
	}
	else {
		data = command;
	}
	return isOne(data);
}

/**
*@brief metodo para imprimir lista con parentesis.
*@param command de tipo std::string que representa el comando.
*@return lista en un string.
*/
auto interpretCommand(std::string command) {
	std::string commandTrimmed = command.substr(1, command.size() - 2);
	std::string head;
	std::string tail;

	if (isAtomic(commandTrimmed)) {
		if (isSingleValue(commandTrimmed)) {
			head = getCommandOrder(commandTrimmed);
		}
		else {
			size_t pos = commandTrimmed.find(" ");
			head = getCommandOrder(commandTrimmed.substr(0, pos));
			tail = getCommandOrder(commandTrimmed.substr(++pos, commandTrimmed.size()));
		}
	}
	else {
		int index = 0;
		int difCont = 0;
		bool foundMid = false;
		bool foundSpace = false;
		while (index < commandTrimmed.size() && !foundMid) {
			if (commandTrimmed[index] == '(') {
				difCont++;
			}
			if (commandTrimmed[index] == ')') {
				difCont--;
				foundSpace = false;
			}
			if (commandTrimmed[index] == ' ') {
				foundSpace = true;
			}
			if (difCont == 0 && foundSpace) {
				foundMid = true;
			}
			else {
				index++;
			}
		}
		head = getCommandOrder(commandTrimmed.substr(0, index));
		tail = getCommandOrder(commandTrimmed.substr(++index, commandTrimmed.size()));
	}

	std::string listData = "(" + head + " " + tail +")";
	return listData;
}

/**
*@brief metodo que realiza consultas a la lista.
*/
void makeQuery() {
	std::string command;
	std::cout << "Ingrese el comando para ingresar una lista. Ejemplos: col(list1) o cab(col(list1))" << std::endl;
	std::cin.ignore();
	std::getline(std::cin, command);
	std::cout << "La consulta introducida fue: " << command << std::endl;
	std::cout << getCommandOrder(command) << std::endl;
}

/**
*@brief metodo que agrega una lista.
*/
void addList() {
	std::string command;
	std::cout << "Ingrese el comando para ingresar una lista." << std::endl;
	std::cout << "Ejemplos: " << std::endl;
	std::cout << "list1 ((3 7) ((5 6) (8 9)))" << std::endl;
	std::cout << "list2 (3 4)" << std::endl;
	std::cout << "list2 (cab(list1) 4)" << std::endl;
	std::cin.ignore();
	std::getline(std::cin, command);		
	
	if (command != "") {
		std::cout << "El comando introducido fue: " << command << std::endl;
		size_t pos = command.find(" ");
		std::string listName = command.substr(0, pos);
		std::string listCommand = command.substr(++pos, command.size());

		if (listCommand[0] == '(' && listCommand[listCommand.size() - 1] == ')') {
			// check if the command is concatenated.
			listRegistry.insert({ listName, getList(interpretCommand(listCommand)) });
		}
		else {
			std::cout << "Los comandos se espera que empiecen con '('" << std::endl;
			std::cout << "Y terminen con ')'" << std::endl;
		}
	}
	else {
		std::cout << "No se ingresó ningún comando." << std::endl;
	}

	
}

/**
*@brief metodo que imprime las listas. 
*/
void printLists() {
	std::cout << std::endl;
	for (auto it = listRegistry.begin(); it != listRegistry.end(); ++it) {
		std::cout << it->first << " : " << it->second.toString() << std::endl;
	}
	std::cout << std::endl;
}

/**
*@brief metodo para limpiar consola dependiendo del sistema operativo, tomado de: https://stackoverflow.com/questions/6486289/how-can-i-clear-console
*/
void clear()
{
#if defined _WIN32
	system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
#elif defined (__APPLE__)
	system("clear");
#endif
}

/**
*@brief metodo para ejecutar el programa.
*/
void Controller::start()
{
	setlocale(LC_ALL, "spanish");
	SetConsoleCP(1252);
	SetConsoleOutputCP(1252);

	//preload some lists for testing.
	//listRegistry.insert({"list0", getList("(4 (7 9))") });
	//listRegistry.insert({"list1", getList("((3 0) (4 (7 9)))")});
	//listRegistry.insert({"list2", getList("((3 7) ((5 6) (8 9)))") });

	// end preload.
	// some commandNestedTests.
	
	// end nested.

	bool finished = false;
	
	int option;

	do {
		std::cout << "1) Ingresar lista." << std::endl;
		std::cout << "2) Realizar consulta." << std::endl;
		std::cout << "3) Imprimir todas las listas." << std::endl;
		std::cout << "4) Salir." << std::endl;
		std::cin >> option;
		clear();

		switch (option)
		{		
		case 1:
			addList();
			std::cin.get();
			break;
		case 2:
			makeQuery();
			std::cin.get();
			break;
		case 3:
			printLists();
			std::cin.get();
			break;
		case 4:
			finished = true;
			break;
		}
		
		
	} while (!finished);
}
