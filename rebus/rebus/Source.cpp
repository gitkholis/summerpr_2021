#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;
string glob_a, glob_b, glob_c;
bool flag;
map <char, int> glob_cipher;

/* ��������, ���������� �� � ����� v, ������ i */
bool is_declared(char& i, vector<char>& v) {			
	bool dec = false;
	for (char j : v) {
		dec = dec || (i == j);
	}
	return dec;
}
/* � ������������ �� ������� cipher, �� ������ s (�� ���������� ����),
   ���������� � ��������. 
   ��������, s = ABC; cipher = {"A" - 0, "B" - 5, "C" - 2};
   �����, ����������� ������ ���� ������� �����:
   string s = "052" */
void decode_string(string& s, map <char, int>& cipher) {
	map <char, int> ::iterator it = cipher.begin();
	for (int i = 0; i < (int)s.size(); i++) {
		it = cipher.find(s[i]);
		s[i] = it->second + '0';
	}
}
/* ��� �������, ��������� �� string � int ���������� �� ���� ������:
   ��������: string s = "052" --> int s = 52 */
int MyAtoi(string& a) {
	int s = 0, d = 0;
	for (int i = a.size() - 1; i >= 0; i--) {
		char tmp = a[i];
		s += (int)((int)tmp - 48) * (int)pow(10, d);
		d++;
	}
	return s;
}
/* ��� ������� ���������, ���� �� ������ ������������ ������ ����� */
bool same_val(map <char, int>& cipher) {
	map <char, int> ::iterator it = cipher.begin();
	for (int i = 0; it != cipher.end(); it++, i++) {
		map <char, int> ::iterator it1 = cipher.begin();
		for (int j = 0; it1 != cipher.end(); it1++, j++) {
			if (it->second == it1->second && it1->first != it->first) return false;
		}
	}
	return true;
}
/* ������ ���������� �� ���� ����� � ���� */
void printNumbers(string& a, string& b, string& c) {
	ofstream fout("output.txt", ios_base::app);
	fout << a << " + " << b << " = " << c << endl;
	fout.close();
}
/* ������ ���������������� �������, ������� ������ ������� ���� 
   ��������� ��������� - ����������� */
void find(string& a, string& b, string& c, map <char, int>& cipher) {
	/*�������� �������, ��������� ����� ������������ � �������� ������ */
	decode_string(a, cipher);									
	decode_string(b, cipher);
	decode_string(c, cipher);
	/* �������� ������ ��������� � ����� */
	int aInt = MyAtoi(a), bInt = MyAtoi(b); int cInt = MyAtoi(c);
	/*if (aInt < pow(10, glob_a.size() - 1))
		return;
	if (bInt < pow(10, glob_b.size() - 1))
		return;
	if (cInt < pow(10, glob_c.size() - 1))
		return;*/
	int expression = aInt + bInt;
	/* �������� ���������� �������� � ��������, ���� �� ������ � ������� ������������
	   ������ ����� */
	if (expression == cInt && same_val(cipher)) {
		a = to_string(aInt), b = to_string(bInt), c = to_string(cInt);
		/* ���� ��, �� ������ ���������� � ���� */
		printNumbers(a, b, c);
		/* ������, ���� ���� ����� �������, ��� �������, ����� � ����� ������ ���������
		   �������� ��������������� ��������� */
		flag = true;
	}
	/* �������� ��������� ���������� ���� ��������������� ������ ������� */
	int sum = 0;
	map <char, int> ::iterator it = cipher.begin();
	for (int i = 0; it != cipher.end(); it++, i++) {
		sum += it->second * (int)pow(10, cipher.size() - 1 - i);
	}
	sum += 1;	
	/* ����, ��� ��������� ���������� ���� ��������������� ������
	   ������� ���� ���������, �� ����������� ������ ��������� � 
	   ��������������� ���������� */
	if (sum >= pow(10, cipher.size())) {
		if (flag)
			throw runtime_error("\nThere are no other solutions");   
		throw runtime_error("There are no solution");
	}		
	/* ����� ���������� ���������� ���� ���������� � ������� */
	int p = (int)pow(10, cipher.size() - 1);
	for (map <char, int> ::iterator it1 = cipher.begin(); it1 != cipher.end(); it1++) {
		it1->second = sum / p;
		sum -= it1->second * p;
		p /= 10;
	}

	/* ����� �������� �������� ���������� �������,
	   �.�. ���� ������� ��������� */
	a = glob_a;
	b = glob_b;
	c = glob_c;
	/* ���������� �������� ��� ������� */
	find(a, b, c, cipher);
}

/* ������ ���������������� ������� */
void decode(string& a, string& b, string& c) {
	/* ��������: ����� ������� ���� */
	if ((c.size() < b.size()) or (c.size() < a.size())) throw runtime_error("��� �������");
	string d = a + b + c;
	vector <char> carr;
	for (int i = 0; i < d.size(); i++) {
		if (!is_declared(d[i], carr)) carr.push_back(d[i]);
	}
	/* �������� carr - ��������� �������������� ���� */
	/* �����, ��p��� map - ������������� ������ ����� ���� ����� */
	map <char, int> cipher;
	for (int i = 0; i < carr.size(); i++) {
		cipher.insert(make_pair(carr[i], 0));
	}
	glob_cipher = cipher;
	/* ����, ����������� ���� ��������, �� ������� �� �����������, � ����������� ����� ������� */
	/*while (true) {
		find(a, b, c, cipher);
		a = glob_a;
		b = glob_b;
		c = glob_c;
	}*/
	/* �������� ������ ���������������� ������� */
	find(a, b, c, cipher);
}

int main() {
	setlocale(LC_ALL, "Russian");
	clock_t start_c, end_c;
	start_c = clock();
	try {
		/* ��������� ������� ������ �� ����� */
		char file[50] = "input7.txt";
		string a, b, c;
		ifstream fin(file);
		fin >> a;
		fin >> b;
		fin >> c;
		fin.close();
		/* ��������� � ���������� ���������� ������� ������,
		����� ����� ������������ ��������, ����� ��������� */
		glob_a = a;
		glob_b = b;
		glob_c = c;
		/* � �������� ���� ���������� ���� ������� ������ � ��������
		���������������� ������� */
		ofstream fout("output.txt");
		fout << a << " + " << b << " = " << c << "\n" << endl;
		fout.close();
		decode(a, b, c);
	}
	catch (exception& e) {
		/* ��� ��������� ����������, ��� ��������� ����������� - �������� � ���� ��������� */
		ofstream fout("output.txt", ios_base::app);
		fout << e.what() << endl;
		fout.close();
	}
	end_c = clock();
	/* �������� ����� ������ ��������� */
	ofstream fout("output.txt", ios_base::app);
	fout << "runtime = " << (double)(end_c - start_c) / CLOCKS_PER_SEC << endl;
	fout.close();
	return 0;
}