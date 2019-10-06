#include<iostream>
#include<string>

using namespace std;

int main() {
	string CipherMode;
	cin >> CipherMode;

	//���ıK�X
	//(���]�e��:�첾���G���W�LASCII�d��)
	if (CipherMode == "caesar") {
		string Plaintext, Ciphertext;
		int Key;
		cin >> Key >> Plaintext;
		Ciphertext = Plaintext;

		//�N�C�@�r���첾�S�w�Z���A�Φ��K��
		for (int i = 0; i < Plaintext.length(); i++) {
			Ciphertext[i] += Key;
			if (97 <= Ciphertext[i] && Ciphertext[i] <= 122)
				Ciphertext[i] -= 32;
		}

		cout << Ciphertext;
	}

	//�i�p�O�K�X
	//(���]�e��:���夣�|���s�򪺭��Ʀr���B����u�����ƭӦr���B������פ��W�L25�B����̦h�u�|��25�ئr���B���|�b�K�_���B�K�_��椤��j�n����i)
	else if (CipherMode == "playfair") {
		string Plaintext, Ciphertext, Key;
		cin >> Key >> Plaintext;
		Ciphertext = Plaintext;
		string NewKey = "";

		//�N����_����j��������i�B�p�g
		for (int i = 0; i < Key.length(); i++) {
			if (Key[i] >= 65 && Key[i] <= 90) {
				Key[i] += 32;
			}
			if (Key[i] == 'j')
				Key[i] = 'i';
		}

		//����夤��j��������i
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

		//�N����_�X�i��25�Ӧr���c�����s���_
		if (NewKey.length() < 25) {
			int additionalLength = 5 * 5 - NewKey.length();
			//�T�{�n�B�~��J���r���A�P�K�_���|�X�{���r��������
			int temp = 97;
			for (int i = 0; i < additionalLength; i++) {
				for (int j = temp; j <= 122; j++) {
					//�J��j�N���L�A�]�����_������X�{j
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

		//�N����̷Ӫ��_���Ѫ����ǧ󴫦r���A�Φ��K��
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

	//�@���ʥ[�K
	else if (CipherMode == "vernam") {
		string Plaintext, Ciphertext, Key;
		cin >> Key >> Plaintext;
		Ciphertext = Plaintext;

		//�N���媺�P���_��XOR�A�Φ��K��
		if (Key.length() >= Plaintext.length()) {
			for (int i = 0; i < Plaintext.length(); i++) {
				Ciphertext[i] = Ciphertext[i] ^ Key[i];
			}
		}
		//�Y�K�_�L�u�A�N�`�K�_�����X�R�ܻP����P����
		else {
			string Realkey;
			for (int i = 0; i < (Plaintext.length() / Key.length()); i++) {
				Realkey = Realkey + Key;
			}
			for (int i = 0; i < (Plaintext.length() % Key.length()); i++) {
				Realkey = Realkey + Key[i];
			}
			for (int i = 0; i < Plaintext.length(); i++) {
				Ciphertext[i] = Ciphertext[i] ^ Realkey[i];
			}
		}

		for (int i = 0; i < Ciphertext.length(); i++) {
			if (97 <= Ciphertext[i] && Ciphertext[i] <= 122)
				Ciphertext[i] -= 32;
		}

		cout << Ciphertext;
	}

	//�������K�X
	else if (CipherMode == "row") {
		string Plaintext, Ciphertext, Key;
		int row, column;
		cin >> Key >> Plaintext;
		Ciphertext = Plaintext;
		column = Key.length();

		//�p�G����L�k�񺡥H���_���ק@����ƪ����A�h��J'\0'
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
		
		//�ͦ��񺡩��媺���
		char** table = new char*[column];
		for (int i = 0; i < column; i++) {
			table[i] = new char[row];
		}

		for (int i = 0; i < row; i++) {
			for (int j = 0; j < column; j++) {
				table[i][j] = Plaintext[i*column + j];
			}
		}

		//�N����̷Ӫ��_���Ѫ����ǭ��s�ƦC�A�Φ��K��
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

	//�X���K�X
	else if (CipherMode == "rail_fence") {
		string Plaintext, Ciphertext;
		int Key;
		int row, column;
		cin >> Key >> Plaintext;
		Ciphertext = Plaintext;
		column = Plaintext.length();
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

		//�N����H�i������J���A�̴`�ٹL�{����
		int tempRow = 0, rowAdder = -1;
		int group = 2 * Key - 2;

		for (int i = 0; i < column; i++) {
			table[tempRow][i] = Plaintext[i];

			if (tempRow % group == 0 || tempRow % group == (Key - 1))
				rowAdder *= -1;

			tempRow = tempRow + rowAdder;
		}

		//�̷ӱK�_�A�H���W���Φ���X�S�w�C�������r���A�Φ��K��
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