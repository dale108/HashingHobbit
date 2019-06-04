#include<iostream>
#include "WordCounterx.h"
using namespace std;
int main() {
   WordCounter w(100);
   cout << w.getCapacity() << endl;
   for(int i = 0; i < 10; i++) {

      w.addWord("possum");
      w.addWord("possum");
      w.addWord("calhoun");
      w.addWord("possum");
      w.addWord("summons");
      w.addWord("cat");
      w.addWord("talce");
      w.addWord("terse");
      w.addWord("fallcall");
      w.addWord("possusm");
      w.addWord("possumx");
      w.addWord("calhounx");
      w.addWord("possxumx");
      w.addWord("summxonsx");
      w.addWord("caxt");
      w.addWord("talcxe");
      w.addWord("tersex");
      w.addWord("falxlcxall");
      w.addWord("possusxxm");
      w.addWord("possuxxxmx");
      w.addWord("calhounxx");
      w.addWord("possxumxx");
      w.addWord("summxonxsx");
      w.addWord("caxtx");
      w.addWord("talcxxe");
      w.addWord("texrsexx");
      w.addWord("fallcxxall");


   }

}
