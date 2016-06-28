#include "nsp.hpp"
#include "solver.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char const *argv[])
{
	if(argc < 2) {
		cout << "Número incorrecto de argumentos";
		return 1;
	}

	if(argc == 2) {

		pid_t pid = fork();

		if(pid == 0) {
			setsid();
			NSP instance = NSP(argv[1]);
			Solver sv = Solver(instance);
			sv.solve();
		} else if(pid > 0) {
			cout << "Presiona CTRL + C para detener la ejecución..." << endl;
			while(wait(NULL) > 0) { /* no-op */ ; }
			cout << endl;
		} else {
			cout << "Ocurrió un error..." << endl;
		}
	}

	return 0;
}