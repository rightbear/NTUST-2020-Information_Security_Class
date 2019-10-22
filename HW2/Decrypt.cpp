#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

string hex_str_to_bin_str(const string&);
string hex_char_to_bin_str(char);
string bin_str_to_hex_str(const string&, const int&);
string bin_str_to_hex_char(const string&);
string Initial_Permuntation(const string&);
string Final_Permuntation(const string&);

string Rounds(const string&, const string&);
string f_Function(const string&, const string&);
string int_to_string(int*, int);
string Expansion(const string&);
string S_Box(const string&);
string f_Permuntation(const string&);

string Shifts(const string&);
string PC_1(const string&);
string PC_2(const string&);
string rotateLeft(const string&, int);
string rotateRight(const string&, int);

int main() {
	string hex_Key;
	cin >> hex_Key;
	string bin_Key = hex_str_to_bin_str(hex_Key);
	string bin_pc1key = PC_1(bin_Key);

	string hex_Ciphertext;
	cin >> hex_Ciphertext;
	string bin_Ciphertext = hex_str_to_bin_str(hex_Ciphertext);
	string bin_InitPermutext = Initial_Permuntation(bin_Ciphertext);

	string Init_text = bin_InitPermutext;
	string Init_shift_key = bin_pc1key;				//第1個round不做Shift
	string Init_round_key = PC_2(Init_shift_key);

	string Round_text, Round_Key, Shift_Key;
	Round_text = Rounds(Init_text, Init_round_key);

	for (int i = 2; i <= 16; i++) {
		Shift_Key = Shifts(Init_shift_key);
		Round_Key = PC_2(Shift_Key);

		Init_text = Round_text;
		Init_round_key = Round_Key;
		Init_shift_key = Shift_Key;

		Round_text = Rounds(Init_text, Init_round_key);
	}

	string bin_FinalPermutext = Final_Permuntation(Round_text);
	string hex_PlainText = bin_str_to_hex_str(bin_FinalPermutext, 16);

	cout << hex_PlainText;
}

//***********************與plaintext有關***********************

//將16進位的測資字串，轉成2進位的測資字串
string hex_str_to_bin_str(const string& hex_str)
{
	string bin;
	for (unsigned i = 2; i < hex_str.length(); ++i)
		bin += hex_char_to_bin_str(hex_str[i]);
	return bin;
}

//將1位元的16進位表示法，轉成4位元的2進位表示法
string hex_char_to_bin_str(char hex_char) {
	switch (hex_char)
	{
		case '0': return "0000";
		case '1': return "0001";
		case '2': return "0010";
		case '3': return "0011";
		case '4': return "0100";
		case '5': return "0101";
		case '6': return "0110";
		case '7': return "0111";
		case '8': return "1000";
		case '9': return "1001";
		case 'a': return "1010";
		case 'b': return "1011";
		case 'c': return "1100";
		case 'd': return "1101";
		case 'e': return "1110";
		case 'f': return "1111";
		case 'A': return "1010";
		case 'B': return "1011";
		case 'C': return "1100";
		case 'D': return "1101";
		case 'E': return "1110";
		case 'F': return "1111";
		default: return "";
	}
}

//將2進位的測資字串，轉成16進位的測資字串
string bin_str_to_hex_str(const string& bin_str, const int& length)
{
	string hex = "0x";
	string temp_str;

	for (unsigned i = 0; i < length; ++i) {
		temp_str = bin_str.substr(i * 4, 4);
		hex += bin_str_to_hex_char(temp_str);
	}
	return hex;
}

//將4位元的2進位表示法，轉成1位元的16進位表示法
string bin_str_to_hex_char(const string& temp_str) {
	if (temp_str == "0000")
		return "0";
	else if (temp_str == "0001")
		return "1";
	else if (temp_str == "0010")
		return "2";
	else if (temp_str == "0011")
		return "3";
	else if (temp_str == "0100")
		return "4";
	else if (temp_str == "0101")
		return "5";
	else if (temp_str == "0110")
		return "6";
	else if (temp_str == "0111")
		return "7";
	else if (temp_str == "1000")
		return "8";
	else if (temp_str == "1001")
		return "9";
	else if (temp_str == "1010")
		return "a";
	else if (temp_str == "1011")
		return "b";
	else if (temp_str == "1100")
		return "c";
	else if (temp_str == "1101")
		return "d";
	else if (temp_str == "1110")
		return "e";
	else if (temp_str == "1111")
		return "f";
}

//做DES中第一次的Permuntation
string Initial_Permuntation(const string& bin_ciphertext) {
	string bin_pertext = bin_ciphertext;

	int IP[8][8] = { 58, 50, 42, 34, 26, 18, 10, 2,
					 60, 52, 44, 36, 28, 20, 12, 4,
					 62, 54, 46, 38, 30, 22, 14, 6,
					 64, 56, 48, 40, 32, 24, 16, 8,
					 57, 49, 41, 33, 25, 17,  9, 1,
					 59, 51, 43, 35, 27, 19, 11, 3,
					 61, 53, 45, 37, 29, 21, 13, 5,
					 63, 55, 47, 39, 31, 23, 15, 7 };

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			bin_pertext[i * 8 + j] = bin_ciphertext[(IP[i][j] - 1)];
		}
	}

	return bin_pertext;
}

//做DES中最後一次的Permuntation
string Final_Permuntation(const string& bin_ciphertext)
{
	string new_ciphertext_left = bin_ciphertext.substr(32, 32);
	string new_ciphertext_right = bin_ciphertext.substr(0, 32);

	string new_ciphertext = new_ciphertext_left + new_ciphertext_right;

	string bin_pertext = new_ciphertext;

	int IP_1[8][8] = { 40,  8, 48, 16, 56, 24, 64, 32,
					   39,  7, 47, 15, 55, 23, 63, 31,
					   38,  6, 46, 14, 54, 22, 62, 30,
					   37,  5, 45, 13, 53, 21, 61, 29,
					   36,  4, 44, 12, 52, 20, 60, 28,
					   35,  3, 43, 11, 51, 19, 59, 27,
					   34,  2, 42, 10, 50, 18, 58, 26,
					   33,  1, 41,  9, 49, 17, 57, 25 };

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			bin_pertext[i * 8 + j] = new_ciphertext[(IP_1[i][j] - 1)];
		}
	}

	return bin_pertext;
}

//做DES中的16個Rounds - a
string Rounds(const string& bin_oldtext, const string& round_key) {
	string old_left_str = bin_oldtext.substr(0, 32);	//原字串左半
	string old_right_str = bin_oldtext.substr(32, 32);	//原字串右半

	string temp_left_str = f_Function(old_right_str, round_key);

	int *new_right_int = new int[32]();
	for (int i = 0; i < 32; i++) {
		new_right_int[i] = (temp_left_str[i] - '0') ^ (old_left_str[i] - '0');
	}

	string new_right_str = int_to_string(new_right_int, 32);
	string new_left_str = old_right_str;

	string bin_newtext = new_left_str + new_right_str;
	return bin_newtext;
}

//做DES中Rounds的f function
string f_Function(const string& old_right_str, const string& bin_pc2key) {
	string expansion_str = Expansion(old_right_str);

	int *XOR_int = new int[48]();
	for (int i = 0; i < 48; i++) {
		XOR_int[i] = (expansion_str[i] - '0') ^ (bin_pc2key[i] - '0');
	}
	string XOR_str = int_to_string(XOR_int, 48);

	string S_box_str = S_Box(XOR_str);
	string f_pertext = f_Permuntation(S_box_str);

	return f_pertext;
}

//將整數陣列換成整數形式的字串
string int_to_string(int* int_bit, int length) {
	string str_bit;

	for (int i = 0; i < length; i++) {
		str_bit += (int_bit[i] + '0');
	}

	return str_bit;
}

//做Rounds中f function的Expansion
string Expansion(const string& old_right_str) {
	string expansion_str;
	expansion_str.resize(48);

	int Expansion[8][6] = { 32,  1,  2,  3,  4,  5,
							 4,  5,  6,  7,  8,  9,
							 8,  9, 10, 11, 12, 13,
							12, 13, 14, 15, 16, 17,
							16, 17, 18, 19, 20, 21,
							20, 21, 22, 23, 24, 25,
							24, 25, 26, 27, 28, 29,
							28, 29, 30, 31, 32,  1 };

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 6; j++) {
			expansion_str[i * 6 + j] = old_right_str[(Expansion[i][j] - 1)];
		}
	}

	return expansion_str;
}

//做Rounds中f function的S Box
string S_Box(const string& S_box_str_48bits) {
	int S_8_boxes[8][4][16] =
	{ { 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
		 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,		//S1
		 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
		15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13  },


	  { 15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
		 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,		//S2
		 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
		13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9  },

	  { 10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
		13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,		//S3
		13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
		 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12  },

	  {  7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
		13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,		//S4
		10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
		 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14  },

	  {  2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
		14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,		//S5
		 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
		11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3  },

	  {  12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
		 10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,	//S6
		  9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
		  4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13 },

	  {   4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
		 13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,	//S7
		  1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
		  6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12  },

	  {	 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
		  1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,	//S8
		  7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
		  2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11  } };

	string  S_BOX_str_partial;
	int decimal_row, decimal_column;
	string S_box_str_32bits;

	int S_BOX_decimal_num[8];
	int S_BOX_decimal_partial;
	int division, divisor, quotient, remainder;

	//將6bit的字串變換成4bit的整數
	for (int round = 0; round < 8; round++) {
		S_BOX_str_partial = S_box_str_48bits.substr((round * 6), 6);
		decimal_row = (S_BOX_str_partial[0] - 48) * pow(2, 1) + (S_BOX_str_partial[5] - 48);
		decimal_column = (S_BOX_str_partial[1] - 48) * pow(2, 3) + (S_BOX_str_partial[2] - 48) * pow(2, 2) + (S_BOX_str_partial[3] - 48) * pow(2, 1) + (S_BOX_str_partial[4] - 48);
		S_BOX_decimal_num[round] = S_8_boxes[round][decimal_row][decimal_column];
	}

	for (int round = 0; round < 8; round++) {
		division = S_BOX_decimal_num[round];
		for (int index = 3; index >= 0; index--) {
			divisor = pow(2, index);
			quotient = division / divisor;
			remainder = division % divisor;

			S_box_str_32bits += (quotient + '0');
			division = remainder;
		}
	}

	return S_box_str_32bits;
}

//做Rounds中f function的Permutation
string f_Permuntation(const string& S_box_str_32bits) {
	string f_pertext = S_box_str_32bits;

	int SP[4][8] = { 16,  7, 20, 21, 29, 12, 28, 17,
					  1, 15, 23, 26,  5, 18, 31, 10,
					  2,  8, 24, 14, 32, 27,  3,  9,
					 19, 13, 30,  6, 22, 11,  4, 25 };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			f_pertext[i * 8 + j] = S_box_str_32bits[(SP[i][j] - 1)];
		}
	}

	return f_pertext;
}

//***********************與key有關***********************

//做Key Schedule中Transform的Rotates
string Shifts(const string& bin_key) {
	string shift_key;

	static int shift_counter = 2;

	string left_str = bin_key.substr(0, 28);	//新金鑰左半
	string right_str = bin_key.substr(28, 28);	//新金鑰右半
	string new_left_str, new_right_str, new_str;

	if (shift_counter == 2 || shift_counter == 9 || shift_counter == 16) {
		new_left_str = rotateRight(left_str, 1);
		new_right_str = rotateRight(right_str, 1);
	}
	else {
		new_left_str = rotateRight(left_str, 2);
		new_right_str = rotateRight(right_str, 2);
	}

	shift_key = new_left_str + new_right_str;

	shift_counter++;
	return shift_key;
}

//做Key Schedule中的PC-1
string PC_1(const string& bin_key) {
	string bin_pc1key;

	for (int i = 0; i < 8; i++) {
		int length = 7;
		bin_pc1key += bin_key.substr(i * 8, length);
	}

	int PCminus1[8][7] = { 57, 49, 41, 33, 25, 17,  9,
							1, 58, 50, 42, 34, 26, 18,
						   10,  2, 59, 51, 43, 35, 27,
						   19, 11,  3, 60, 52, 44, 36,
						   63, 55, 47, 39, 31, 23, 15,
							7, 62, 54, 46, 38, 30, 22,
						   14,  6, 61, 53, 45, 37, 29,
						   21, 13,  5, 28, 20, 12,  4 };

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 7; j++) {
			bin_pc1key[i * 7 + j] = bin_key[(PCminus1[i][j] - 1)];
		}
	}

	return bin_pc1key;
}

//做Key Schedule中Transform的PC-2
string PC_2(const string& shift_key) {
	string bin_pc2key;

	bin_pc2key = shift_key.substr(0, 48);

	int PCminus1[8][6] = { 14, 17, 11, 24,  1,  5,
							3, 28, 15,  6, 21, 10,
						   23, 19, 12,  4, 26,  8,
						   16,  7, 27, 20, 13,  2,
						   41, 52, 31, 37, 47, 55,
						   30, 40, 51, 45, 33, 48,
						   44, 49, 39, 56, 34, 53,
						   46, 42, 50, 36, 29, 32 };

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 6; j++) {
			bin_pc2key[i * 6 + j] = shift_key[(PCminus1[i][j] - 1)];
		}
	}

	return bin_pc2key;
}

//做Transform中的左移
string rotateLeft(const string& left_str, int shift) {	//循環左移n位
	string new_left_str = left_str.substr(shift, left_str.length() - shift) + left_str.substr(0, shift);
	return new_left_str;
}

//做Transform中的右移
string rotateRight(const string& right_str, int shift) {	//循環右移n位
	string new_right_str = right_str.substr(right_str.length() - shift, shift) + right_str.substr(0, right_str.length() - shift);
	return new_right_str;
}