#include "lexicon.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

Lexicon lex("../EnglishWords.txt");
unsigned long long score = 0;
unsigned long long computerScore = 0;

void find(std::vector<std::string>& total, int n, char** letter,bool** flag, int x, int y, int length, std::string word = "") {
    word.push_back(letter[x][y]);
    ++length;
    flag[x][y] = false;
    if (!lex.containsPrefix(word)) {
        flag[x][y] = true;
        return;
    }
    if (word.length() >= 4 && lex.contains(word) && (std::find(total.begin(), total.end(), word) == total.end())) {
        total.push_back(word);
        computerScore += word.length() - 3;
    }
    if (x > 0){
        if(flag[x - 1][y])
            find(total, n, letter, flag, x - 1, y, length, word);
        if(y > 0 && flag[x - 1][y - 1])
            find(total, n, letter, flag, x - 1, y - 1, length, word);
        if(y < n - 1 && flag[x - 1][y + 1])
            find(total, n, letter, flag, x - 1, y + 1, length, word);
    }
    if (x < n - 1){
        if(flag[x + 1][y])
            find(total, n, letter, flag, x + 1, y, length, word);
        if(y > 0 && flag[x + 1][y - 1])
            find(total, n, letter, flag, x + 1, y - 1, length, word);
        if(y < n - 1 && flag[x + 1][y + 1])
            find(total, n, letter, flag, x + 1, y + 1, length, word);
    }
    if (y > 0 && flag[x][y - 1])
        find(total, n, letter, flag, x, y - 1, length, word);
    if (y < n - 1 && flag[x][y + 1])
        find(total, n, letter, flag, x, y + 1, length, word);
    flag[x][y] = true;
}

void findAllWord(std::vector<std::string>& total, int n, char** letter, bool** flag){
    for(int i =0 ;i < n;++i) {
        for (int j = 0; j < n; ++j) {
            find(total, n, letter,flag, i, j, 0);
        }
    }
}

void output(std::vector<std::string> word){
    std::cout << "Computer Words: ";
    for(std::vector<std::string>::iterator it = word.begin();it != word.end();++it){
        std::cout << *it << " ";
    }
    std::cout << '\n';
}

char upper(char a) {
    if(a >= 'a' && a <= 'z')
        a += 'A' -'a';
    return a;
}

char lower(char a) {
    if(a >= 'A' && a <= 'Z')
        a += 'a' - 'A';
    return a;
}

int main() {
  // TODO
  int N;
  std::cin >> N;
  char ** letter = new char*[N];
  bool ** flag = new bool*[N];
  for(int i = 0;i < N;++i){
      letter[i] = new char[N];
      flag[i] = new bool[N];
      for(int j = 0; j < N;++j){
          std::cin >> letter[i][j];
          letter[i][j] = upper(letter[i][j]);
          flag[i][j] = true;
      }
  }
  std::string word;
  std::string end_word = "???";
  std::vector<std::string> playerWord;
  std::vector<std::string> computerWord;

  findAllWord(computerWord, N, letter, flag);
  std::sort(computerWord.begin(),computerWord.end());
  std::getline(std::cin,word);
  while(true){
      for(int i = 0;i < N;++i){
          for(int j = 0;j < N;++j){
              std::cout << letter[i][j];
          }
          std::cout << '\n';
      }
      std::cout << "Your Score: " << score << '\n';
      std::cout << "Your Words: " ;
      for(auto it = playerWord.begin();it != playerWord.end();++it){
          std::cout << *it << " ";
      }
      std::cout << '\n';
      std::getline(std::cin,word);
      if(word == end_word){
          std::cout << "Computer Score: " << computerScore << '\n';
          output(computerWord);
          break;
      }
      if(word.length() < 4) {
          std::cout << word << " is too short.\n";
          continue;
      }
      std::string word_copy = word;
      for(int i = 0;i < word_copy.length();++i)
          word_copy[i] = lower(word[i]);
      if(!lex.contains(word)){
          std::cout << word << " is not a word.\n";
          continue;
      }
      for(int i = 0;i < word_copy.length();++i)
          word_copy[i] = upper(word[i]);
      auto position1 = std::find(computerWord.begin(), computerWord.end(), word_copy);
      for(int i = 0;i < word_copy.length();++i)
          word_copy[i] = lower(word[i]);
      auto position2 = std::find(playerWord.begin(), playerWord.end(), word_copy);
      if(position1 == computerWord.end() && position2 == playerWord.end()) {
          std::cout << word << " is not on board.\n";
          continue;
      }
      for(int i = 0;i < word_copy.length();++i)
          word_copy[i] = lower(word[i]);
      if(position2 != playerWord.end()){
          std::cout << word << " is already found.\n";
          continue;
      }
      playerWord.push_back(word_copy);
      score += word.length() - 3;
      computerWord.erase(position1);
      computerScore -= word.length() - 3;
  }


  for(int i = 0;i < N;++i){
      delete []letter[i];
      delete []flag[i];
  }
  delete []letter;
  delete []flag;
  return 0;
}
