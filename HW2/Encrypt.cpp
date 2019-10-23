#include<iostream>
#include<string>

using namespace std;

string convert_to_bit_pattern(string original);
string permutation(string original, char c);
string oneRound(string original, string roundKey);
string f_Function(string right, string roundKey);

string keyShrink(string oriKey);
string keyTransform(string shrinkKey, int rotateNum);
string keyPermute(string subkey);

string convert_to_hex(string ciphertext);

int main()
{
	string plaintext_hex, key_hex, plaintext, key, ciphertext, subkey, permutedKey;
	cin >> key_hex >> plaintext_hex;
	plaintext = convert_to_bit_pattern(plaintext_hex);
	key = convert_to_bit_pattern(key_hex);

	plaintext = permutation(plaintext, 'i');//initial permutation
	subkey = keyShrink(key);//64bits->56bits
	for (int i = 1; i <= 16; i++)
	{
		switch (i)
		{
		case 1: case 2: case 9: case 16:
			subkey = keyTransform(subkey, 1);//rotate left 1 position
			break;
		default:
			subkey = keyTransform(subkey, 2);//rotate left 2 position
		}
		permutedKey = keyPermute(subkey);//key re-permutation(shrink)
		plaintext = oneRound(plaintext, permutedKey);//f-function, XOR
	}
	ciphertext += plaintext.substr(32, 32);//swap
	ciphertext += plaintext.substr(0, 32);//swap
	ciphertext = permutation(ciphertext, 'f');//final permutation
	cout << convert_to_hex(ciphertext) << endl;
	system("pause");
}

string convert_to_bit_pattern(string original)
{
	string arr[16] = { "0000","0001","0010","0011","0100","0101","0110",
	"0111","1000","1001","1010","1011","1100","1101","1110","1111" };
	char lower[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	char upper[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	string answer = "";
	for (int i = 2; i < 18; i++)//skip 0x
	{
		for (int j = 0; j < 16; j++)
		{
			if (original[i]==lower[j]||original[i]==upper[j])
			{
				answer += arr[j];
				break;
			}
		}
	}
	return answer;
}

string permutation(string original, char c)
{
	string answer;
	answer.resize(64);
	int IPtable[64] = {58,50,42,34,26,18,10,2,
					   60,52,44,36,28,20,12,4,
					   62,54,46,38,30,22,14,6,
					   64,56,48,40,32,24,16,8,
					   57,49,41,33,25,17, 9,1,
					   59,51,43,35,27,19,11,3,
					   61,53,45,37,29,21,13,5,
					   63,55,47,39,31,23,15,7};
	int IPinverseTable[64] = {40, 8,48,16,56,24,64,32,
							  39, 7,47,15,55,23,63,31,
							  38, 6,46,14,54,22,62,30,
							  37, 5,45,13,53,21,61,29,
							  36, 4,44,12,52,20,60,28,
							  35, 3,43,11,51,19,59,27,
							  34, 2,42,10,50,18,58,26,
							  33, 1,41, 9,49,17,57,25};
	if (c == 'i')//initial permutation
	{
		for (int i = 0; i < 64; i++)
		{
			answer[i] = original[IPtable[i] - 1];
		}
	}
	else//final permutation
	{
		for (int i = 0; i < 64; i++)
		{
			answer[i] = original[IPinverseTable[i] - 1];
		}
	}
	return answer;
}

string oneRound(string original, string roundKey)
{
	string left, right, temp, answer;
	left.resize(32);
	right.resize(32);
	temp.resize(32);
	for (int i = 0; i < 32; i++)
	{
		left[i] = original[i];
		right[i] = original[i + 32];
	}
	answer += right;//original.rightthalf = new.lefthalf
	temp = f_Function(right, roundKey);
	for (int i = 0; i < 32; i++)//new.righthalf = original.lefthalf XOR f-func(original.righthalf, roundkey)
	{
		answer += temp[i] ^ left[i];//XOR
	}
	return answer;
}

string f_Function(string right, string roundKey)
{
	string temp, answer, substitution = "";
	temp.resize(48);
	answer.resize(32);

	int expansionTable[48] = {32, 1, 2, 3, 4, 5,
							   4, 5, 6, 7, 8, 9,
							   8, 9,10,11,12,13,
							  12,13,14,15,16,17,
							  16,17,18,19,20,21,
							  20,21,22,23,24,25,
							  24,25,26,27,28,29,
							  28,29,30,31,32, 1};
	int finalPermutation[32] = {16, 7,20,21,29,12,28,17,
								 1,15,23,26, 5,18,31,10,
								 2, 8,24,14,32,27, 3, 9,
								19,13,30, 6,22,11, 4,25};
	int Sbox[8][64] = {
{ 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
  0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
  4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
  15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},
{ 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
  3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
  0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
  13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},
{ 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
  13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
  13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
  1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},
{ 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
  13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
  10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
  3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},
{ 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
  14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
  4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
  11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},
{ 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
  10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
  9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
  4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},
{ 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
  13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
  1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
  6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},
{ 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
  1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
  7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
  2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11} };
	for (int i = 0; i < 48; i++)
	{
		temp[i] = right[expansionTable[i] - 1];
	}
	for (int i = 0; i < 48; i++)
	{
		temp[i] = temp[i] ^ roundKey[i];
	}
	int position = 0, output;
	for (int i = 0; i < 8; i++)//every round: transform 6-bits to 4-bits(with substitution)
	{
		if (temp[i * 6] == 1) position += 32;
		if (temp[i * 6 + 5] == 1)position += 16;
		if (temp[i * 6 + 1] == 1) position += 8;
		if (temp[i * 6 + 2] == 1)position += 4;
		if (temp[i * 6 + 3] == 1)position += 2;
		if (temp[i * 6 + 4] == 1)position += 1;
		output = Sbox[i][position]; position = 0;
		string fourBits; fourBits.resize(4);
		for (int i = 0; i < 4; i++)
		{
			fourBits[3 - i] = output % 2;
			output /= 2;
		}
		substitution += fourBits;
	}

	for (int i = 0; i < 32; i++)
	{
		answer[i] = substitution[finalPermutation[i] - 1];
	}
	return answer;
}

string keyShrink(string oriKey)
{
	string answer;
	answer.resize(56);
	//8,16,24,etc... bits will be remove from oriKey
	int table[56] = {57,49,41,33,25,17,9,
					 1,58,50,42,34,26,18,
					 10,2,59,51,43,35,27,
					 19,11,3,60,52,44,36,
					 63,55,47,39,31,23,15,
					 7,62,54,46,38,30,22,
					 14,6,61,53,45,37,29,
					 21,13,5,28,20,12,4 };
	for (int i = 0; i < 56; i++)
	{
		answer[i] = oriKey[table[i] - 1];
	}
	return answer;
}

string keyTransform(string shrinkKey, int rotateNum)
{
	string left, right, answer;//each half rotates left 1 or 2 bits, depend on rotateNum
	if (rotateNum == 1)
	{
		left = shrinkKey.substr(1,27);
		left += shrinkKey[0];
		right = shrinkKey.substr(29, 27);
		right += shrinkKey[28];
	}
	else
	{
		left = shrinkKey.substr(2, 26);
		left += shrinkKey.substr(0, 2);
		right = shrinkKey.substr(30, 26);
		right += shrinkKey.substr(28, 2);
	}
	answer = left + right;
	return answer;
}

string keyPermute(string subkey)
{
	string answer;
	answer.resize(48);
	//56-bits to 48 bits
	int table[48] = {14,17,11,24, 1, 5,
					  3,28,15, 6,21,10,
					 23,19,12, 4,26, 8,
					 16, 7,27,20,13, 2,
					 41,52,31,37,47,55,
					 30,40,51,45,33,48,
					 44,49,39,56,34,53,
					 46,42,50,36,29,32};
	for (int i = 0; i < 48; i++)
	{
		answer[i] = subkey[table[i] - 1];
	}
	return answer;
}

string convert_to_hex(string ciphertext)
{
	string arr[16] = { "0000","0001","0010","0011","0100","0101","0110",
	"0111","1000","1001","1010","1011","1100","1101","1110","1111" };
	char upper[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	string answer = "";
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (ciphertext.substr(i * 4, 4) == arr[j])
			{
				answer += upper[j];
				break;
			}
		}
	}
	answer = "0x" + answer;
	return answer;
}