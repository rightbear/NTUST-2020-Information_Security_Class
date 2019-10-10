#include<iostream>
#include<stdlib.h>
#include<string>
#include<cmath>
#include<vector>

using namespace std;
void encryptCaesar(int key, string plaintext);
void encryptPlayfair(string key, string plaintext);
void encryptAutokey(string word, string plaintext);
void encryptRowtransposition(string key, string plaintext);
void encryptRailFence(int key, string plaintext);

int main() {
	string CipherMode , plaintext;
	cin >> CipherMode;

	//���ıK�X
	//(���]�e��:�첾���G���W�LASCII�d��)
	if (CipherMode == "caesar")
	{
		int key;
		cin >> key >> plaintext;
		encryptCaesar(key, plaintext);
	}

	//�i�p�O�K�X
	//(���]�e��:���夣�|���s�򪺭��Ʀr���B����u�����ƭӦr���B������פ��W�L25�B����̦h�u�|��25�ئr���B���|�b�K�_���B�K�_��椤��j�n����i)
	else if (CipherMode == "playfair")
	{
		string key;
		cin >> key >> plaintext;
		encryptPlayfair(key, plaintext);
	}

	//�@���ʥ[�K
	else if (CipherMode == "vernam")
	{
		string key;
		cin >> key >> plaintext;
		encryptAutokey(key, plaintext);
	}

	//�������K�X
	else if (CipherMode == "row")
	{
		string key;
		cin >> key >> plaintext;
		encryptRowtransposition(key, plaintext);
	}

	//�X���K�X
	else if (CipherMode == "rail_fence")
	{
		int key;
		cin >> key >> plaintext;
		encryptRailFence(key, plaintext);
	}
	system("pause");
}
void encryptCaesar(int key, string plaintext)
{
	string ciphertext = "";
	for (int i = 0; i < plaintext.size(); i++)
	{
		ciphertext += toupper('A' + (plaintext[i] - 'a' + key) % 26);
	}
	cout << ciphertext;
}

void encryptPlayfair(string key, string plaintext)
{
	char matrix[5][5];
	int count = 0;
	string temp = "";
	//�N����_����J��������I
	for (int i = 0; i < key.size(); i++) {

		if (key[i] == 'J')
			key[i] = 'I';
	}
	//����夤��j��������i
	for (int i = 0; i < plaintext.size(); i++) {
		if (plaintext[i] == 'j')
			plaintext[i] = 'i';
	}
	for (int i = 0; i < key.size(); i++)
	{
		if (temp.find(key[i]) == -1)//�קK����
		{
			matrix[count / 5][count % 5] = key[i];
			temp += key[i];
			count++;
		}
	}
	for (char c = 'A'; c <= 'Z'; c++)
	{
		if (c == 'J') continue;
		if (key.find(c) == -1)
		{
			matrix[count / 5][count % 5] = c;
			count++;
		}
	}
	int row1, row2, col1, col2;
	for (int i = 0; i < plaintext.size(); i++)
	{
		for (int c = 0; c < 25; c++)
		{
			if (matrix[c / 5][c % 5] == toupper(plaintext[i]))
			{
				row1 = c / 5;
				col1 = c % 5;
			}
		}
		i++;
		for (int c = 0; c < 25; c++)
		{
			if (matrix[c / 5][c % 5] == toupper(plaintext[i]))
			{
				row2 = c / 5;
				col2 = c % 5;
			}
		}
		if (row1 == row2)
		{
			col1++, col2++;
			if (col1 == 5)
			{
				col1 = 0;
			}
			if (col2 == 5)
			{
				col2 = 0;
			}
			cout << matrix[row1][col1] << matrix[row2][col2];
		}
		else if (col1 == col2)
		{
			row1++, row2++;
			if (row1 == 5)
			{
				row1 = 0;
			}
			if (row2 == 5)
			{
				row2 = 0;
			}
			cout << matrix[row1][col1] << matrix[row2][col2];
		}
		else
		{
			cout << matrix[row1][col2] << matrix[row2][col1];
		}
	}
	cout << endl;
}

void encryptAutokey(string word, string plaintext)
{
	string key = word, ciphertext = "";
	for (int i = 0; i < plaintext.size() - word.size(); i++)
	{
		key += toupper(plaintext[i]);
	}
	for (int i = 0; i < key.size(); i++)
	{
		ciphertext += 'A' + ((key[i] - 'A') ^ (plaintext[i] - 'a'));
	}
	cout << ciphertext << endl;
}

void encryptRowtransposition(string key, string plaintext)
{
	vector<vector<char> > ciphertext;
	int row = ceil((float)plaintext.size() / key.size()), col = key.size();
	vector<char> temp;
	for (int i = 0; i < col; i++)
	{
		temp.push_back(' ');
	}
	for (int i = 0; i < row; i++)
	{
		ciphertext.push_back(temp);
	}

	for (int i = 0; i < plaintext.size(); i++)
	{
		ciphertext[i / col][i % col] = toupper(plaintext[i]);
	}

	for (int i = 1; i <= col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			if (ciphertext[j][key.find(i + '0')] != ' ')
			{
				cout << ciphertext[j][key.find(i + '0')];
			}
		}
	}
}

void encryptRailFence(int key, string plaintext)
{
	char** ciphertext;
	ciphertext = new char*[key];
	for (int i = 0; i < key; i++)
	{
	    int num = ceil(float(plaintext.size()) / key);
		ciphertext[i] = new char[num];
	}
	for (int i = 0; i < plaintext.size(); i++)
	{
		ciphertext[i % key][i / key] = toupper(plaintext[i]);
	}
	for (int i = 0, count = 0; i < key; i++)
	{
		for (int j = 0; j < ceil(float(plaintext.size()) / key); j++, count++)
		{
			if (count != plaintext.size())
			{
				cout << ciphertext[i][j];
			}
			else return;
		}
	}
}
