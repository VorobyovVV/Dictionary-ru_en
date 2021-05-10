#include <iostream>
#include "dictpara.h"
#include "array.h"
#include <vector>


typedef Array<DictPara> Container;
class Bad_words {
	std::ofstream BadWords;
public:
	Bad_words(const char* FileName) {
		BadWords.open(FileName);
		if (!BadWords)std::cout << "Cant open File :";
	}
	std::ofstream& getFile() { return BadWords; }
	void inFile(const DictPara& X) { BadWords << X; }
	~Bad_words() {
		BadWords.close();
	}
};

bool ch(Bad_words& Y, DictPara& X) {
	const char* sup = X.get_word().str();
	while (*sup != '\0') {
		if (*sup == ' ') {
			Y.inFile(X);
			return 1;
		}
		++sup;
	}
	if (X.get_word() == '\0') {
		Y.inFile(X);
		return 1;
	}
	else {
		const char* p = X.get_word().str();
		for (; *p != '\0'; p++) {
			int i = 0;
			if (*p == ' ') {
				i++;
				if (i == 2) {
					Y.inFile(X);
					return 1;
				}
			}
		}
	}
	return 0;
}
void Merge(Bad_words& bad, const DictPara X, Container& A) {
	char* p = X.get_word().str_2();
	std::vector<char*> Vec = { p };
	int count = 1;
	
	while (*p != '\0') {
		if (*p == ',') {
			*p = '\0';
			if (*(p - 1) == ' ')*(p - 1) = '\0';
			if (*(p + 1) == ' ')Vec.push_back(p + 2);
			else { Vec.push_back(p++); }
			count++;
		}
		p++;
	}
	for (int i = 0; i < Vec.size(); i++) {
		DictPara Y = DictPara(Vec[i], X.get_translate().str());
		if (!ch(bad, Y))A.push_back(Y);
	}
}
int main()
{
	char fname[] = "en_ru.txt";
	Bad_words File("Bad_words.txt");
	try {
		std::ifstream inFile(fname);
		if (!inFile) throw stroka("Can not open dictionary ");
		DictPara X;
		Container Dictionary;
		int counter = 0;
		long tic = time(NULL);
		while (inFile.peek() != EOF) {
			inFile >> X;
			Merge(File, X, Dictionary);
			++counter;
		}
		inFile.close();
		for (int i = 0; i < Dictionary.size(); i++) {
			std::cout << Dictionary[i];
		}
	

		Dictionary.sort();
		std::ofstream outFile("ru-en.txt");
		for (auto pos = Dictionary.begin(); pos != Dictionary.end(); ++pos)  
			outFile << *pos;

	}
	
	catch (...) {
		std::cout << "error" << std::endl;
	}
	

	return 0;
}