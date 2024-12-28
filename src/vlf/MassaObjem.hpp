#pragma once

#include "MySyst.hpp"

struct MassaObjem
{
	double m, V;

	MassaObjem()
		:m(0), V(0)
	{};

	MassaObjem(double m_, double V_)
		:m(m_), V(V_)
	{};

	MassaObjem& operator+=(const MassaObjem& mm1)
	{
		m += mm1.m;
		V += mm1.V;
		return *this;
	};

	MassaObjem& operator-=(const MassaObjem& mm1)
	{
		m -= mm1.m;
		V -= mm1.V;
		return *this;
	};

	bool primet(const MassaObjem& mm1) // То есть фура с оставшшимся местом this примет груз mm1
	{
		return ((m >= mm1.m) && (V >= mm1.V));
		
	};
};
