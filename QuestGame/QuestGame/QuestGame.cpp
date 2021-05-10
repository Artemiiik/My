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
	// TODO ��� ��������� ������������ � operator=

	void readFile(const char* const, const char);

	friend class GamePlay;
};

/// <summary>
/// ������� ������ ������� ��������-��������
/// </summary>
struct GameData::Node
{
	string action{ "" }; // ������ � ���������
	string situation{ "" }; // ������ � ���������
	vector <Node*> child; // ������ ��������

	Node(const string& act, const string& sit) : action(act), situation(sit)
	{ }
};

void GameData::readFile(const char* const filePath, const char codeSym = '#')
{
	ifstream inf{ filePath };
	if (!inf.is_open()) throw 1; // �������� �� ������������� �����
	
	// ������ �����
	using PowNodePair = pair <int, Node*>; // ���� ������� ������� - ��������� �� �������
	vector <PowNodePair> vertices; // ������ ������ � �� ���������
	string istr; // ������ �� �����
	while (getline(inf, istr))
	{
		// ��������� 2-� ������� �������� (1-����� �������� � ���������, 2-����� ��������� � ���������)
		unsigned int firstCSPos{ istr.find(codeSym) };
		if (firstCSPos == string::npos) throw 2;
		unsigned int secondCSPos{ istr.find(codeSym, firstCSPos + 1) };
		if (secondCSPos == string::npos) throw 2;
		unsigned int excessCSPos{ istr.find(codeSym, secondCSPos + 1) };
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
private:
	const GameData& data;
public:
	GamePlay(const GameData& _data) : data(_data)
	{ }
	// TODO ��� ��������� ������������ � operator=

	void play() const;
};

void GamePlay::play() const
{
	GameData::Node* currentNode = data.root;
	while ((currentNode->child).size() != 0)
	{
		cout << "~" + currentNode->situation + "\n\n~�������� ������:\n";

		vector<GameData::Node*>::const_iterator itChild{ (currentNode->child).cbegin() };
		for (int i{ 0 }; itChild != (currentNode->child).cend(); i++)
		{
			cout << i << ") " + (*itChild)->action + '\n';
			itChild++;
		}

		cout << "\n~������� ��������� ������� ������: ";
		int response{};
		while (!(cin >> response) || cin.peek() != '\n' || response < 0 || (unsigned)response >= (currentNode->child).size())
		{
			cin.clear();
			while (cin.get() != '\n');
			cout << "~������ �������� �� ����������. ���������� ��� ���: ";
		}
		currentNode = (currentNode->child)[response];

		cout << '\n';
	}
	cout << "~" + currentNode->situation + "\nGAMEOVER ���� �� ������\n\n";
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	GameData data;
	data.readFile("Plot.txt");
	GamePlay game(data);
	game.play();
	
	system("pause");
}