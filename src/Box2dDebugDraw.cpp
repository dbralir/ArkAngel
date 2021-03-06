//
// Created by Anthony on 5/27/2016.
//

#include "Box2dDebugDraw.hpp"
#include "Print.hpp"
#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include "LineShape.hpp"

using namespace std;

void Box2dDebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    sf::ConvexShape s;

    s.setPointCount(vertexCount);
    s.setOutlineColor(B2SFColor(color));
    s.setOutlineThickness(1.f);

    vector<sf::Vector2f> points;
    sf::Vector2f center;
    for (int i = 0; i < vertexCount; i++)
    {
        points.emplace_back((sf::Vector2f(vertices[i].x * PixelsPerMeter, vertices[i].y * -PixelsPerMeter)));
        center += points.back();
    }
    center /= (float)points.size();

    for (int i = 0; i < vertexCount; i++)
    {
        sf::Vector2f p = points[i] - center;
        thor::setLength(p, thor::length(p) - 1.4f);
        s.setPoint(i, p + center);
    }

    Window->draw(s);
}

void Box2dDebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    sf::ConvexShape s;

    s.setPointCount(vertexCount);
    s.setFillColor(B2SFColor(color, 50));
    s.setOutlineColor(B2SFColor(color));
    s.setOutlineThickness(1.f);

    vector<sf::Vector2f> points;
    sf::Vector2f center;
    for (int i = 0; i < vertexCount; i++)
    {
        points.emplace_back((sf::Vector2f(vertices[i].x * PixelsPerMeter, vertices[i].y * -PixelsPerMeter)));
        center += points.back();
    }
    center /= (float)points.size();

    for (int i = 0; i < vertexCount; i++)
    {
        sf::Vector2f p = points[i] - center;
        thor::setLength(p, thor::length(p) - 1.4f);
        s.setPoint(i, p + center);
    }

    Window->draw(s);
}

void Box2dDebugDraw::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color)
{
    sf::CircleShape s;

    s.setPosition(center.x * PixelsPerMeter - radius * PixelsPerMeter + 1, center.y * -PixelsPerMeter - radius * PixelsPerMeter + 1);
    s.setRadius(radius * PixelsPerMeter - 1);
    s.setOutlineThickness(1.f);
    s.setOutlineColor(B2SFColor(color));

    Window->draw(s);
}

void Box2dDebugDraw::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color)
{
    sf::CircleShape s;

    s.setPosition(center.x * PixelsPerMeter - radius * PixelsPerMeter + 1, center.y * -PixelsPerMeter - radius * PixelsPerMeter + 1);
    s.setRadius(radius * PixelsPerMeter - 1);
    s.setFillColor(B2SFColor(color, 50));
    s.setOutlineColor(B2SFColor(color));
    s.setOutlineThickness(1.f);

    b2Vec2 p = center + radius * axis;
    DrawSegment(center, p, color);

    Window->draw(s);
}

void Box2dDebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
    sf::Vector2f Point1(p1.x * PixelsPerMeter, p1.y * -PixelsPerMeter);
    sf::Vector2f Point2(p2.x * PixelsPerMeter, p2.y * -PixelsPerMeter);
    LineShape segment(Point1, Point2, 1.f, B2SFColor(color));

    Window->draw(segment);
}

void Box2dDebugDraw::DrawTransform(const b2Transform &xf)
{
    float lineProportion = 0.15; // 0.15 ~ 10 pixels
    b2Vec2 p1 = xf.p, p2;

    //red (X axis)
    p2 = p1 + (lineProportion * xf.q.GetXAxis());
    sf::Vertex redLine[] = {
            sf::Vertex(sf::Vector2f(p1.x * PixelsPerMeter, p1.y * PixelsPerMeter), sf::Color::Red),
            sf::Vertex(sf::Vector2f(p2.x * PixelsPerMeter, p2.y * PixelsPerMeter), sf::Color::Red)
    };

    //green (Y axis)
    p2 = p1 - (lineProportion * xf.q.GetYAxis());
    sf::Vertex greenLine[] = {
            sf::Vertex(sf::Vector2f(p1.x * PixelsPerMeter, p1.y * PixelsPerMeter), sf::Color::Green),
            sf::Vertex(sf::Vector2f(p2.x * PixelsPerMeter, p2.y * PixelsPerMeter), sf::Color::Green)
    };

    Window->draw(redLine, 2, sf::Lines);
    Window->draw(greenLine, 2, sf::Lines);
}

sf::Color Box2dDebugDraw::B2SFColor(const b2Color &color, int alpha)
{
    return sf::Color((sf::Uint8)(color.r*255), (sf::Uint8)(color.g*255), (sf::Uint8)(color.b*255), (sf::Uint8) alpha);
}

float Box2dDebugDraw::getPixelsPerMeter()
{
    return PixelsPerMeter;
}

float Box2dDebugDraw::getMetersPerPixel()
{
    return MetersPerPixel;
}