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

    // Gets the grid from the MyGrid Script
    /*void Awake()
    {
        grid = GetComponent<MyGrid>();
    }*/


    // Refer to the PSEUDO-CODE in A* algorithm notes
    void FindPath(Vector3 startPos, Vector3 targetPos)
    {
        Node startNode = grid.NodeFromWorldPoint(startPos);
        Node targetNode = grid.NodeFromWorldPoint(targetPos);

        // Set of nodes to be evaluated
        std::vector<Node> openSet;
        // Set of nodes already evaluated
        std::vector<Node> closedSet;
        openSet.push_back(startNode);

        while (openSet.size() > 0)
        {
            Node currentNode = openSet[0];
            
            for (int i = 1; i < openSet.size(); i++)
            {
                if (openSet[i].fCost() < currentNode.fCost() || openSet[i].fCost() == currentNode.fCost() && openSet[i].hCost < currentNode.hCost)
                {
                    currentNode = openSet[i];
                }
            }

            openSet.erase(std::remove(openSet.begin(), openSet.end(), currentNode), openSet.end());
            closedSet.push_back(currentNode);

            if (currentNode == targetNode)
            {
                RetracePath(startNode, currentNode);
                return;
            }

            for (Node neighbour : grid.GetNeighbours(currentNode))
            {
                if (!neighbour.walkable || std::count(closedSet.begin(), closedSet.end(), neighbour))
                {
                    continue;
                }

                int newMovementCostToNeighbour = currentNode.gCost + GetDistance(currentNode, neighbour);
                if (newMovementCostToNeighbour < neighbour.gCost || !(std::find(openSet.begin(), openSet.end(), neighbour) != openSet.end()))
                {
                    neighbour.gCost = newMovementCostToNeighbour;
                    neighbour.hCost = GetDistance(neighbour, targetNode);
                    neighbour.parent = &currentNode;

                    // This just check if it doesn't contain
                    if (!(std::count(openSet.begin(), openSet.end(), neighbour)))
                    {
                        openSet.push_back(neighbour);
                    }
                }
            }
        }
    }

    // Retraces and calculates what the path it took was, the parent node in the Node class is vital
    void RetracePath(Node startNode, Node endNode)
    {
        std::vector<Node> path;
        Node currentNode = endNode;

        while (currentNode != startNode)
        {
            path.push_back(currentNode);
            currentNode = *currentNode.parent;
        }
        // Path will be backwards, need to reverse
        std::reverse(path.begin(), path.end());
        pathToGoal = path;
    }

    // Function to get the distance of two nodes
    int GetDistance(Node nodeA, Node nodeB)
    {
        int dstX = abs(nodeA.gridX - nodeB.gridX);
        int dstY = abs(nodeA.gridY - nodeB.gridY);

        // 14 is the unit for diagonals and 10 is non-diagonals, this is a fancy math equation refer to notes on A* algorithm
        if (dstX > dstY)
        {
            return 14 * dstY + 10 * (dstX - dstY);
        }
        return 14 * dstX + 10 * (dstY - dstX);
    }


};
