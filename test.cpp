#include<iostream>
#include"WordCounter.h"
using namespace std;
int main() {
   WordCounter w(4);
   w.addWord("possum");
   w.addWord("possum");
   w.addWord("calhoun");
   w.addWord("possum");
   w.addWord("possum");
   w.addWord("calhoun");
   w.addWord("possum");
   w.addWord("possum");
   w.addWord("calhoun");
   w.addWord("possum");
   w.addWord("possum");
   w.addWord("calhoun");
   w.removeWord("possum");
   WordCounter w2(w);
   WordCounter w3 = w2;
   cout << w2.getTotalWordCount() << endl;
   cout << "possum number: " << w3.getWordCount("possum") << endl;
   w.removeWord("possum");
   w.removeWord("calhoun");
   cout << w.getTotalWordCount() << endl;
   //cout << "possum number: " << w.getWordCount("possum") << endl;

}
