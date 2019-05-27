#include<iostream>
#include"WordCounter.h"
using namespace std;
int main() {
   WordCounter w(100);
   w.addWord("possum");
   w.addWord("possum");
   w.addWord("calhoun");
   cout << w.getTotalWordCount() << endl;
   w.display();
}
