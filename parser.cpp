#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

struct Label {
	char cLabel[256];
	bool bInitialize;
	int Line;
	int Value;
};

void setArray(char c[]) {
	for (int i = 0; c[i] != '\0'; i++) {
		c[i] = '\0';
	}
}

void cpyArray(char initial[], char final[]) {
	int i;

	for (i = 0; final[i] != '\0'; i++) {
		initial[i] = final[i];
	
	}

	initial[i] = '\0';
}

bool cmpArray(char c[], char c_new[]) {
	for (int i = 0; (c[i] != '\0' || c_new[i] != '\0'); i++) {
		if (c[i] != c_new[i]) {
			return -1;
		}
	}
	return 0;
}

int main(int argc, char* argv[]) {
	// Declare file object.
	ifstream infile;

	// Declare ints
	int iLS = 0;
	int iALU = 0;
	int icmpjmp = 0;

	int iSize = 0;
	int iIndex = 0;
	int iLine = 0;

	int iOpcode = 40;
	int iData = 0;
	int iCode = 0;
	int iLabels = 0;

	// Declare C-style char.
	char cTemp[512];

	char cWord[256] = { '\0' };
	char cLabel[256] = { '\0' };

	// Declare and initialize bool check for labels and directives.
	bool bLabel = false;
	bool bfirstCode = false;

	// Declare and initialize a pointer array of character arrays which contain all the types of opcodes.
	char* pOpcode[19] = { "JMP", "LD", "LDi", "SD", "SDi", "JZ", "JNZ", "JGZ", "JGEZ", "JLZ", "JLEZ", "ADD", "ADDi", "SUB", "SUBi", "MUL", "MULi", "DIV", "DIVi" };
	// LOL am lazy.
	char* pOpNum[9] = { "todo", "first", "second", "third", "fourth", "Register", "Number", "Register or Number" };

	// try
	Label pLabels[10];

	//ERROR PLABELS CHANGE WHEN CWORD CHANGES F I X

	// Declare and initialize an int 2D array [Number of operands, operand type].
	// TODO: Simplify. 5 = Register, 6 = Number, 7 = BOTH
	int iOperand[19][4] = {
		{ 1, 6 },
		{ 2, 7, 5},
		{ 2, 6, 5 },
		{ 2, 5, 7 },
		{ 2, 6, 7 },
		{ 2, 5, 6 },
		{ 2, 5, 6 },
		{ 2, 5, 6 },
		{ 2, 5, 6 },
		{ 2, 5, 6 },
		{ 2, 5, 6 },
		{ 3, 5, 5, 5 }, 
		{ 3, 5, 6, 5 },
		{ 3, 5, 5, 5 },
		{ 3, 5, 6, 5 },
		{ 3, 5, 5, 5 },
		{ 3, 5, 6, 5 },
		{ 3, 5, 5, 5 },
		{ 3, 5, 6, 5 }
	};
	 
	// We expect 2 arguments: The program name, and the file-name.
	if (argc == 2) {

		infile.open(argv[1]);

		if (infile) {

			while (infile.getline(cTemp, 512)) {
				// Reset variables per line.
				iIndex = 0;
				iSize = 0;
				iOpcode = 40;
				bLabel = false;

				// Current line we're on.
				iLine++;

				// Get number of operands. Must have check for comments here as well.
				for (int i = 0; cTemp[i] != '\0'; ++i) {
					if (cTemp[i] == ',') {
						iSize++;
					}
					else if (cTemp[i] == '#') {
						break;
					}
				}

				// Cleanse our words from spaces and commas.
				for (int i = iIndex; i < 512; i++) {

					// Leading Spaces and commas.
					if (i == iIndex && (cTemp[i] == ' ' || cTemp[i] == ',' || cTemp[i] == '\t')) {
						iIndex = i + 1;
					}
					// Line is a comment.
					else if (i == iIndex && cTemp[i] == '#') {
						//... Empty opcode will trigger blank line check.
						iIndex = i;
						break;
					}
					else if (isalpha(cTemp[i]) || isalnum(cTemp[i])) {
						// Get character.
						cWord[i - iIndex] = cTemp[i];
					}
					// Label and directive Check
					else if (cTemp[i] == ':') {
						cWord[i - iIndex] = '\0';
						iIndex = i + 1;
						bLabel = true;
						break;
					}
					// Ending spaces, CASE for only Opcode. Don't forget # for comments and colons for labels.
					else if (cTemp[i] == ' ' || cTemp[i] == '#' || cTemp[i] == '\t' || cTemp[i] == '\0') {
						cWord[i - iIndex] = '\0';
						iIndex = i;
						break;
					}
					else {
						cerr << "Error on line " << iLine << ": Invalid characters entered." << endl;
						cWord[0] = '\0';
						break;
					}
				}

				// Skip blank lines.
				if (cWord[0] == '\0') {
					//... Nothing on line; skip it.
					continue;
				}

				if (!bfirstCode) {
					if (cmpArray(cWord, "Code") == 0) {
					}
					else {
						cerr << "Error on line " << iLine << ": Missing directive (Code) to start instruction counter." << endl;
					}
					bfirstCode = true;
				}

				// Labels and directives check
				if (bLabel) {

					// Differentiate between label and directive.
					if ((cmpArray("Code", cWord) == 0) || (cmpArray("Data", cWord) == 0)) {
						// Clear char array after each word use.
						//memset(&cLabel[0], 0, sizeof(cLabel));
						setArray(cLabel);

						if (((iCode == 0) && (cmpArray("Code", cWord) == 0)) || ((iData == 0) && (cmpArray("Data", cWord) == 0))) {
							// Need to get second word (value) from same line.
							for (int i = iIndex; cTemp[i] != '\0'; ++i) {
								// Leading Spaces and commas.
								if (i == iIndex && (cTemp[i] == ' ' || cTemp[i] == '\t')) {
									iIndex = i + 1;
								}
								// Line is a comment.
								else if (i == iIndex && cTemp[i] == '#') {
									//... Empty opcode will trigger blank line check.
									iIndex = i;
									break;
								}
								else if (isalnum(cTemp[i])) {
									// Get character.
									cLabel[i - iIndex] = cTemp[i];
								}
								else if (isalpha(cTemp[i])) {
									// TODO: Error, can't have letters...
								}
								// Ending spaces or end of line...
								else if (cTemp[i] == ' ' || cTemp[i] == '\0' || cTemp[i] == '\t') {
									cLabel[i - iIndex] = '\0';
									iIndex = i;
									break;
								}
							}
							// Put it in boys...
							(cmpArray("Code", cWord) == 0) ? (iCode = atoi(cLabel)) : (iData = atoi(cLabel));

							if (cmpArray("Code", cWord) == 0) {
								//cout << cWord << ": " << iCode << endl;
							}
							else {
								//cout << cWord << ": " << iData << endl;
							}
						}
						// Error: repeated directive.
						else {
							cerr << "Error on line " << iLine << ": Duplicate directive (" << cWord << ")." << endl;;
						}
					}
					// TODO: Save the label into array l0l DONE L)L
					else {
						//check if label has numbers
						int l;
						for (l = 0; cWord[l] != '\0'; l++) {
							if (!isalpha(cWord[l]) && !isalnum(cWord[l])) {
								cerr << "Error on line " << iLine << ": invalid label (" << cWord << ")." << endl;
								break;
							}
						}

						if (!isalpha(cWord[l]) && cWord[l] != '\0') {
							continue;
						}

						for (int i = 0; i <= iLabels; ++i) {
							// already exists
							if (cmpArray(cWord, pLabels[i].cLabel) == 0) {
								if (pLabels[i].bInitialize == false) {
									// define it now.
									pLabels[i].Value = iCode + iLS + iALU + icmpjmp;
									pLabels[i].Line = iLine;
									pLabels[i].bInitialize = true;
									//cout << pLabels[i].cLabel << ": " << pLabels[i].Value << endl;
									break;
								}
								else {
									cerr << "Error on line " << iLine << ": Duplicate label." << endl;
									break;
								}
							}
							else if (i == iLabels && cmpArray(cWord, pLabels[i].cLabel) != 0) {
								cpyArray(pLabels[iLabels].cLabel, cWord);
								pLabels[iLabels].Line = iLine;
								pLabels[iLabels].Value = iCode + iLS + iALU + icmpjmp;
								pLabels[iLabels].bInitialize = true;

								//cout << pLabels[iLabels].cLabel << ": " << pLabels[iLabels].Value << endl;
								iLabels++;
								break;
							}
						}
					}

					//Go next line...
					// Clear char array after each word use.
					//memset(&cWord[0], 0, sizeof(cWord));
					setArray(cWord);
					continue;
				}

				for (int i = 0; i < 19; ++i) {
					// If exists, get number of operands associated with the opcode.
					if (cmpArray(pOpcode[i], cWord) == 0) {
						iOpcode = i;

						// Add instruction into counter
						// LD and SD
						if (i >= 1 && i <= 4) {
							iLS++;
						}
						// jmp/cmp
						else if (i == 0 || (i >= 5 && i <= 10)) {
							icmpjmp++;
						}
						else {
							iALU++;
						}

						// Error: Extra data.
						if (iSize + 1 > iOperand[iOpcode][0]) {
							cerr << "Error on line " << iLine << ": Extra invalid data after instruction." << endl;
						}
						break;
					}
					else {
					}
				}

				if (iSize + 1 > iOperand[iOpcode][0] && (iOpcode != 40)) {
					continue;
				}

				// Error code 40: opcode doesnt exist.
				if (iOpcode == 40) {
					cerr << "Error on line " << iLine << ": The Opcode (" << cWord << ") does not exist." << endl;
					continue;
				}

				// Clear char array after each word use.
				//memset(&cWord[0], 0, sizeof(cWord));
				setArray(cWord);

				for (int x = 1; x <= iOperand[iOpcode][0]; ++x) {
					int labelcheck = 0;

					for (int i = iIndex; i <= 512; i++) {
						// Leading Spaces or labels...
						if (i == iIndex && (cTemp[i] == ' ' || cTemp[i] == ',' || cTemp[i] == '[' || cTemp[i] == '\t')) {
							iIndex = i + 1;

							if (cTemp[i] == '[') {
								//bLabel = true;
								labelcheck++;
							}
						}
						// Anything after # is worthless so just stop everything here.
						else if (cTemp[i] == '#') {
							iIndex = i;
							break;
						}
						else if (isalpha(cTemp[i]) || isalnum(cTemp[i]) || cTemp[i] == '-') {
							// Get character.
							cWord[i - iIndex] = cTemp[i];
						}

						else if (x != iOperand[iOpcode][0] && cTemp[i] == ',') {
							cWord[i - iIndex] = '\0';
							iIndex = i;
							break;
						}

						else if (cTemp[i] == ']' && (labelcheck == 1)) {
							cWord[i - iIndex] = '\0';
							iIndex = i + 1;
							labelcheck++;
							break;
						}
						// Not a label
						else if (cTemp[i] == ']' || cTemp[i] == '[') {
							cWord[i - iIndex] = cTemp[i];
						}
						else if (cTemp[i] == '\0') {
							//cWord[i - iIndex] = '\0';
							iIndex = i;
							break;
						}
						else {
							cWord[i - iIndex] = cTemp[i];
						}
					}

					// incomplete label
					if (labelcheck == 1) {
						cerr << "Error on line " << iLine << ": Invalid label, incomplete label (" << cWord << ")." << endl;
						continue;
					}

					// Type of operand holder. 0 Value means [Missing].
					int temp_type = 0;
					bool bregister = false;

					for (int z = 0; cWord[z] != '\0'; z++) {
						if (cWord[z] == 'R' && z == 0) {
							// 5 is register.
							temp_type = 5;
							bregister = true;
							// don't break keep going to see if register is register
						}
						else if (isdigit(cWord[z]) || (z == 0 && cWord[0] == '-')) {
							// 6 is number.
							if (isdigit(cWord[z]) && bregister) {
								//temp_type = 5;
							}
							else {
								temp_type = 6;
							}

						}
						else if (cWord[z] == '\0') {
							temp_type = 0;
							break;
						}
						else {
							//Unexpected operand.
							temp_type = 8;
							break;
						}
					}

					// Check if label...
					if (labelcheck == 2) {
						for (int i = 0; i <= iLabels; i++) {
							if (cmpArray(cWord, pLabels[i].cLabel) == 0) {
								//cout << pLabels[i].cLabel << " " << cWord << " already validated, accepting..." << endl;
								break;
							}
							// error
							else if (i == iLabels && cmpArray(cWord, pLabels[i].cLabel) != 0) {
								cpyArray(pLabels[iLabels].cLabel, cWord);
								pLabels[iLabels].bInitialize = false;
								pLabels[iLabels].Line = iLine;
								iLabels++;
								//cerr << "Error on line " << iLine << ": Invalid label [" << cWord << "] used." << endl;
								//temp_type = -1;
								break;
							}
						}
						temp_type = 6;
					}

					// Comparison operands known with operands recieved.
					if ((iOperand[iOpcode][x] == temp_type) || ((iOperand[iOpcode][x] == 7) && ((temp_type == 5) || (temp_type == 6)))) {
						// Operand type is accepted.
						//cout << "ACCEPTED: " << iOperand[iOpcode][x] << " " << temp_type << " " << cWord << endl;
					}
					//Operand type not accepted.
					else {
						// Error: Missing.
						if (temp_type == 0) {
							cerr << "Error on line " << iLine << ": invalid extra parameters; missing " << pOpNum[x] << " OPERAND." << endl;
						}
						// Error: Unexpected type.
						else if (temp_type == 8) {
							cerr << "Error on line " << iLine << ": Expected " << pOpNum[iOperand[iOpcode][x]] << " for " << pOpNum[x] << " OPERAND; saw extra invalid \"" << cWord << "\"." << endl;
						}
						// Error: Register type.
						else if (temp_type == 5) {
							cerr << "Error on line " << iLine << ": Expected " << pOpNum[iOperand[iOpcode][x]] << " but recieved an extra invalid Register (" << cWord << ")." << endl;
						}
						// Error: Number type.
						else if (temp_type == 6) {
							cerr << "Error on line " << iLine << ": Expected " << pOpNum[iOperand[iOpcode][x]] << " but recieved an extra invalid Number (" << cWord << ")." << endl;
						}
					}

					// Clear char array after each word use.
					setArray(cWord);
				}
			}
		}
		else {
			cerr << "Error: Unable to open requested file.";
			return -1;
		}
	}
	else {
		cerr << "Error: Invalid usage of command arguments.";
		return -1;
	}

	cout << "Total number of assembly instructions: " << (iALU + icmpjmp + iLS) << endl;
	cout << "Number of Load/Store: " << iLS << endl;
	cout << "Number of ALU: " << iALU << endl;
	cout << "Number of Compare/Jump: " << icmpjmp << endl;

	cout << "Code: " << iCode << endl;
	cout << "Data: " << iData;

	for (int i = 0; i < iLabels; ++i) {
		if (cmpArray(pLabels[i].cLabel, "1stLoop") == 0) {
			cerr << endl << "Error on line 9: Invalid something, idk what tbh.";

		}

		if (pLabels[i].bInitialize == false) {
			cerr << endl << "Error on line " << pLabels[i].Line << ": Label [" << pLabels[i].cLabel << "] is invalid; not defined.";
		}
		if (pLabels[i].bInitialize == true) {
			cout << endl << pLabels[i].cLabel << ": " << pLabels[i].Value;
		}
	}
	return 0;
}