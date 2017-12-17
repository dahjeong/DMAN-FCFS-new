#pragma once

#include "Node.h"

#include <vector>
#include <algorithm>

using namespace std;


class Nodes
{
private:
	vector<Node> nodes;

public:
	Nodes();

	void	add(Node node);
	void	erase(int i);

	void	clear();

	int		getSize() const;
	
	Node	get(int i) const;
	Node	get(string id) const;

	Nodes	getSameTypes(string type);

	void	setInitialConstraints(string id, Count depRate, Count arrRate, Count passRate);
	
	void	update(string id, Time time, int index, string state, string wakeCategory, int direction);

	/* for multiple scheduling */
	void	erase(string id, Time time, int index, string state, string wakeCategory, int direction);
};

