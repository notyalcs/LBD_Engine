#pragma once
#include "Utilities.h"

struct Vector2 {
    float x;
    float y;
    Vector2(float x, float y) : x(x), y(y) {}
};

struct Vector3 {
    float x;
    float y;
    float z;
    Vector3() = default;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    friend Vector3 operator-(Vector3 lhs, const Vector3& rhs) {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        lhs.z -= rhs.z;
        return lhs;
    }

    friend Vector3 operator+(Vector3 lhs, const Vector3& rhs) {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        lhs.z += rhs.z;
        return lhs;
    }

    bool operator==(const Vector3& v) {
        return (x == v.x && y == v.y && z == v.z);
    }

    bool operator!=(const Vector3& v) {
        return !operator==(v);
    }

    bool operator< (const Vector3& v)
    {
        return (x < v.x && z < v.z);
    }

};

//Class to represent a world position.
class Node
{
public:

    bool walkable;
    Vector3 worldPosition;
    // Position of node in the array of nodes
    int gridX;
    int gridY;

    int gCost;
    int hCost;
    // Keeps track of parent node/last node (relative path it takes, parent node is the node before it in the path)
    Node *parent;

    Node() = default;

    Node(bool _walkable, Vector3 _worldPos, int _gridX, int _gridY)
    {
        walkable = _walkable;
        worldPosition = _worldPos;
        gridX = _gridX;
        gridY = _gridY;
    }

    int fCost()
    {
        return gCost + hCost;
    }

    bool operator==(const Node& n) {
        return (walkable == n.walkable && worldPosition == n.worldPosition && gridX == n.gridX && gridY == n.gridY);// && gCost == n.gCost && hCost == n.hCost && parent == n.parent);
    }

    bool operator!=(const Node& n) {
        return !operator==(n);
    }

    Node& operator=(const Node& n)
    {
        walkable = n.walkable;
        worldPosition = n.worldPosition;
        gridX = n.gridX;
        gridY = n.gridY;
        gCost = n.gCost;
        hCost = n.hCost;
        parent = n.parent;

        return *this;
    }
    Node(const Node& n) {
        walkable = n.walkable;
        worldPosition = n.worldPosition;
        gridX = n.gridX;
        gridY = n.gridY;
        gCost = n.gCost;
        hCost = n.hCost;
        parent = new Node;
        if (n.parent != nullptr) {
            *parent = *n.parent;
        }
        else {
            parent = nullptr;
        }
    }
};