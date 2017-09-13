#include "database.h"
#include "time.h"
#include <random>
#include <sstream>
//"C:/Users/92036/Documents/C++/test_index100.dat", "C:/Users/92036/Documents/C++/test_value100.dat" 共1073196条数据 不定长value
//"C:/Users/92036/Documents/C++/test_index150.dat", "C:/Users/92036/Documents/C++/test_value150.dat" 共1499999条数据 定长value
void test1(); //存储150w条数据（定长）
void test2();//存储一百万条数据（不定长）
void test3(int size); //存储size条数据（不定长）
void test4();
void test5();
void test6(int size);
void test7(int size);
void test8(int size);
void test9(int size);
void m1();
  
vector<ValueType> r = { "You know you love me","I knowyou care","You shout whenever", "And I'll be there","You want my love, You want myheart","And we will never ever ever beapart","Are we an item ?",
" Girl quit playing","Were just friends"," Or are we saying","So theres another one"," Looks right in my eyes","My first love broke my heart forthe first time",
"And I was like","Baby", "baby", "baby nooo","My baby", "baby", "baby noo","My baby", "baby", "baby nooo","I thought youd always be mine","mine","Baby", "baby", "baby nooo",
"My baby", "baby", "baby noo","My baby", "baby", "baby nooo","I thought youd always be mine","oh oh","For you", "I would have done","whatever","Another chance and we",
"We get together","And wanna play it cool"," About","loosin' you","I'll buy you anything", "I'll buy you","any ring","And i'm in piece ", "baby fix me",
"And you'll shake me til' you wake","me from this bad dream","Im going down", "down", "dooown","And just can't believe my first","love won't be around","Baby", "baby", "baby nooo",
"I thought youd always be mine","When i was 13 i had my first love","Here was nobody to compare my baby","And nobody came between us or","could ever come above",
"She had me goin crazy","Oh i was starstruck.","She woke me up daily dont need","no starbucks","She made my heart pound","Asking for a beat when i see her","in the street",
"And in the school on the playground","But i really wanna see her on the weekends","She knows she got me dazy","Cause she was so amazing and now my heart is breaking",
"But i just keep on sayin","Baby", "baby", "baby nooo","I thought youd always be mine x2","Now Im all gone" };
int main() {

	m1();
}



void test1() {   //存储150w条数据（定长）
	time_t start, end;
	string s = "abcdefghijklmnopqrstuvwxyz";
	string r1, r2;
	float f;
	double tC;
	start = clock();
	DataBase test;

	for (int i = 1; i < 1500000; i++) {
		r1 = s[rand() % 26];
		r2 = s[rand() % 26];
		test.insert(float(2*i+1), r1 + r2);
	
	}
	test.printAll();
	test.close();
	end = clock();
	tC = double(end - start) / CLOCKS_PER_SEC;
	cout << tC;
	cin.get();
	cin.get();
}

void test2() {  //存储一百万条数据（不定长）
	time_t start, end;
	string s = "abcdefghijklmnopqrstuvwxyz";
	string r1, r2;
	float f;
	double tC;
	start = clock();
	DataBase test;

	for (int i = 1; i < 1000000; i++) {
		test.insert(float(i) / 1000, r[rand() % 81]);

	}

	test.close();
	end = clock();
	tC = double(end - start) / CLOCKS_PER_SEC;
	cout << tC;
	cin.get();
	cin.get();
}

void test3(int size) {
	time_t start, end;
	string s = "abcdefghijklmnopqrstuvwxyz";
	string r1, r2;
	float f;
	double tC;
	start = clock();
	DataBase test;

	for (int i = 1; i < size; i++) {
		test.insert(float(i) / 1000, r[rand() % 81]);

	}
	test.close();
	end = clock();
	tC = double(end - start) / CLOCKS_PER_SEC;
	cout << tC;
	cin.get();
	cin.get();
}

void test4() {
	time_t start, end;
	double tC;
	start = clock();
	DataBase test("C:/Users/92036/Documents/C++/test_index100.dat", "C:/Users/92036/Documents/C++/test_value100.dat");
	test.close();
	end = clock();
	tC = double(end - start) / CLOCKS_PER_SEC;
	cout << tC;
	cin.get();
	cin.get();
}



void test5() {
	time_t start, end;
	double tC;
	start = clock();
	DataBase test("C:/Users/92036/Documents/C++/test_index150.dat", "C:/Users/92036/Documents/C++/test_value150.dat");
	test.close();
	end = clock();
	tC = double(end - start) / CLOCKS_PER_SEC;
	cout << tC;
	cin.get();
	cin.get();
}

void test6(int size) {
	time_t start, end;
	float f;
	double tC;
	start = clock();
	DataBase test("C:/Users/92036/Documents/C++/test_index100.dat", "C:/Users/92036/Documents/C++/test_value100.dat"
	);
	for (int i = 1; i < size; i++) {
		f = float(rand() % 1073196)/1000;
	//	test.fetch(f);
		test.remove(f);
	}
	test.close();
	end = clock();
	tC = double(end - start) / CLOCKS_PER_SEC;
	cout << tC;
	cin.get();
	cin.get();
}

void test7(int size) {
	time_t start, end;
	float f;
	double tC;
	start = clock();
	DataBase test("C:/Users/92036/Documents/C++/test_index150.dat", "C:/Users/92036/Documents/C++/test_value150.dat"
	);
	for (int i = 1; i < size; i++) {
		f = float(rand() % 1500000);
		test.fetch(f);
		test.remove(f);
	}
	test.close();
	end = clock();
	tC = double(end - start) / CLOCKS_PER_SEC;
	cout << tC;
	cin.get();
	cin.get();
}

void test8(int size) {
	time_t start, end;
	float f;
	double tC;
	start = clock();
	DataBase test("C:/Users/92036/Documents/C++/test_index100.dat", "C:/Users/92036/Documents/C++/test_value100.dat"
	);
	for (int i = 1; i < size; i++) {
		f = float(rand() % 1073196) / 1000;
		//test.fetch(f);
		test.modify(f,"haha");
		//test.fetch(f);
	}
	test.close();
	end = clock();
	tC = double(end - start) / CLOCKS_PER_SEC;
	cout << tC;
	cin.get();
	cin.get();
}

void test9(int size) {
	time_t start, end;
	float f;
	double tC;
	start = clock();
	DataBase test("C:/Users/92036/Documents/C++/test_index150.dat", "C:/Users/92036/Documents/C++/test_value150.dat"
	);
	for (int i = 1; i < size; i++) {
		f = float(rand() % 3000000);
		//test.fetch(f);
		test.modify(f,"haha");
		//test.fetch(f);
	}
	test.close();
	end = clock();
	tC = double(end - start) / CLOCKS_PER_SEC;
	cout << tC;
	cin.get();
	cin.get();
}


void m1() {
	string line;
	DataBase test;
	string order;
	while (true) {
	//	cout << "please enter the order\n";
		stringstream ss;
		getline(cin, line);
		ss << line;
		ss >> order;
		if (order == "close") {
			test.close();
			break;
		}
		else if (order == "insert") {
			KeyType key;
			ValueType value;
			ss >> key;
			ss >> value;
			test.insert(key, value);
		}
		else if (order == "remove") {
			KeyType key;
			ss >> key;
			test.remove(key);
		}
		else if (order == "fetch") {
			KeyType key;
			ss >> key;
			test.fetch(key);
		}
		else if(order == "modify") {
			KeyType key;
			ValueType value;
			ss >> key;
			ss >> value;
			test.modify(key, value);
		}
		else if (order == "printAll") {
			test.printAll();
		}
		else {
			//cout << "wrong order";
			continue;
		
		}
	}
}