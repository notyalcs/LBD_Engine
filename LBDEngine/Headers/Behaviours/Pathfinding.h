#pragma once
#include "../Utilities/Node.h"
#include "../Utilities/MyGrid.h"
#include <vector>
#include <unordered_set>
#include <iostream>

class Pathfinding
{
public:

    MyGrid grid;

    std::vector<Node> pathToGoal;

    // Refer to the PSEUDO-CODE in A* algorithm notes
    void FindPath(Vector3 startPos, Vector3 targetPos);

    // Retraces and calculates what the path it took was, the parent node in the Node class is vital
    void RetracePath(Node startNode, Node endNode);

    // Function to get the distance of two nodes
    int GetDistance(Node nodeA, Node nodeB);


};
