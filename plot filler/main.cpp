#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h> 
using namespace std;

/// <summary>
/// ���� ���������� �����, 
/// ����������� �������-������������
/// </summary>
/// <param name="istr">����� �����</param>
/// <param name="str">������ �����</param>
/// <param name="sep_str">������-�����������</param>
void getphrase(istream& istr, string& str, const string &sep_str)
{
	string str_in{ "" }; // ��������� ������

	// ���������� ����
	getline(istr, str_in); // TODO: ���������� ��������
	while (str_in != sep_str)
	{
		str += str_in + '\n';
		getline(istr, str_in);
	}

	// �������� ���������� ������� �������� �������
	str.erase(str.end() - 1);
}

int main()
{
	// ��������� ������� - 1251
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	// �������� ��������� ����� Plot.txt
	ofstream outf("Plot.txt");

	// ���������� �����
	cout << "~��������� �����:\n";
	bool response{ 1 };
	for (int i = 1; response; i++)
	{
		// ���� ��������
		cout << "\n~������� �������� " << i << " (end - ����� �����):\n";
		string situation{ "" };
		getphrase(cin, situation, "end");

		// ���� �������� 
		cout << "~������� ��������, ���������� � �������� " << i << " (end - ����� �����):\n";
		string action{ "" };
		getphrase(cin, action, "end");

		// ���������� ������ ������� � �����
		outf << "#" << i << "#\n" // ��� �������
			<< situation // ������ � ���������
			<< "\n*\n" // ����������� ������ � ��������� � ������ � ���������
			<< action << "\n" // ������ � ���������
			<< flush; // ������� ������

		// ����������� ����������
		cout << "~������� �� �� ����������?(1-��/0-���): ";
		(cin >> response).ignore();
	}

	// ���� ���� ������� TODO: ���������
	cout << "\n~������� ��� ������� (������ x1,x2,...,xn):";
	response = 1;
	string prufer{ "" }; // ������ � �����
	for (int i = 1; response; i++)
	{
		// ���� �����
		int x{ -1 };
		cout << "~������� ����� x" << i << " : ";
		while (!(cin >> x) || cin.peek() != '\n' || x <= 0) // �������� �� ���� ����� (������ ���� ������� ����������� �����)
		{
			cin.clear();
			cin.ignore();
			cout << "~������: ����������� ������� �����. ���������� ���: ";
		}
		cin.ignore();

		// ���������� ������
		prufer += to_string(x) + ",";

		// ����������� ����������
		cout << "~������� �� �� ����������?(1-��/0-���): ";
		(cin >> response).ignore();
	}
	prufer.erase(prufer.end() - 1); // �������� ��������� �������

	// ������ ���� ������� � ����
	outf << "\n#code#\n"
		<< prufer + "\n" 
		<< flush; // ������� ������

	// �������� ����� ���������
	cout << endl;
	system("Pause");
}