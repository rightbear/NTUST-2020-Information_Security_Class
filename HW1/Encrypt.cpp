#include<iostream>
#include<string>

using namespace std;

int main() {
	string CipherMode;
	cin >> CipherMode;

	//凱薩密碼
	//(假設前提:位移結果不超過ASCII範圍)
	if (CipherMode == "caesar") {
		string Plaintext, Ciphertext;
		int Key;
		cin >> Key >> Plaintext;
		Ciphertext = Plaintext;

		//將每一字母位移特定距離，形成密文
		for (int i = 0; i < Plaintext.length(); i++) {
			Ciphertext[i] += Key;
			if (97 <= Ciphertext[i] && Ciphertext[i] <= 122)
				Ciphertext[i] -= 32;
		}

		cout << Ciphertext;
	}

	//波雷費密碼
	//(假設前提:明文不會有連續的重複字母、明文只有偶數個字母、明文長度不超過25、明文最多只會有25種字母且都會在密鑰中、密鑰表格中的j要視為i)
	else if (CipherMode == "playfair") {
		string Plaintext, Ciphertext, Key;
		cin >> Key >> Plaintext;
		Ciphertext = Plaintext;
		string NewKey = "";

		//將原金鑰中的j都先換成i、小寫
		for (int i = 0; i < Key.length(); i++) {
			if (Key[i] >= 65 && Key[i] <= 90) {
				Key[i] += 32;
			}
			if (Key[i] == 'j')
				Key[i] = 'i';
		}

		//原明文中的j都先換成i
		for (int i = 0; i < Ciphertext.length(); i++) {
			if (Ciphertext[i] == 'j')
				Ciphertext[i] = 'i';
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
			int temp = 97;
			for (int i = 0; i < additionalLength; i++) {
				for (int j = temp; j <= 122; j++) {
					//遇到j就跳過，因為金鑰中不能出現j
					if (char(j) == 'j') {
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
		char table[5][5];
		
		int counter = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				table[i][j] = NewKey[counter];
				counter++;
			}
		}

		//將明文依照金鑰提供的順序更換字母，形成密文
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
				col1 = (col1 + 1) % 5;
				col2 = (col2 + 1) % 5;

			}
			else if (col1 == col2) {
				row1 = (row1 + 1) % 5;
				row2 = (row2 + 1) % 5;
			}
			else {
				int tempCol = col2;
				col2 = col1;
				col1 = tempCol;
			}

			Ciphertext[i] = table[row1][col1];
			Ciphertext[i + 1] = table[row2][col2];

			if (97 <= Ciphertext[i] && Ciphertext[i] <= 122)
				Ciphertext[i] -= 32;
			if (97 <= Ciphertext[i + 1] && Ciphertext[i + 1] <= 122)
				Ciphertext[i + 1] -= 32;
		}

		cout << Ciphertext;
	}

	//一次性加密
	else if (CipherMode == "vernam") {
		string Plaintext, Ciphertext, Key;
		cin >> Key >> Plaintext;
		Ciphertext = Plaintext;

		//將明文的與金鑰做XOR，形成密文
		if (Key.length() >= Plaintext.length()) {
			for (int i = 0; i < Plaintext.length(); i++) {
				Ciphertext[i] = (Ciphertext[i]-'a') ^ (Key[i]-'A');
			}
		}
		//若密鑰過短，將總密鑰長度擴充至與明文同長度
		else {
			string Realkey;
			for (int i = 0; i < (Plaintext.length() / Key.length()); i++) {
				Realkey = Realkey + Key;
			}
			for (int i = 0; i < (Plaintext.length() % Key.length()); i++) {
				Realkey = Realkey + Key[i];
			}
			for (int i = 0; i < Plaintext.length(); i++) {
				Ciphertext[i] = (Ciphertext[i] -'a') ^ (Realkey[i]-'A');
			}
		}

		for (int i = 0; i < Ciphertext.length(); i++) {
			if (0 <= Ciphertext[i] && Ciphertext[i] <= 26)
				Ciphertext[i] += 'A';
		}

		cout << Ciphertext;
	}

	//替換式密碼
	else if (CipherMode == "row") {
		string Plaintext, Ciphertext, Key;
		int row, column;
		cin >> Key >> Plaintext;
		Ciphertext = Plaintext;
		column = Key.length();

		//如果明文無法填滿以金鑰長度作為行數的表格，則填入'\0'
		if (Plaintext.length() % Key.length()) {
			row = (Plaintext.length() / Key.length()) + 1;

			int additionalLength = Key.length() - (Plaintext.length() % Key.length());
			for (int i = 0; i < additionalLength; i++) {
				Plaintext = Plaintext + '\n';
			}
		}
		else {
			row = Plaintext.length() / Key.length();
		}
		
		//生成填滿明文的表格
		char** table = new char*[column];
		for (int i = 0; i < column; i++) {
			table[i] = new char[row];
		}

		for (int i = 0; i < row; i++) {
			for (int j = 0; j < column; j++) {
				table[i][j] = Plaintext[i*column + j];
			}
		}

		//將明文依照金鑰提供的順序重新排列，形成密文
		int counter = 0;
		for (int i = 1; i <= column; i++) {
			for (int j = 0; j < column; j++) {
				if (Key[j] != (i + 48))
					continue;
				else {
					for (int k = 0; k < row; k++) {

						if (table[k][j] != '\n') {
							Ciphertext[counter] = table[k][j];
							if (97 <= Ciphertext[counter] && Ciphertext[counter] <= 122) {
								Ciphertext[counter] -= 32;
							}
							counter++;
						}
					}
					break;
				}
			}
		}

		cout << Ciphertext;
	}

	//籬笆密碼
	else if (CipherMode == "rail_fence") {
		string Plaintext, Ciphertext;
		int Key;
		int row, column;
		cin >> Key >> Plaintext;
		Ciphertext = Plaintext;
		column = Plaintext.length();
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

		//將明文以波浪狀填入表格，依循還過程分組
		int tempRow = 0, rowAdder = -1;
		int group = 2 * Key - 2;

		for (int i = 0; i < column; i++) {
			table[tempRow][i] = Plaintext[i];

			if (tempRow % group == 0 || tempRow % group == (Key - 1))
				rowAdder *= -1;

			tempRow = tempRow + rowAdder;
		}

		//依照密鑰，以遞增的形式填出特定列中的表格字元，形成密文
		int counter = 0;
		for (int i = 0; i < Key; i++) {
			for (int j = 0; j < column; j++) {
				if (table[i][j] != '\n') {
					Ciphertext[counter] = table[i][j];

					if (97 <= Ciphertext[counter] && Ciphertext[counter] <= 122)
						Ciphertext[counter] -= 32;

					counter++;
				}
			}
		}

		cout << Ciphertext;
	}
}