#pragma once
#include "../Utilities/Node.h"
#include <vector>


//Class to represent the grid, the entire map
class MyGrid
{
public:

    //  Anything with a mask that defines where it can't walk
    //LayerMask unwalkableMask;
    //Size the grid will cover 30x30
    Vector2 gridWorldSize{ 250, 500 };
    // How much space each node covers
    float nodeRadius = 0.5;
    std::vector< std::vector<Node> > grid;

    float nodeDiameter;
    // Amount of nodes fits into our grid world
    int gridSizeX, gridSizeY;

    std::vector<Vector3> locations;
    std::vector<Vector3> sizes;

    float enemyRadiusX;
    float enemyRadiusY;
    float enemyRadiusZ;

    Vector3 currentLocation;


    // Defines how many nodes we can fit into our grid world size
    void Commence();

    bool CheckCollision(Vector3 worldPoint);

    void CreateGrid();

    // Gets a list of all the neighbouring nodes
    std::vector<Node> GetNeighbours(Node node);

    // Get the node from where the player is currently standing from the grid
    Node NodeFromWorldPoint(Vector3 worldPosition);
};

