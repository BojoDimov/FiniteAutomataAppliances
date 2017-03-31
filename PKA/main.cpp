#include <iostream>
#include "FSA.h"


int main() {
	FSA automata(
	{
		{15, true, false},
		{69, true, false},
		{120, false, false},
		{8, false, true}
	}, 
	{
		{15, "baba qga", 120},
		{69, "dedo ceko", 120},
		{120, "qj", 8},
		{8, "qj", 8},
		{8, "hello", 69}
	});

	FSA automata1(
	{
		{ 15, true, false },
		{ 69, false, false },
		{ 120, false, true }
	},
	{
		{ 15, "qm", 69 },
		{ 69, "shokolad", 120 },
		{ 120, "qm", 69 }
	});

	

	FSA automata2 = FSA::Union(automata, automata1);
	FSA automata3 = FSA::Concat(automata, automata1);
	FSA automata4 = FSA::Star(automata1, false);

	bool sth = automata.check({ "baba qga", "qj", "qj" });
	bool sth2 = automata2.check({ "baba qga", "qj", "qj" });
	bool sth3 = automata2.check({ "qm", "shokolad", "qm", "shokoladchge" });
	bool sth4 = automata3.check({ "baba qga", "qj", "qm", "shokolad" });
	bool sth5 = automata3.check({ "baba qga", "qj", "alalala", "qm", "shokolad" });
	bool sth6 = automata1.check({});
	bool sth7 = automata4.check({});
	bool sth8 = automata4.check({ "qm", "shokolad" });
	return 0;
}