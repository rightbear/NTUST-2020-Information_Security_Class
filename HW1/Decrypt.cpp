#include<iostream>
#include<string>

using namespace std;

int main() {
	string CipherMode;
	cin >> CipherMode;

	//凱薩密碼
	if (CipherMode == "caesar") {
		string Plaintext, Ciphertext;
		int Key;
		cin >> Key >> Ciphertext;
		Plaintext = Ciphertext;

		//將每一字母位移特定距離，形成明文
		for (int i = 0; i < Ciphertext.length(); i++) {
			Plaintext[i] -= Key;
			if (65 <= Plaintext[i] && Plaintext[i] <= 90)
				Plaintext[i] += 32;
		}
		cout << Plaintext;
	}

	//波雷費密碼
	else if (CipherMode == "playfair") {
		string Plaintext, Ciphertext, Key;
		cin >> Key >> Ciphertext;
		Plaintext = Ciphertext;
		string NewKey = "";

		//將原金鑰中的J都先換成I
		for (int i = 0; i < Key.length(); i++) {
			if (Key[i] == 'J')
				Key[i] = 'I';
		}

		for (int i = 0; i < Key.length(); i++) {
			int judge = 0;
			for (int j = 0; j < NewKey.length(); j++) {
				if (NewKey[j] == (Key[i] + 32)) {
					judge = 1;
					break;
				}
			}
			if (judge == 0) {
				NewKey = NewKey + Key[i];
			}
		}

		//將原金鑰擴展成25個字母構成的新金鑰
		if (NewKey.length() < 25) {
			int additionalLength = 5 * 5 - NewKey.length();
			//確認要額外填入的字母，與密鑰中會出現的字母不重複
			int temp = 65;
			for (int i = 0; i < additionalLength; i++) {
				for (int j = temp; j <= 90; j++) {
					//遇到J就跳過，因為金鑰中不能出現J
					if (char(j) == 'J') {
						continue;
					}
					else {
						int judge = 0;
						for (int k = 0; k < NewKey.length(); k++) {
							if (char(j) == NewKey[k]) {
								judge = 1;
								break;
							}
						}
						if (judge == 0) {
							NewKey = NewKey + char(j);
							temp = j + 1;
							break;
						}
					}
				}
			}
		}

		//生成填滿金鑰的表格
		char** table = new char*[5];
		for (int i = 0; i < 5; i++) {
			table[i] = new char[5];
		}
		int counter = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				table[i][j] = NewKey[counter];
				counter++;
			}
		}

		//將密文依照金鑰提供的順序更換字母，形成明文
		for (int i = 0; i < Ciphertext.length(); i = i + 2) {
			int row1, col1, row2, col2;
			int judge1 = 0, judge2 = 0;
			for (int row = 0; row < 5; row++) {
				for (int col = 0; col < 5; col++) {
					if (table[row][col] == Ciphertext[i]) {
						row1 = row;
						col1 = col;
						judge1 = 1;
						break;
					}
				}
				if (judge1 == 1)
					break;
			}
			for (int row = 0; row < 5; row++) {
				for (int col = 0; col < 5; col++) {
					if (table[row][col] == Ciphertext[i + 1]) {
						row2 = row;
						col2 = col;
						judge2 = 1;
						break;
					}
				}
				if (judge2 == 1)
					break;
			}

			if (row1 == row2) {
				col1 = col1 - 1;
				if (col1 < 0) {
					col1 = col1 + 5;
				}
				col2 = col2 - 1;
				if (col2 < 0) {
					col2 = col2 + 5;
				}

			}
			else if (col1 == col2) {
				row1 = row1 - 1;
				if (row1 < 0) {
					row1 = row1 + 5;
				}
				row2 = row2 - 1;
				if (row2 < 0) {
					row2 = row2 + 5;
				}
			}
			else {
				int tempCol = col2;
				col2 = col1;
				col1 = tempCol;
			}

			Plaintext[i] = table[row1][col1];
			Plaintext[i + 1] = table[row2][col2];

			if (65 <= Plaintext[i] && Plaintext[i] <= 90)
				Plaintext[i] += 32;
			if (65 <= Plaintext[i + 1] && Plaintext[i + 1] <= 90)
				Plaintext[i + 1] += 32;
		}

		cout << Plaintext;
	}

	//一次性加密
	else if (CipherMode == "vernam") {
		string Plaintext, Ciphertext, Key;
		cin >> Key >> Ciphertext;
		Plaintext = Ciphertext;

		//將密文的與金鑰做XOR，形成明文
		if (Key.length() >= Ciphertext.length()) {
			for (int i = 0; i < Ciphertext.length(); i++) {
				Plaintext[i] = Plaintext[i] ^ Key[i];
			}
		}
		//若密鑰過短，將總密鑰長度擴充至與密文同長度
		else {
			string Realkey;
			for (int i = 0; i < (Ciphertext.length() / Key.length()); i++) {
				Realkey = Realkey + Key;
			}
			for (int i = 0; i < (Ciphertext.length() % Key.length()); i++) {
				Realkey = Realkey + Key[i];
			}
			for (int i = 0; i < Ciphertext.length(); i++) {
				Plaintext[i] = Plaintext[i] ^ Realkey[i];
			}
		}

		for (int i = 0; i < Plaintext.length(); i++) {
			if (65 <= Plaintext[i] && Plaintext[i] <= 90)
				Plaintext[i] += 32;
		}

		cout << Plaintext;
	}

	//替換式密碼
	else if (CipherMode == "row") {
		string Plaintext, Ciphertext, Key;
		int row, column;
		cin >> Key >> Ciphertext;
		Plaintext = Ciphertext;
		column = Key.length();

		int * absentCol = new int[Key.length()];

		if (Ciphertext.length() % Key.length()) {
			row = (Ciphertext.length() / Key.length()) + 1;

			//紀錄填滿明文的中，哪幾行中會填不滿
			for (int i = 0; i < Key.length(); i++) {
				if (i < (Ciphertext.length() % Key.length()))
					absentCol[i] = 0;
				else
					absentCol[i] = 1;
			}
		}
		else {
			row = Ciphertext.length() / Key.length();
		}

		//生成填滿明文的表格
		char** table = new char*[column];
		for (int i = 0; i < column; i++) {
			table[i] = new char[row];
		}

		//將明文依照金鑰提供的順序重新排列，形成密文，並分成填不滿和填得滿兩種情形
		if (Ciphertext.length() % Key.length()) {
			int counter = 0;
			for (int i = 0; i < Key.length(); i++) {
				for (int j = 0; j < column; j++) {
					int judge = 0;

					//如果密文無法填滿表格的某一行，則將該行的空缺填上'\n'
					if ((Key[j] - 48) == (i+1)) {
						if (absentCol[i] == 1) {
							for (int k = 0; k < row -1 ; k++) {
								table[k][j] = Ciphertext[counter];
								counter++;
							}
							table[j][row-1] = '\n';
						}
						else {
							for (int k = 0; k < row ; k++) {
								table[k][j] = Ciphertext[i*row + k];
								counter++;
							}
						}
						judge = 1;
					}
					if (judge == 1)
						break;
				}
			}
		}
		else {	
			for (int i = 0 ; i < Key.length(); i++) {
				for (int j = 0; j < column ; j++) {
					int judge = 0;
					if ((Key[j] - 48) == (i+1)) {
						for (int k = 0; k < row; k++) {
							table[k][j] = Ciphertext[i*row + k];
						}
						judge = 1;
					}
					if (judge == 1)
						break;
				}
			}
		}
		
		//將表格內的密文依順序轉換成明文
		int counter = 0;
		for (int i = 0; i < row; i++) {
			int judge = 0;
			for (int j = 0; j < column; j++) {
				Plaintext[counter] = table[i][j];

				if (65 <= Plaintext[counter] && Plaintext[counter] <= 90)
					Plaintext[i] += 32;

				counter++;
				if (counter == Ciphertext.length()) {
					judge = 1;
					break;
				}
			}
			if (judge == 1)
				break;
		}

		cout << Plaintext;
	}

	//籬笆密碼
	else if (CipherMode == "rail_fence") {
		string Plaintext, Ciphertext;
		int Key;
		int row, column;
		cin >> Key >> Ciphertext;
		Plaintext = Ciphertext;
		column = Ciphertext.length();
		row = Key;

		//生成填滿明文的表格
		char** table = new char*[column];
		for (int i = 0; i < column; i++) {
			table[i] = new char[row];
		}

		for (int i = 0; i < row; i++) {
			for (int j = 0; j < column; j++) {
				table[i][j] = '\n';
			}
		}
		//先將最後要解的明文順序以'*'標記，並以波浪狀填入表格
		int tempRow = 0, rowAdder = -1;
		int group = 2 * Key - 2;

		for (int i = 0; i < column; i++) {
			table[tempRow][i] = '*';

			if (tempRow % group == 0 || tempRow % group == (Key - 1))
				rowAdder *= -1;
	
			tempRow = tempRow + rowAdder;
		}
		//將密文依照密鑰，由小至大填入表格的列中，並取代掉原先的明文標記
		int counter = 0;
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < Ciphertext.length(); j++) {
				if (table[i][j] == '*') {
					table[i][j] = Ciphertext[counter];
					counter++;
				}
			}
		}
		//將明文以波浪狀填入表格，依循還過程分組
		tempRow = 0, rowAdder = -1;

		for (int i = 0; i < column; i++) {
			Plaintext[i] = table[tempRow][i];

			if (65 <= Plaintext[i] && Plaintext[i] <= 90)
				Plaintext[i] += 32;

			if (tempRow % group == 0 || tempRow % group == (Key - 1))
				rowAdder *= -1;
			
			tempRow = tempRow + rowAdder;
		}

		cout << Plaintext;
	}
}