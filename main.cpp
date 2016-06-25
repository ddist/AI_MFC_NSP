#include "nsp.hpp"
#include "solver.hpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[])
{
	if(argc < 2) {
		cout << "Número incorrecto de argumentos";
		return 1;
	}

	if(argc == 2) {
		NSP instance = NSP(argv[1]);
		Solver sv = Solver(instance);
		sv.solve();
	}

	return 0;
}