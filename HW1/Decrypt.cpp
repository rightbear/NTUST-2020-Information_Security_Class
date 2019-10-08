#include<iostream>
#include<string>

using namespace std;

int main() {
	string CipherMode;
	cin >> CipherMode;

	//���ıK�X
	if (CipherMode == "caesar") {
		string Plaintext, Ciphertext;
		int Key;
		cin >> Key >> Ciphertext;
		Plaintext = Ciphertext;

		//�N�C�@�r���첾�S�w�Z���A�Φ�����
		for (int i = 0; i < Ciphertext.length(); i++) {
			Plaintext[i] -= Key;
			if (65 <= Plaintext[i] && Plaintext[i] <= 90)
				Plaintext[i] += 32;
		}
		cout << Plaintext;
	}

	//�i�p�O�K�X
	else if (CipherMode == "playfair") {
		string Plaintext, Ciphertext, Key;
		cin >> Key >> Ciphertext;
		Plaintext = Ciphertext;
		string NewKey = "";

		//�N����_����J��������I
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

		//�N����_�X�i��25�Ӧr���c�����s���_
		if (NewKey.length() < 25) {
			int additionalLength = 5 * 5 - NewKey.length();
			//�T�{�n�B�~��J���r���A�P�K�_���|�X�{���r��������
			int temp = 65;
			for (int i = 0; i < additionalLength; i++) {
				for (int j = temp; j <= 90; j++) {
					//�J��J�N���L�A�]�����_������X�{J
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

		//�ͦ��񺡪��_�����
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

		//�N�K��̷Ӫ��_���Ѫ����ǧ󴫦r���A�Φ�����
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

	//�@���ʥ[�K
	else if (CipherMode == "vernam") {
		string Plaintext, Ciphertext, Key;
		cin >> Key >> Ciphertext;
		Plaintext = Ciphertext;

		//�N�K�媺�P���_��XOR�A�Φ�����
		if (Key.length() >= Ciphertext.length()) {
			for (int i = 0; i < Ciphertext.length(); i++) {
				Plaintext[i] = (Plaintext[i]-'A') ^ (Key[i]-'A');
			}
		}
		//�Y�K�_�L�u�A�N�`�K�_�����X�R�ܻP�K��P����
		else {
			string Realkey;
			for (int i = 0; i < (Ciphertext.length() / Key.length()); i++) {
				Realkey = Realkey + Key;
			}
			for (int i = 0; i < (Ciphertext.length() % Key.length()); i++) {
				Realkey = Realkey + Key[i];
			}
			for (int i = 0; i < Ciphertext.length(); i++) {
				Plaintext[i] = (Plaintext[i]-'A') ^ (Realkey[i]-'A');
			}
		}

		for (int i = 0; i < Plaintext.length(); i++) {
			if (0 <= Plaintext[i] && Plaintext[i] <= 26)
				Plaintext[i] += 'a';
		}

		cout << Plaintext;
	}

	//�������K�X
	else if (CipherMode == "row") {
		string Plaintext, Ciphertext, Key;
		int row, column;
		cin >> Key >> Ciphertext;
		Plaintext = Ciphertext;
		column = Key.length();

		int * absentCol = new int[Key.length()];

		if (Ciphertext.length() % Key.length()) {
			row = (Ciphertext.length() / Key.length()) + 1;

			//�����񺡩��媺���A���X�椤�|�񤣺�
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

		//�ͦ��񺡩��媺���
		char** table = new char*[column];
		for (int i = 0; i < column; i++) {
			table[i] = new char[row];
		}

		//�N����̷Ӫ��_���Ѫ����ǭ��s�ƦC�A�Φ��K��A�ä����񤣺��M��o����ر���
		if (Ciphertext.length() % Key.length()) {
			int counter = 0;
			for (int i = 0; i < Key.length(); i++) {
				for (int j = 0; j < column; j++) {
					int judge = 0;

					//�p�G�K��L�k�񺡪�檺�Y�@��A�h�N�Ӧ檺�ůʶ�W'\n'
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
		
		//�N��椺���K��̶����ഫ������
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

	//�X���K�X
	else if (CipherMode == "rail_fence") {
		string Plaintext, Ciphertext;
		int Key;
		int row, column;
		cin >> Key >> Ciphertext;
		Plaintext = Ciphertext;
		column = Ciphertext.length();
		row = Key;

		//�ͦ��񺡩��媺���
		char** table = new char*[column];
		for (int i = 0; i < column; i++) {
			table[i] = new char[row];
		}

		for (int i = 0; i < row; i++) {
			for (int j = 0; j < column; j++) {
				table[i][j] = '\n';
			}
		}
		//���N�̫�n�Ѫ����嶶�ǥH'*'�аO�A�åH�i������J���
		int tempRow = 0, rowAdder = -1;
		int group = 2 * Key - 2;

		for (int i = 0; i < column; i++) {
			table[tempRow][i] = '*';

			if (tempRow % group == 0 || tempRow % group == (Key - 1))
				rowAdder *= -1;
	
			tempRow = tempRow + rowAdder;
		}
		//�N�K��̷ӱK�_�A�Ѥp�ܤj��J��檺�C���A�è��N�����������аO
		int counter = 0;
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < Ciphertext.length(); j++) {
				if (table[i][j] == '*') {
					table[i][j] = Ciphertext[counter];
					counter++;
				}
			}
		}
		//�N����H�i������J���A�̴`�ٹL�{����
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