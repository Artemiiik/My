/*=======================================
Name: QuestGame.cpp
Author: Yunin Artem (Artemiiik)
Date: 09.06.2021
Version: 1.0
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

//================================================================================================

/// <summary>
/// ���� ������� ������������ ����� ��������,
/// ���������� � ��������, � ���� �������� ���������������
/// </summary>
struct Level
{
	string action; // ��������
	string situation; // ��������

	// ����������� � ����� ��������
	Level(const string& _action, const string& _situation) : action(_action), situation(_situation)
	{ }
};

/// <summary>
/// ���� ������� ������������ k-����� ������ � ������� � ���� ������ Level
/// </summary>
struct Node
{
	Level data; // ������
	vector <Node*> child; // �������

	// ����������� � ���������� Level
	explicit Node(const Level& _Level) : data{ _Level }
	{ }
	// ����������� �����������
	Node(const Node& _Node) : data{ _Node.data }
	{
		vector<Node*>::const_iterator it;
		for (it = _Node.child.cbegin(); it != _Node.child.cend(); it++)
			child.push_back(new Node{ **it }); // ������ �������� ����������� ������� �������� ���������� �������
	}
	// �������� ������������
	Node& operator=(const Node& _Node)
	{
		data = _Node.data;
		vector<Node*>::const_iterator it;

		// ������� ������� ������
		for (it = child.cbegin(); it != child.cend(); it++)
			delete* it; // ������ ������� ����������� � ���� ������������ ������, ������� ������� ������
		child.clear(); // ����� ������� ��� ���������

		// ����� ��������� ��� ������� �����
		for (it = _Node.child.cbegin(); it != _Node.child.cend(); it++)
			child.push_back(new Node{ **it });

		// ���������� �������� ���������
		return *this;
	}
	// ����������
	~Node()
	{
		vector<Node*>::const_iterator it;
		for (it = child.cbegin(); it != child.cend(); it++)
			delete* it;
		child.clear();
	}
};

//================================================================================================

/// <summary>
/// �����, ���������� �� ���������� �������� ������ �� �����
/// � ����������� �������� ��
/// </summary>
class GameData
{
private:
	Node* root{ nullptr }; // ������ ������
public:
	// ����������� �� ���������
	GameData() : root{ nullptr }
	{ }
	// ����������� ����������� 
	GameData(const GameData& _GameData) 
	{ 
		(_GameData.root == nullptr) ? root = nullptr : root = new Node{ *(_GameData.root) };
	}
	// �������� ������������ 
	GameData& operator=(const GameData& _GameData)
	{
		delete root;
		(_GameData.root == nullptr) ? root = nullptr : root = new Node{ *(_GameData.root) };
		return *this;
	}
	// ���������� 
	~GameData()
	{
		delete root;
	}
	
	void readFile(const char* const, const char);

	friend class GamePlay;
};

/// <summary>
/// ������ �����
/// </summary>
/// <param name="filePath">���� � �����</param>
/// <param name="codeSym">������� ������</param>
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

		Level lvl{ action, situation };
		
		// ������ ������ � ������
		vertices.push_back(PowNodePair{ vertPow, new Node{lvl} });
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

//================================================================================================

/// <summary>
/// �����, ���������� �� ������ ����
/// </summary>
class GamePlay
{
private:
	GameData data;
public:
	// ����������� c ����� ����������. ����������� �� ��������� �� ������������ ����������
	explicit GamePlay(const GameData& _data) : data(_data)
	{ }
	// ����������� �����������
	GamePlay(const GamePlay& _GamePlay) : data(_GamePlay.data)
	{ }
	// �������� ������������
	GamePlay& operator=(const GamePlay& _GamePlay) 
	{
		data = _GamePlay.data;
	}

	void play() const;
};

/// <summary>
/// ������� ����
/// </summary>
void GamePlay::play() const
{
	Node* currentNode = data.root; // �������� �������� �������
	while (currentNode) // ���� ���� �������
	{
		string situation{ (currentNode->data).situation };

		// ������� ��������
		cout << "~" + situation + '\n';

		// ���� ��� ��������� ������, ����� ����
		if ((currentNode->child).size() == 0)
		{
			cout << "\n~����� ����\n\n";
			break;
		}

		// ���� �������� ������ ����, ������� �� �� �����
		cout << "\n~�������� ������:\n";
		vector<Node*>::const_iterator itChild{ (currentNode->child).cbegin() };
		for (int i{ 0 }; itChild != (currentNode->child).cend(); i++)
		{
			string action{ ((*itChild)->data).action };

			cout << i << ") " + action + '\n';
			itChild++;
		}

		// ���������� ������� ������� ������
		cout << "\n~������� ��������� ������� ������: ";
		int response{};
		while (!(cin >> response) || cin.peek() != '\n' || response < 0 || (unsigned)response >= (currentNode->child).size())
		{
			cin.clear();
			while (cin.get() != '\n');
			cout << "~������ �������� �� ����������. ���������� ��� ���: ";
		}
		
		// ������� �� ����� �������
		currentNode = (currentNode->child)[response];

		// ����� ����� ��������� ���������� ������
		cout << '\n';
	}
}

//================================================================================================

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	// ������ �������� ������
	GameData data; // �������� ������
	try 
	{
		data.readFile("Plot.txt"); // ������� ��������� ����
	}
	catch (int code)
	{
		switch (code)
		{
		case 1:
			cerr << "������: �������� ���� �� ������!\n";
			return 1;
			break;
		case 2:
			cerr << "������: �������� ���� �������� �������!\n";
			return 2;
			break;
		}
	}

	// ���� ����
	GamePlay game(data); // �������������� ���� ����� GameData
	game.play(); // �������� ����
	
	system("pause");
}