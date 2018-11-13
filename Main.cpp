/* @Author
*  Name:Mehmet Ali Han Tutuk
* Date: 01.11.2018  */
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>

#define SIZE 128

using namespace std;
void sort(char *list, int size);

struct occur_node {
	char character;
	occur_node *next;
	int occurence;
};
struct vocab_node {
	char character;
	vocab_node *next;
	occur_node *list;
};
struct vocab_list
{
	vocab_node *head;
	void create();
	void print();
	void add_char(char);
	void add_occurence(char, char);
	int get_occurence(char);
	int get_union_occurence(char, char);
};
struct language_model {
	vocab_list *vocabularylist;
	void readData(const char *);
	double calculateProbability(char, char);
};

// all of this for compile command prompt in linux if remove argc and argv parameter and change statement in the main function you may compile the code in every environment
int main(int argc, char* argv[]) {  

	language_model language; // create new struct language

	if (argc == 2) {

		language.readData(argv[1]); // read data from file
		language.vocabularylist->print(); // print
	}
	else {

		language.readData(argv[1]); // read data from file
		cout << language.calculateProbability(argv[2][0], argv[3][0])<<endl; // get occurence 2 character whic is given and printout

	}
	return 0;
}

void vocab_list::create() { 
	head = NULL;
}
void vocab_list::add_char(char c) {


	if (head == NULL) { // if it is empty create a new one

		head = new vocab_node;
		head->character = c;
		head->next = NULL;
		head->list = NULL;

	}
	else {
		vocab_node *iter = head;
		while (iter->next != NULL) {

			iter = iter->next;
		}
		vocab_node *last = new vocab_node;

		iter->next = last;
		last->character = c;
		last->next = NULL;
		last->list = NULL;
	}

}
void vocab_list::print() {

	vocab_node *iter = head;
	occur_node *occur;
	while (iter != NULL) {
		occur = iter->list;

		cout << iter->character << ":" << endl;
		while (occur != NULL) {
			if (occur->character == ' ') {
				cout << "\t<" << "<SP>" << "," << occur->occurence << ">" << endl;

			}
			else {
				cout << "\t<" << occur->character << "," << occur->occurence << ">" << endl;
			}

			occur = occur->next;

		}
		iter = iter->next;	}

}
void vocab_list::add_occurence(char first, char second) {

	vocab_node *iter = new vocab_node;
	iter = head;

	while (iter->next != NULL) { // searching which list element is same the first character
		if (iter->character == first) {

			break;
		}
		else {
			iter = iter->next;
		}

	}

	if (iter->list == NULL) { // for first element

		occur_node *occur = new occur_node;

		occur->character = second;
		occur->occurence = 1;
		iter->list = occur;
		occur->next = NULL;
	}
	else {
		occur_node *occur = new occur_node;
		occur = iter->list;

		while (occur->next != NULL && occur->character != second) {

			occur = occur->next;


		}
		if (occur->character == second) {
			occur->occurence++;


		}
		else {
			occur_node *temp = new occur_node;

			temp->character = second;
			temp->occurence = 1;
			occur->next = temp;
			temp->next = NULL;


		}
	}
}
int vocab_list::get_occurence(char first) {

	vocab_node *iter = new vocab_node;
	iter = head;
	int total = 0;
	if (int(first) > 64 && first < 91) { // for case sensitive

		first = char(int(first) + 32);
	}

	while (iter->next != NULL) { // searching which list element is same the first character
		if (iter->character == first) {

			break;
		}
		else {
			iter = iter->next;
		}
	}
	if (iter->list == 0) {

		return total;
	}
	else {
		occur_node *occur = new occur_node;
		occur = iter->list;
		while (occur != NULL) {

			total += occur->occurence;
			occur = occur->next;


		}
	}

	return total;
}
int vocab_list::get_union_occurence(char first, char second) {

	vocab_node *iter = new vocab_node;
	iter = head;
	int total = 0;
	if (int(first) > 64 && first < 91) { // for case sensitive

		first = char(int(first) + 32);
	}
	if (int(second) > 64 && second < 91) { // for case sensitive

		second = char(int(second) + 32);
	}
	while (iter->next != NULL) { // searching which list element is same the first character
		if (iter->character == first) {

			break;
		}
		else {
			iter = iter->next;
		}

	}
	if (iter->list == 0) {

		return total;
	}
	else {
		occur_node *occur = new occur_node;
		occur = iter->list;
		while (occur != NULL) {

			if (occur->character == second) {

				total = occur->occurence;
				break;
			}
			else {

				occur = occur->next;
			}

		}

	}

	return total;
}
void language_model::readData(const char *file) {


	char *list = new char[SIZE];

	char all[10000];
	for (int i = 0; i < SIZE; i++) {

		list[i] = '|';
	}
	for (int i = 0; i < 10000; i++) {

		all[i] = '|';
	}

	ifstream input(file);
	int size = 0;
	int alls = 0;

	string str;
	while (!input.eof()) {

		getline(input, str);

		for (unsigned int i = 0; i < str.length(); i++) {

			all[alls] = str[i]; // all file input
			alls++;

			if (size == 0) {
				if (int(str[i]) > 64 && str[i] < 91) { // for case sensitive

					str[i] = char(int(str[i]) + 32);
				}

				list[size] = str[i];
				size++;
			}
			int num = 0;
			for (int j = 0; j < size; j++) {

				if (int(str[i]) > 64 && str[i] < 91) { // for case sensitive

					str[i] = char(int(str[i]) + 32);
				}
				if (str[i] != list[j]) {

					num++;

				}

			}

			if (num == size) {

				list[size] = str[i];  // only spesific character 

				size++;

			}
		}

	}
	input.close();
	char *all_last = new char[alls];
	char *last = new char[size];

	for (int i = 0; i < size; i++) {

		last[i] = list[i];
	}
	for (int i = 0; i < alls; i++) {
		if (int(all[i]) > 64 && all[i] < 91) { // for case sensitive

			all[i] = char(int(all[i]) + 32);
		}

		all_last[i] = all[i];
	}

	sort(last, size); // sort alphabetically
	

	vocabularylist = new vocab_list;

	vocabularylist->create();

	for (int i = 0; i < size; i++) {

		vocabularylist->add_char(last[i]); // add all chaacter in list which is sorted alphabeticly
	}

	for (int i = 0; i < alls - 1; i++) {

		vocabularylist->add_occurence(all_last[i], all_last[i + 1]); // addd occurene for all elements in string which is given from input.txt 
	}

}
double language_model::calculateProbability(char first, char second) {

	int up = 0, down = 0;
	up = vocabularylist->get_union_occurence(first, second);
	down = vocabularylist->get_occurence(first);
	double prob = (double)up / (double)down;

	return prob;

}
void sort(char *list, int size) {

	for (int m = 0; m < size - 1; m++)  // sort ASCII number ordered
	{
		for (int n = m + 1; n < size; n++)
		{

			if (list[m] > 96 || list[m] < 123) {
				if (list[m] > list[n])
				{
					char temp;
					temp = list[m];
					list[m] = list[n];
					list[n] = temp;
				}
			}
		}
	}
	char *temp = new char[size];
	int num = 0;
	int index = 0;
	while (num < size) {  // add character in list to other list except special character

		if (list[num] < 123 && list[num]>96) {

			temp[index] = list[num];
			index++;
		}
		num++;
	}
	for (int num = 0; num < size; num++) {       // add special character to new list from list         
		if (list[num] < 97 || list[num]>122) {

			temp[index] = list[num];
			index++;
		}

	}
	for (int i = 0; i < size; i++) { // list = temp all element back to old list alphabetically ordered

		list[i] = temp[i];
	}
}

