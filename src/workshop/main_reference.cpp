#include <iostream>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

// Box2D -> SFML
constexpr float SCALE = 40.0f;

sf::RectangleShape CreateStaticRectangle(b2WorldId worldId, float posX, float posY, float width, float height, sf::Color color)
{
    b2BodyDef groundDef = b2DefaultBodyDef();
    groundDef.position = {posX / SCALE, posY / SCALE};
    b2BodyId groundId = b2CreateBody(worldId, &groundDef);

    b2Polygon groundBox = b2MakeBox((width / 2.0f) / SCALE, (height / 2.0f) / SCALE);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    sf::RectangleShape groundRect({width, height});
    groundRect.setOrigin({width / 2.0f, height / 2.0f});
    groundRect.setPosition({posX, posY});
    groundRect.setFillColor(color);

    return groundRect;
}



b2BodyId CreatePlayer(b2WorldId worldId, float posX, float posY, float width, float height)
{
    b2BodyDef playerDef = b2DefaultBodyDef();
    playerDef.type = b2_dynamicBody;
    playerDef.position = {posX / SCALE, posY / SCALE};
    playerDef.fixedRotation = true; // Prevents player from tipping over on corners
    b2BodyId playerId = b2CreateBody(worldId, &playerDef);

    b2Polygon playerBox = b2MakeBox((width / 2.0f) / SCALE, (height / 2.0f) / SCALE);
    b2ShapeDef playerShapeDef = b2DefaultShapeDef();
    playerShapeDef.density = 1.0f;
    playerShapeDef.enableSensorEvents = true;
    playerShapeDef.material.friction = 0.0f; // Zero friction prevents sticking to walls mid-jump
    b2CreatePolygonShape(playerId, &playerShapeDef, &playerBox);

    return playerId;
}

sf::RectangleShape CreatePlayerRender(float width, float height, const sf::Texture& texture)
{
    sf::RectangleShape playerRect({width, height});
    playerRect.setOrigin({width / 2.0f, height / 2.0f});

    playerRect.setFillColor(sf::Color::White);
    playerRect.setTexture(&texture);

    return playerRect;
}

b2ShapeId CreateGoal(b2WorldId worldId, float posX, float posY, float width, float height)
{
    b2BodyDef goalDef = b2DefaultBodyDef();
    goalDef.position = {posX / SCALE, posY / SCALE};
    b2BodyId goalId = b2CreateBody(worldId, &goalDef);

    b2Polygon goalBox = b2MakeBox((width / 2.0f) / SCALE, (height / 2.0f) / SCALE);
    b2ShapeDef goalShapeDef = b2DefaultShapeDef();
    goalShapeDef.isSensor = true; // Player passes through without physical collision
    goalShapeDef.enableSensorEvents = true;
    b2ShapeId goalShapeId = b2CreatePolygonShape(goalId, &goalShapeDef, &goalBox);

    return goalShapeId;
}

sf::RectangleShape CreateGoalRender(float posX, float posY, float width, float height, sf::Color color)
{
    sf::RectangleShape goalRect({width, height});
    goalRect.setOrigin({width / 2.0f, height / 2.0f});
    goalRect.setPosition({posX, posY});
    goalRect.setFillColor(color);

    return goalRect;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Workshop");
    window.setFramerateLimit(60);

    // Create Box2D v3 World with downward gravity (Y points down in SFML)
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 25.0f}; 
    b2WorldId worldId = b2CreateWorld(&worldDef);

    // Create Static Ground Platform (center at 400px, 700px; size 600px x 40px)
    sf::RectangleShape groundRect = CreateStaticRectangle(worldId, 400.0f, 700.0f, 600.0f, 40.0f, sf::Color(50, 200, 100));

    // Create Dynamic Player Body (size 40px x 60px)
    sf::Texture playerTexture("player.png");
    playerTexture.setSmooth(true);
    b2BodyId playerId = CreatePlayer(worldId, 400.0f, 300.0f, 127.0f, 120.0f);
    sf::RectangleShape playerRect = CreatePlayerRender(127.0f, 120.0f, playerTexture);

    // Create a goal to be reached
    b2ShapeId goalShapeId = CreateGoal(worldId, 640.0f, 630.0f, 30.0f, 100.0f);
    sf::RectangleShape goalRect = CreateGoalRender(640.0f, 630.0f, 30.0f, 100.0f, sf::Color::Yellow);

    // Movement tuning values (in meters/second)
    const float moveSpeed = 7.5f;
    const float jumpSpeed = 11.5f;
    bool jumpPressedLastFrame = false;
    bool levelComplete = false;

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Read Keyboard Input
        b2Vec2 currentPos = b2Body_GetPosition(playerId);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) || (currentPos.y * SCALE) > 850.0f)
        {
            b2Body_SetTransform(playerId, {400.0f / SCALE, 300.0f / SCALE}, b2Rot_identity);
            b2Body_SetLinearVelocity(playerId, {0.0f, 0.0f});
            b2Body_SetAwake(playerId, true);
            levelComplete = false;
            goalRect.setFillColor(sf::Color::Yellow);
        }

        b2Vec2 vel = b2Body_GetLinearVelocity(playerId);
        float targetVx = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            targetVx -= moveSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            targetVx += moveSpeed;
        }
        vel.x = targetVx;

        // Jump when Space/W/Up is newly pressed and vertical speed is near zero
        bool jumpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
                           sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
                           sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);

        if (jumpPressed && !jumpPressedLastFrame && std::abs(vel.y) < 0.05f)
        {
            vel.y = -jumpSpeed; // Negative Y moves upward in SFML coordinates
        }
        jumpPressedLastFrame = jumpPressed;

        b2Body_SetLinearVelocity(playerId, vel);

        // Step Physics World (60 Hz)
        b2World_Step(worldId, 1.0f / 60.0f, 4);

        b2SensorEvents sensorEvents = b2World_GetSensorEvents(worldId);
        for (int i = 0; i != sensorEvents.beginCount; ++i)
        {
            const b2SensorBeginTouchEvent& touch = sensorEvents.beginEvents[i];
            b2BodyId visitorBodyId = b2Shape_GetBody(touch.visitorShapeId);

            if (B2_ID_EQUALS(touch.sensorShapeId, goalShapeId) &&
                B2_ID_EQUALS(visitorBodyId, playerId))
            {
                levelComplete = true;
                goalRect.setFillColor(sf::Color(100, 220, 255));
                std::cout << "Goal Reached!" << std::endl;
            }
        }

        // Sync SFML Player Position to Box2D Body
        b2Vec2 pos = b2Body_GetPosition(playerId);
        playerRect.setPosition({pos.x * SCALE, pos.y * SCALE});

        // Render
        window.clear(sf::Color::White);
        window.draw(groundRect);
        window.draw(playerRect);
        window.draw(goalRect);
        window.display();
    }

    b2DestroyWorld(worldId);
    return 0;
}
