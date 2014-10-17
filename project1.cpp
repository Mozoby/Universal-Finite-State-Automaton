/* CS 311 Project 1
 * 
 * Universal Finite State Automaton
 * 		Build a program which can evaluate string across any FSA represented generically.
 *
 * This version can also accept a definition for a non-deterministic FSA and will simulate the computation tree for any string.
 * 
 * Author: Bryan Thornbury
*/

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

typedef pair<string, bool> sb;
typedef vector<int> vi;
typedef vector<vi > vvi;
typedef vector<vvi > vvvi; // [state][symbol] => next state
typedef map<char, int> mci;

#define REP(i,a,b) \
	for(int i = int(a); i<= int(b); i++)

//Method which recurses through the computation tree for dfsa or nfsa
bool validate(vvvi &fsa, mci &alphabet, vector<bool> &final, string &input,  int currentState, int stringPosition){
	//Check if we are in a final state and have processed the whole string
	if(final[currentState] && stringPosition >= input.size()) return true;

	//Check if we are over 
	else if(stringPosition >= input.size()) return false;

	//Get the current symbol's representation
	mci::iterator mit = alphabet.find(input[stringPosition]);

	//check for existence of the symbol in the alphabet
	if(mit == alphabet.end()) return false;

	int symbol = mit->second;

	//If there are no next states, return false, we are in a trap
	if(fsa[currentState][symbol].size() == 0){
	 	return false;
	}

	//For each possible next state, traverse the computation tree to find a valid state
	for(vi::iterator it = fsa[currentState][symbol].begin(); it != fsa[currentState][symbol].end(); ++it){
		if(validate(fsa, alphabet, final, input, *it, stringPosition + 1)) return true;
	}

	//No valid branches were found in the computation tree
	return false;
}


void outputInfo(int caseNum, int N, vvvi &fsa, mci &alphabet, vector<bool> &final){
	//Output Machine information
		cout << "FSA #" << caseNum << ":\n";

		cout << "(1) Number of States: " << N << endl;

		cout << "(2) Final States: ";

		bool comma = false;
		REP(i,0,N-1){
			if(final[i]){
				if(comma) cout << ", ";
				else comma = true;
				cout << i;
			}
		}
		cout << endl;

		cout << "(3) Alphabet: ";

		comma = false;
		for(mci::iterator it = alphabet.begin(); it != alphabet.end(); ++it){
			if(comma) cout << ", ";
			else comma = true;
			cout << it->first;
		}

		cout << endl;

		cout << "(4) Transitions: " << endl;

		//For each possible state
		REP(i,0,N-1){
			// For each possible symbol
			for(mci::iterator it = alphabet.begin(); it != alphabet.end(); ++it){
				if(fsa[i][it->second].size() > 0) {
					cout << "\t" << i << " " << it->first << " ";
					comma = false;
					for(vi::iterator vit = fsa[i][it->second].begin(); vit != fsa[i][it->second].end(); ++vit){
						if(comma) cout << ", ";
						else comma = true;
						cout << *vit;
					}
					cout << endl;
				}
			}
		}
}


void outputTests(vector<string> &tests, vvvi &fsa, mci &alphabet, vector<bool> &final){
	cout << "(5) Tests:" << endl;

	for(vector<string>::iterator it = tests.begin(); it != tests.end(); ++it){
		cout << "\t" << *it << "\t";
		if((*it).size() < 8) cout << "\t";
		cout << (validate(fsa, alphabet,final, *it, 0,0) ? "Accepted" : "Rejected");
		//cout << "\tExpected: " << (it->second ? "Accepted" : "Rejected");
		cout << endl;
	}
	cout << endl << endl;
}

string nextChar(){
	char c[5];
	cin.get(c,5);
	string aaa(c);
	return aaa;
}

int main() {
	//freopen("output.txt", "w", stdout);
	FILE *fp;
	if((fp = freopen("input.txt", "r", stdin)) == NULL){
		cout << "Cannot open input file. Closing.\n";
		return 1;
	}

	int T = 0;
	cin >> T;

	REP(caseNum,1,T){
		int N = 0, D = 0, U = 0, S = 0;
		int f = 0, fi = 0, a = 0, p = 0, q = 0, qi = 0;
		char ai = '0';
		string s;
		string empty = "";
		bool o = false;
		bool E = false;

		//Input Number of states
		cin >> N;

		//Input Number of final states
		cin >> f;
		vector<bool> final(N, false);


		REP(i,0,f-1){
			cin >> fi;
			final[fi] = true;
		}

		//Input number of symbols in alphabet
		cin >> a;
		mci alphabet;

		REP(i,0,a-1){
			cin >> ai;
			alphabet[ai] = i;
		}

		//Initialize Transition Table
		vvvi transitionTable(N);
		REP(i,0,N-1){
			transitionTable[i].resize(a);
		}

		//Input number of Deterministic transitions
		cin >> D;

		REP(i,0,D-1){
			cin >> p;
			cin >> ai;
			cin >> q;

			transitionTable[p][alphabet[ai]].push_back(q);
		}

		//Input number of Non-Deterministic transitions
		cin >> D;

		REP(i,0,D-1){
			cin >> p;
			cin >> ai;
			cin >> q;

			REP(j,0,q-1){
				cin >> qi;
				transitionTable[p][alphabet[ai]].push_back(qi);
			}
		}

		//Input number of Test Strings
		cin >> S;
		vector<string> tests;

		cin.ignore();
		REP(i,0,S-1){
			getline(cin,s);
			tests.push_back(s);
		}
		outputInfo(caseNum, N, transitionTable, alphabet, final);
		outputTests(tests, transitionTable, alphabet, final);
	}

	fclose(fp);
	return 0;
}

