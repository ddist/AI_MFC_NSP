#include <iostream>
#include <fstream>
#include <string>

#ifndef _NSP_HPP_INCLUDED_
#define _NSP_HPP_INCLUDED_

#define NUMBER unsigned short int

using namespace std;

class NSP {	
	
	public:
		NSP(const char*);

		NUMBER N, D, S; // N = numero de enferemeras, D = dias de programacion, S = turnos por dia
		/* Restricciones generales */
		NUMBER **demand; // Tamaño [D][S]
		NUMBER **preferences; // Tamaño [N][DxS]
		/* Restriciones extras */
		NUMBER minAssign, maxAssign; // min/max numero de asignaciones por enfermera
		NUMBER minConsec, maxConsec; // min/max turnos consecutivos por enfermera
		NUMBER *minSameConsec, *maxSameConsec; // min/max turnos iguales consecutivos de trabajo
		NUMBER *minShiftAssign, *maxShiftAssign; // min/max numero de asignaciones por turno
};

#endif