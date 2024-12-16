#include "Cube.h"
#include <array>
#include <cmath>

// User-defined variables
//float size = 2.0f;

float angleX = 0.01f;
float angleY = 0.02f;

// Vertices buffer
std::array<Point3D, 8> vertices;
std::array<sf::Vector2f, 8> projectedVertices;
sf::VertexArray edges(sf::Lines, 24);

// Background color transition
sf::Color bgColor[3] = {
    sf::Color(129, 116, 160),
    sf::Color(255, 210, 160),
    sf::Color::Yellow
};
int colourIndex = 0;
sf::Color currentColor;
float smoothGradient = 1.0f;

// Window initialization
void RenderinngWindow::initVariables() {
    this->rWindow = nullptr;
}

//frame limit to 12
void RenderinngWindow::initWindow() {
    this->videoMode.height = 540;
    this->videoMode.width = 960;
    this->rWindow = new sf::RenderWindow(this->videoMode, "The Cube", sf::Style::Titlebar | sf::Style::Close);
    this->rWindow->setFramerateLimit(60);

    currentColor = bgColor[0];
    InitCube();
}

RenderinngWindow::RenderinngWindow() {
    this->initVariables();
    this->initWindow();
}

RenderinngWindow::~RenderinngWindow() {
    delete this->rWindow;
}

const bool RenderinngWindow::getWindowIsOpen() const {
    return this->rWindow->isOpen();
}

void RenderinngWindow::pollEvents() {
    while (this->rWindow->pollEvent(this->ev)) {
        switch (this->ev.type) {
        case sf::Event::Closed:
            this->rWindow->close();
            break;

        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape)
                this->rWindow->close();
            break;
        }
    }
}

// Cube Initialization
void InitCube() {
    vertices = {
        Point3D{-1, -1, -1}, Point3D{1, -1, -1},
        Point3D{1, 1, -1}, Point3D{-1, 1, -1},
        Point3D{-1, -1, 1}, Point3D{1, -1, 1},
        Point3D{1, 1, 1}, Point3D{-1, 1, 1}
    };
}

// Cube Rotation
Point3D rotateX(const Point3D& point, float angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    return { point.x, point.y * cosA - point.z * sinA, point.y * sinA + point.z * cosA };
}

Point3D rotateY(const Point3D& point, float angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    return { point.x * cosA + point.z * sinA, point.y, -point.x * sinA + point.z * cosA };
}

Point3D rotateZ(const Point3D& point, float angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    return { point.x * cosA - point.y * sinA, point.x * sinA + point.y * cosA, point.z };
}

void RotateCube() {
    for (size_t i = 0; i < vertices.size(); ++i) {
        Point3D rotated = vertices[i];
        rotated = rotateX(rotated, angleX);
        rotated = rotateY(rotated, angleY);
        vertices[i] = rotated;
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        projectedVertices[i] = projectPoint(vertices[i], 200.0f, 480.0f, 270.0f);
    }

    std::array<std::pair<int, int>, 12> cubeEdges = {
        std::make_pair(0, 1), std::make_pair(1, 2), std::make_pair(2, 3), std::make_pair(3, 0),
        std::make_pair(4, 5), std::make_pair(5, 6), std::make_pair(6, 7), std::make_pair(7, 4),
        std::make_pair(0, 4), std::make_pair(1, 5), std::make_pair(2, 6), std::make_pair(3, 7)
    };

    for (size_t i = 0; i < cubeEdges.size(); ++i) {
        edges[i * 2].position = projectedVertices[cubeEdges[i].first];
        edges[i * 2 + 1].position = projectedVertices[cubeEdges[i].second];
        edges[i * 2].color = sf::Color::Black;
        edges[i * 2 + 1].color = sf::Color::White;
    }
}

// Projection
sf::Vector2f projectPoint(const Point3D& point, float scale, float centerX, float centerY) {
    float perspective = 4.0f / (10.0f + point.z); // Simple perspective projection
    return { point.x * scale * perspective + centerX, point.y * scale * perspective + centerY };
}

// Background Transition
void UpdateBackground() {
    float absoluteDiff_r = fabs(currentColor.r - bgColor[colourIndex].r);
    float absoluteDiff_g = fabs(currentColor.g - bgColor[colourIndex].g);
    float absoluteDiff_b = fabs(currentColor.b - bgColor[colourIndex].b);

    // Avoid grey colors
    if (fabs(currentColor.r - currentColor.g) < 10 && fabs(currentColor.g - currentColor.b) < 10 && fabs(currentColor.r - currentColor.b) < 10) {
        // Adjust colors to ensure they are not too close
        currentColor.r += 10;
        currentColor.g -= 10;
        currentColor.b += 10;
    }

    if (absoluteDiff_r <= 1) //1 is used as sensitivity
    {
        colourIndex++;
    }

    if (colourIndex >= 2)
        colourIndex = 0;

    if (currentColor.r > bgColor[colourIndex].r && absoluteDiff_r >= 1)
        currentColor.r -= smoothGradient;
    else
        currentColor.r += smoothGradient;

    if (currentColor.g > bgColor[colourIndex].g && absoluteDiff_g >= 1)
        currentColor.g -= smoothGradient;
    else
        currentColor.g += smoothGradient;

    if (currentColor.b > bgColor[colourIndex].b && absoluteDiff_b >= 1)
        currentColor.b -= smoothGradient;
    else
        currentColor.b += smoothGradient;
}

// Window Update & Render
void RenderinngWindow::update() {
    this->pollEvents();
    UpdateBackground();
    RotateCube();
}

void RenderinngWindow::render() {
    this->rWindow->clear(currentColor);
    this->rWindow->draw(edges);
    //for smmmmmmmmmoooooooooooooothness decrease the value .
    //angleX += 0.01f;
    //angleY += 0.02f; sorry skeeping constant speed
    this->rWindow->display();
}
