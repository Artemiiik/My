/*=======================================
Name: QuestGame.cpp
Author: Yunin Artem (Artemiiik)
Date: 08.05.2021
Version: 
Copiright: freeware
Description:
A simple console game based 
on multiple-choice questions
=======================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>
using namespace std;

/// <summary>
/// 
/// </summary>
class GameData
{
private:
	struct Node; // ������� ������ ������� ��������-��������
	Node* root{ nullptr }; // ������ ������
public:
	// ����������� �� ���������
	GameData() : root{ nullptr }
	{ }
	// ����������� �����������
	GameData(const GameData&)
	{ } // TODO ��������
	//~GameData();

	GameData& operator=(const GameData&);

	void readFile(const char* const);
};

/// <summary>
/// ������� ������ ������� ��������-��������
/// </summary>
struct GameData::Node
{
	string action{ "" }; // ������ � ���������
	string situation{ "" }; // ������ � ���������
	vector <Node*> child; // ������ ��������

	Node() : action(""), situation("")
	{ }
	Node(const string& act, const string& sit) : action(act), situation(sit)
	{ }
	// TODO: ��������� ������������ ��� � �������� =?
};


/// <summary>
/// 
/// </summary>
/// <param name="s"></param>
void GameData::readFile(const char* const filePath)
{
	ifstream inf{ filePath };
	if (!inf.is_open()) throw 1; // �������� �� ������������� �����
	
	// ������ �����
	using PowNodePair = pair <int, Node*>; // ���� ������� ������� - ��������� �� �������
	vector <PowNodePair> vertices; // ������ ������ � �� ���������
	const char CODE_SYMBOL = '#'; // ������� ������-�����������
	string istr; // ������ �� �����
	while (getline(inf, istr))
	{
		// ��������� 2-� ������� �������� (1-����� �������� � ���������, 2-����� ��������� � ���������)
		unsigned int firstCSPos{ istr.find(CODE_SYMBOL) };
		if (firstCSPos == string::npos) throw 2;
		unsigned int secondCSPos{ istr.find(CODE_SYMBOL, firstCSPos + 1) };
		if (secondCSPos == string::npos) throw 2;
		unsigned int excessCSPos{ istr.find(CODE_SYMBOL, secondCSPos + 1) };
		if (excessCSPos != string::npos) throw 2;

		// �������������� ������� ������� � int
		string powStr{ istr.substr(0, firstCSPos) }; 
		int vertPow{};
		try
		{
			vertPow = stoi(powStr);
		} 
		catch (invalid_argument) 
		{
			throw 2;
		}

		// ������ �������� � �������� � string
		string action{ istr.substr(firstCSPos + 1, secondCSPos - firstCSPos - 1) };
		string situation{ istr.substr(secondCSPos + 1, istr.length() - secondCSPos - 1) };
		
		// ������ ������ � ������
		vertices.push_back(PowNodePair{ vertPow, new Node{action, situation} });
	}
	inf.close(); // �������� �����

	// ���������� ������ (������������ ������)
	root = vertices[0].second;
	vector<PowNodePair>::const_iterator itPrnt{ vertices.cbegin() };
	vector<PowNodePair>::const_iterator itChild{ vertices.cbegin() + 1 };
	while(itPrnt != vertices.cend())
	{
		int prntPow = (*itPrnt).first; // ������� ������������ �������
		Node* prnt = (*itPrnt).second; // ��������� �� ���. �������
		// ������������ ����� ����� ������������ �������� � prntPow ��������� ���������
		for (int i{ 0 }; i < prntPow; ++i, ++itChild)
		{
			(itChild == vertices.cend()) ? throw 2 : 0; // ���� ������� ������� ������� �������, 
														// ����� ������� ����� ���� ��������� ������ - �������� ���������� �����
			Node* child = (*itChild).second; 
			prnt->child.push_back(child);
		}
		++itPrnt;
	}
}

class GamePlay
{

};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	GameData data;
	try
	{
		data.readFile("Plot.txt");
	}
	catch (int k)
	{
		cerr << "ERROR: code " << k << "!\n";
	}
	system("pause");
}