#include "../../Headers/Utilities/MyGrid.h"

namespace LBD {
    // Defines how many nodes we can fit into our grid world size
    void MyGrid::Commence()
    {
        nodeDiameter = nodeRadius * 2;
        gridSizeX = round(gridWorldSize.x / nodeDiameter);
        gridSizeY = round(gridWorldSize.y / nodeDiameter);
        CreateGrid();

    }

    bool MyGrid::CheckCollision(Vector3 worldPoint) {
        bool walkable = false;

        float left = worldPoint.x - enemyRadiusX;
        float right = worldPoint.x + enemyRadiusX;
        float top = worldPoint.y + enemyRadiusY;
        float bottom = worldPoint.y - enemyRadiusY;
        float back = worldPoint.z + enemyRadiusZ;
        float front = worldPoint.z - enemyRadiusZ;

        for (int i = 0; i < locations.size(); i++) {
            int locationLeft = locations[i].x - sizes[i].x;
            int locationRight = locations[i].x + sizes[i].x;
            int locationTop = locations[i].y + sizes[i].y;
            int locationBottom = locations[i].y - sizes[i].y;
            int locationBack = locations[i].z + sizes[i].z;
            int locationFront = locations[i].z - sizes[i].z;


            if (((locationLeft >= left && locationLeft <= right) || (locationRight >= left && locationRight <= right)) &&
                ((locationBottom >= bottom && locationBottom <= top) || (locationTop >= bottom && locationTop <= top)) &&
                ((locationFront >= front && locationFront <= back) || (locationBack >= front && locationBack <= back))) {
                return false;
            }
            if (((left >= locationLeft && left <= locationRight) || (right >= locationLeft && right <= locationRight)) &&
                ((bottom >= locationBottom && bottom <= locationTop) || (top >= locationBottom && top <= locationTop)) &&
                ((front >= locationFront && front <= locationBack) || (back >= locationFront && back <= locationBack))) {
                return false;
            }


            if (locationTop == locationBottom && locations[i].y >= 0.0f) {
                if (left >= locationLeft && right <= locationRight &&
                    front >= locationFront && back <= locationBack) {
                    walkable = true;
                }
            }

        }

        return walkable;
    }

    void MyGrid::CreateGrid()
    {
        //grid = new Node[gridSizeX, gridSizeY];
        grid.resize(gridSizeX);

        // WorldBottomLeft = (position (0, 0, 0)) - (left edge of the world) - (bottom left corner)
        Vector3 worldBottomLeft = Vector3(0, 0, 0) - Vector3(1 * gridWorldSize.x / 2, 0, 0) - Vector3(0, 0, 1 * gridWorldSize.y / 2); // Vector3.forward gives the z axis in 3d space

        for (int x = 0; x < gridSizeX; x++)
        {
            grid[x].resize(gridSizeY);
            for (int y = 0; y < gridSizeY; y++)
            {
                // As x increases we go in increments of node diameter until we reach the edge (same thing for y, aka z axis for world space)
                // Each point a node will occupy
                Vector3 worldPoint = worldBottomLeft + Vector3(1 * (x * nodeDiameter + nodeRadius), 0, 0) + Vector3(0, 0, 1 * (y * nodeDiameter + nodeRadius));
                // Collision check (true if we don't collide with anything in the walkable mask)
                bool walkable = CheckCollision(worldPoint);//!(Physics.CheckSphere(worldPoint, nodeRadius, unwalkableMask));
                // Create a point on the grid using the Node class
                grid[x][y] = Node{ walkable, worldPoint, x, y };
            }
        }
    }

    // Gets a list of all the neighbouring nodes
    std::vector<Node> MyGrid::GetNeighbours(Node node)
    {
        std::vector<Node> neighbours;
        // Search by a 3x3 block around node
        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                //Skip the current node
                if (x == 0 && y == 0)
                {
                    continue;
                }
                int checkX = node.gridX + x;
                int checkY = node.gridY + y;

                if (checkX >= 0 && checkX < gridSizeX && checkY >= 0 && checkY < gridSizeY)
                {
                    neighbours.push_back(grid[checkX][checkY]);
                }
            }
        }
        return neighbours;
    }

    // Get the node from where the player is currently standing from the grid
    Node MyGrid::NodeFromWorldPoint(Vector3 worldPosition)
    {
        // Gets the position in percentage of the world size (middle would be 0.5, far left is 0, far right would be 1)
        float percentX = (worldPosition.x + gridWorldSize.x / 2) / gridWorldSize.x;
        // NOTE: world position z because z in the world space is y in the grid space
        float percentY = (worldPosition.z + gridWorldSize.y / 2) / gridWorldSize.y;
        // Line will make sure it is not out of the world position (without might cause invalid index)
        percentX = MathHelper::Clamp(percentX, 0.0f, 1.0f);
        percentY = MathHelper::Clamp(percentY, 0.0f, 1.0f);
        // Gets the index of the grid array (minus 1 because of index of array)
        int x = round((gridSizeX - 1) * percentX);
        int y = round((gridSizeY - 1) * percentY);
        return grid[x][y];
    }
}
