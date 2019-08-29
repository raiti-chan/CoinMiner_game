// CoinMiner_game.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "big_integer.h"
#include <iostream>
#include <string>
#include <Windows.h>

using namespace lib::literal;
int main() {
	lib::big_integer bi(0);
	unsigned long long l = 0;
	while (bi == l) {
		//std::cout << ++bi << ", ";
		bi++;
		l++;
	}
}
