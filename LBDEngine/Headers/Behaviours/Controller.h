#pragma once
#include "../Utilities/Utilities.h"
#include "Behaviour.h"
#include "PhysicsBody.h"

class Controller : public Behaviour
{
public:
    void Update() override;

    POINT GetLastMousePosition() const
    {
        return _lastMousePosition;
    }

    POINT GetCurrentMousePosition() const
    {
        return _currentMousePosition;
    }

    float GetIsMouseDown() const
    {
        return _isMouseDown;
    }
    
    float GetHorizontalAxis() const
    {
        return _horizontalAxis;
    }

    float GetVerticalAxis() const
    {
        return _verticalAxis;
    }

    bool GetKey(int key) const
    {
        return GetAsyncKeyState(key) & HighBitMask;
    }

private:
    const int HighBitMask{ 0x8000 };

    POINT _lastMousePosition;
    POINT _currentMousePosition;

    float _horizontalAxis;
    float _verticalAxis;

    bool _isMouseDown;

    void ResetController();

};
