#include<iostream>
#include<fstream>
using namespace std;
// 20180612
// 20181128

inline bool ends_with (std::string const & value, std::string const & ending) {
    if (ending.size() > value.size ()) 
		return false;
    return std::equal (ending.rbegin (), ending.rend (), value.rbegin ());
}

void deauthorize (string name, string &output) {
	name = name + ' ';
	int pos = 0, m = 2, fflag = 0;
	output = "";
	string token;
	while ((pos = name.find ("subsp.", pos + 1)) != -1)
		name.insert (pos + 6, " ");
	while ((pos = name.find ("var.", pos + 1)) != -1)
		name.insert (pos + 4, " ");
	while ((pos = name.find ("f.", pos + 1)) != -1)
		name.insert (pos + 2, " ");
	while ((pos = name.find ('(')) != -1)
		name[pos] = ' ';
	while ((pos = name.find (')')) != -1)
		name[pos] = ' ';
	while ((pos = name.find ("  ")) != -1)
		name.erase (pos, 1);
	while ((pos = name.find ("×")) != -1)
		name.erase (pos, 3);
	while ((pos = name.find (' ')) != -1) {
		m --;
		token = name.substr(0, pos + 1);
		if ((token == "var. ") || (token == "subsp. ")) {
			m = 1;
		}
		if ((m == 0)) {
			if (int(token[0]) < int('a')) {
				output = "";
				return;
			}
			if (token == "sp. ") {
				output = "";
				return;
			}
		}
		if ((fflag == 1) && (token.length () > 3) && (int(token[0]) >= int('a'))) {
			m = 0;
			output += "f. ";
			fflag = 0;
		}
		if ((fflag == 0) && (token == "f. ")) {
			fflag = 1;
			m = -1;
		}
		if (m >= 0) {
			output += token;
		}
		name.erase (0, pos + 1);
	}
	output.erase (output.length () - 1, 1);
	if (ends_with (output, "sp")) {
		output = "";
		return;
	}
}	

int main (void) {
	const char *file_name = "TSV_INPUT.txt";
	ifstream fin (file_name);
	if (!fin) {
		cout << "INPUT FILE 'TSV_INPUT.txt' NOT FOUND!" << endl;
		system("pause");
		return -1;
	}
	int a[100], nn, thereis13 = 0;
	cout << "HOW MANY COLUMNS DO YOU WANT?" << endl;
	cin >> nn;
	cout << "WHAT ARE THEIR PLACES (COUNTING FROM THE LEFT, STARTING FROM 1)?" << endl;
	for (int i = 0; i < nn; i ++) {
		cin >> a[i];
		if (a[i] == 13)
			thereis13 = 1;
	}
	cout << "KEEP THE AUTHOR CITATION FOLLOWING THE LATIN NAME? PRESS 1 TO RETAIN, PRESS 0 TO DISCARD:" << endl;
	cin >> thereis13;
	// thereis13 = ~thereis13;
	ofstream fout ("TSV_OUTPUT.txt");
	string line;
	int n = 1;
	cout << "WORKING! MAKE TAKE SOME TIME..." << endl;
	while (getline (fin, line)) {
		if (n & 131071 == 0) {
			cout << n << endl;
		}
		n ++;
		int pos = 0, m = 0;
		string token[1024];
		while ((pos = line.find ('\t')) != -1) {
			token[m] = line.substr (0, pos);
			line.erase (0, pos + 1);
			m ++;
		}
		if (thereis13 == 0)
			deauthorize (token[12], token[12]);
		const char tab = '\t';
		for (int i = 0; i < nn - 1; i ++)
			fout << token[a[i]-1] << tab;
		fout << token[a[nn-1]-1] << endl;
	}
	fin.close ();
	fout.close ();
	cout << n - 1 << " RECORDS OJBK!" << endl;
	system("pause");
	return 0;
}
