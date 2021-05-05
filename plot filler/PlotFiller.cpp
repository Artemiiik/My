/*
=======================================
Name: PlotFiller.cpp
Author: Yunin Artem (Artemiiik)
Date: 05.05.2021
Version: 1.1
Copiright: freeware
Description:
This program is designed to fill in 
a plot file in the form of a plot tree
=======================================
*/

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h> 
using namespace std;

int main()
{
	// ��������� ������� - 1251
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	// �������� ��������� ����� Plot.txt
	ofstream outf("Plot.txt");

	// ���������� �����
	cout << "~����� ���� ��������� ��� ���������� ��������� �����.\n"
		<< "����� ������������ ����� ������������ ������ � ���������,\n"
		<< "��������������� � ���� ���� �����: �������� � ��������,\n"
		<< "� ������� ����� ��������. ���������� ����������� ������\n"
		<< "���������� � �������� ������� (�������� ����� �� �������)\n"
		<< "� ���� ����������� (������ ����, ����� �������).\n"
		<< "����������� �������: # � * (������ ������������)\n";
	int verticesCount{ 1 }; // ���������� ������ � ���������� (��������� �� ����� �������� ������)
	for(int i = 1; i <= verticesCount; i++)
	{
		// ���� ������� �������
		cout << "\n~������� ������� ������� " << i << ": ";
		int verticePow{}; // ������� �������
		while (!(cin >> verticePow) || cin.peek() != '\n' || verticePow < 0) // �������� ����� ����� �� ������������
		{
			cin.clear(); // ������� ������ �� ������
			while (cin.get() != '\n'); // �������� ������� �������� ������� �� ������
			cout << "~������� ������� ��������. ���������� ���: ";
		}
		cin.ignore(); // �������� ������� �������� ������� �� ������
		verticesCount += verticePow; // ������� ����� ���������� ������ � ����������

		// ���� �������� 
		cout << "~������� ��������, ���������� � ��������:\n";
		string action{};
		getline(cin, action);
		while (action.find('#') != string::npos || action.find('*') != string::npos)
		{
			cout << "~������ �������� ����������� �������. ���������� ���:\n";
			getline(cin, action);
		}

		// ���� ��������
		cout << "~������� ��������:\n";
		string situation{};
		getline(cin, situation);
		while (situation.find('#') != string::npos || situation.find('*') != string::npos)
		{
			cout << "~������ �������� ����������� �������. ���������� ���:\n";
			getline(cin, situation);
		}

		// ���������� ������ ������� � �����
		outf << '#' << verticePow << '#' // ������� �������
			<< action // ������ � ���������
			<< '*' // ����������� ������ � ��������� � ������ � ���������
			<< situation << '\n' // ������ � ���������
			<< flush; // ������� ������
	}
	outf.close(); // ��������� ����

	// �������� ����� ���������
	cout << endl;
	system("Pause");
}