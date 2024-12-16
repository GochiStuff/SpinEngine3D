#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

// Struct to represent 3D points
struct Point3D {
    float x, y, z;
};

class RenderinngWindow {
private:
    sf::RenderWindow* rWindow;
    sf::VideoMode videoMode;
    sf::Event ev;

    // Private functions
    void initVariables();
    void initWindow();

public:
    RenderinngWindow();
    virtual ~RenderinngWindow();

    // Accessors
    const bool getWindowIsOpen() const;

    // Core functions
    void pollEvents();
    void update();
    void render();
};

// Cube-related functions
void InitCube();
void RotateCube();
void RenderCube();

//backgoround effect
void UpdateBackground();

// Rotation matrix functions
Point3D rotateX(const Point3D& point, float angle);
Point3D rotateY(const Point3D& point, float angle);
Point3D rotateZ(const Point3D& point, float angle);

// Projection function
sf::Vector2f projectPoint(const Point3D& point, float scale, float centerX, float centerY);
