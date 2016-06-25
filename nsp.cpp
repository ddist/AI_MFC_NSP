#include "nsp.hpp"

NSP::NSP (const char* file) {
	ifstream nsp (file);

	if(nsp.is_open()) {
		nsp >> this->N;
		nsp >> this->D;
		nsp >> this->S;

		this->preferences = new NUMBER*[this->N];
		for (int i = 0; i < this->N; ++i)
		{
			this->preferences[i] = new NUMBER[this->D*this->S];
		}
		this->demand = new NUMBER*[this->D];
		for (int i = 0; i < this->D; ++i)
		{
			this->demand[i] = new NUMBER[this->S];
		}
		this->minSameConsec = new NUMBER[this->S];
		this->maxSameConsec = new NUMBER[this->S];
		this->minShiftAssign = new NUMBER[this->S];
		this->maxShiftAssign = new NUMBER[this->S];

		for (int i = 0; i < this->D; ++i)
		{
			for (int j = 0; j < this->S; ++j)
			{
				nsp >> this->demand[i][j];
			}
		}

		for (int i = 0; i < this->N; ++i)
		{
			for (int j = 0; j < this->D*this->S; ++j)
			{
				nsp >> this->preferences[i][j];
			}
		}

		nsp >> this->D;
		nsp >> this->S;
		nsp >> this->minAssign;
		nsp >> this->maxAssign;
		nsp >> this->minConsec;
		nsp >> this->maxConsec;

		for (int i = 0; i < this->S; ++i)
		{
			nsp >> this->minSameConsec[i];
			nsp >> this->maxSameConsec[i];
			nsp >> this->minShiftAssign[i];
			nsp >> this->maxShiftAssign[i];
		}

	} else {
		cout << "No se pudo abrir el archivo " << file << endl;
	}
}