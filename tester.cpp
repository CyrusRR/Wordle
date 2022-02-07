#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>

#include <stdio.h>

#define YEL   "\x1B[33m"
#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"
using namespace std;

string keyspacing(char c){
  // Turns char to string with keyboard spacing
  string str(1, c);
  return str + " ";
}

void clrstrm(){
  // Moves output to blank string
  cout << "\033[2J\033[1;1H";
}

string keyboardmaker(string keyboard){
  // Returns the keyboard
  return keyspacing(keyboard.at(16)) + keyspacing(keyboard.at(22)) +keyspacing(keyboard.at(4)) +keyspacing(keyboard.at(17))  +keyspacing(keyboard.at(19))  +keyspacing(keyboard.at(24)) +keyspacing(keyboard.at(20)) +keyspacing(keyboard.at(8)) +keyspacing(keyboard.at(14)) +keyspacing(keyboard.at(15)) +"\n "+keyspacing(keyboard.at(0)) +keyspacing(keyboard.at(18)) +keyspacing(keyboard.at(3)) +keyspacing(keyboard.at(5)) +keyspacing(keyboard.at(6)) +keyspacing(keyboard.at(7)) +keyspacing(keyboard.at(9)) +keyspacing(keyboard.at(10)) +keyspacing(keyboard.at(11)) +"\n   "+keyspacing(keyboard.at(25)) +keyspacing(keyboard.at(23)) +keyspacing(keyboard.at(2)) +keyspacing(keyboard.at(21)) +keyspacing(keyboard.at(1)) +keyspacing(keyboard.at(13)) +keyspacing(keyboard.at(12));
}

bool print_info(string &guess, string &answer, unordered_set <char> answer_set, string& key_board, string &report, bool found, int len, bool edit){
  found = true;
  for (int i =0; i < len; i++){
    if (answer_set.find(guess.at(i)) != answer_set.end()) {//if the spot is correct,
      if (answer.at(i) == guess.at(i))  {
	cout << GRN <<guess.at(i)<< RESET;//print it in green
	if (edit) report +="🟩";// green emoji
      }
	   
      else {
	cout << YEL <<guess.at(i)<< RESET;//if incorrect spot, print in yellow
	if (edit)report +="🟨";// yellow emoji
	found = false;// not the correct word
      }
    }
    else {
      cout << guess.at(i);// if not found, print in black
      if (edit) report += "⬛";// black emoji
      found = false;;// not the correct word
    }
    if (edit) key_board.at(guess.at(i)-97) = ' ';// erase the char in the keyboard if letter is used
    
  }
  return found;
    
}


int main(){
  srand(time(NULL));
  // Dictionary w all words
  unordered_set<string> dic;

  // used to build the words in the dict
  string word = "";
  // parse file
  char g;
  
  // the length of all words in the dict
  int len = 5;

  
  // parse thru the dict file
  ifstream file("engmix.txt");

  
  // Keeps track of words in the dictionary
  int count = 1;

  // Holds the keys in the keyboard
  string keyb = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  // string that shows the keyboard
  string keys = keyboardmaker(keyb);

  // used to randomize the chosen word, bc hashset doesn't allow for O(1) randmomization. This sacrifices O(n) space complexity
  vector <string> posswords;

  // parse thru file, if the word.length() == desired length, add to the Hashet and the possible words.
  while (file.get(g)){
    if (g == '\n'){
      if (word.length() ==len){
	count ++;
	posswords.push_back(word);
	dic.insert(word);
      }
      word = "";
    }
    else{
      word += g;
    }
  }

  // Inserts the last word
  dic.insert(word);

  // number of guesses required
  int guesses = 6;

  // Finds a random word to be the answer
  string answer = posswords[rand()%count];

  // Used to compare in O(1) time
  unordered_set <char> theword;

  // Creates a set for comparison
  for (int i =0; i < len; i++){
    theword.insert(answer.at(i));
  }

  // The user's guess
  string guess = "";

  cout << "Guess a word\n";

  // If answer is solved
  bool found = true;

  // Holds the emoji representaton
  string report = "";

  vector <string> prev_guesses;

  cout << keys << endl;

 
  while (guesses >0){
    found = true;

    cin >> guess;
    clrstrm();

    // look at previous guesses
    for (int i =0; i<prev_guesses.size(); i++){
      print_info(prev_guesses[i], answer, theword, keyb, report,found,len,false);
      cout << endl;
    }

    // keeps prompting for a guess until a valid word
    if (dic.find(guess) == dic.end()){
      cout << "Must use a valid word of length " << len<< endl;
      cout << keys << endl;
      continue;
    }

    // parse thru the guess string. If each char is found in the hashmap...
    found = print_info(guess, answer, theword, keyb, report,found,len,true);
    prev_guesses.push_back(guess);

      //bool print_info(string guess, string answer, unordered_set <char> answer_set, vector <char> key_board, string report, bool found, int len){

	       
    if (found){// if never enter a black or yellow condition, it has been found
      cout << "\nCorrect! You got " << answer << " in " << 6-guesses +1 << " guesses!"<< endl;
      break;
    }
    cout << endl;

    // rebuilds the keyboard with newly erased letters
    keys = keyboardmaker(keyb);
    cout<< keys<< endl;


    guesses --;
    report +="\n";
  }
  // Exited due to out of guesses.
  if (!found){
    cout << "Dang, the word was " << answer << endl;
  }
  // shows emoji representation.
  cout << report<< endl;


}
