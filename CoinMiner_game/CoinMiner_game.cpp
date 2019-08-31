// CoinMiner_game.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "big_integer.h"
#include <iostream>
#include <string>
#include <Windows.h>

using namespace lib::literal;
int main() {
	lib::big_integer bi = -10000;
	long long l = -10000;
	while (bi == l) {
		std::cout << bi << "\n";
		++bi;
		++l;
	}
	
}
