#include <chrono>
#include <SSVUtils/SSVUtils.hpp>
#include <SSVStart/SSVStart.hpp>
#include "CESystem/CES.hpp"

using namespace ssvces;

struct CPosition : Component
{
    float x, y;
    CPosition(float mX, float mY) : x{mX}, y{mY} {}
};
struct CVelocity : Component
{
    float x, y;
    CVelocity(float mX, float mY) : x{mX}, y{mY} {}
};
struct CAcceleration : Component
{
    float x, y;
    CAcceleration(float mX, float mY) : x{mX}, y{mY} {}
};
struct CLife : Component
{
    float life;
    CLife(float mLife) : life{mLife} {}
};
struct CSprite : Component
{
    sf::RectangleShape sprite;
    CSprite() : sprite{ssvs::Vec2f(1, 1)} {}
};
struct CColorInhibitor : Component
{
    float life;
    CColorInhibitor(float mLife) : life{mLife} {}
};

struct SMovement : System<SMovement, Req<CPosition, CVelocity, CAcceleration>>
{
    inline void update(FT mFT) { processAll(mFT); }
    inline void process(Entity&, CPosition& cPosition, CVelocity& cVelocity,
        CAcceleration& cAcceleration, FT mFT)
    {
        cVelocity.x += cAcceleration.x * mFT;
        cVelocity.y += cAcceleration.y * mFT;
        cPosition.x += cVelocity.x * mFT;
        cPosition.y += cVelocity.y * mFT;
    }
};

struct SDeath : System<SDeath, Req<CLife>>
{
    inline void update(FT mFT) { processAll(mFT); }
    inline void process(Entity& entity, CLife& cLife, FT mFT)
    {
        cLife.life -= mFT;
        if(cLife.life < 0) entity.destroy();
    }
};

struct SNonColorInhibitor
    : System<SNonColorInhibitor, Req<CSprite>, Not<CColorInhibitor>>
{
    inline void draw() { processAll(); }
    inline void process(Entity&, CSprite& cSprite)
    {
        cSprite.sprite.setFillColor(sf::Color::Red);
    }
};

struct SDraw : System<SDraw, Req<CPosition, CSprite>>
{
    sf::RenderTarget& renderTarget;
    inline SDraw(sf::RenderTarget& mRenderTarget) : renderTarget(mRenderTarget)
    {
    }

    inline void draw() { processAll(); }
    inline void added(Entity&, CPosition&, CSprite&) {}
    inline void removed(Entity&, CPosition&, CSprite&) {}
    inline void process(Entity&, CPosition& cPosition, CSprite& cSprite)
    {
        cSprite.sprite.setPosition(cPosition.x, cPosition.y);
        renderTarget.draw(cSprite.sprite);
    }
};

struct SColorInhibitor : System<SColorInhibitor, Req<CSprite, CColorInhibitor>>
{
    inline void update(FT mFT) { processAll(mFT); }
    inline void draw() { processAll(); }

    inline void process(
        Entity& entity, CSprite&, CColorInhibitor& cColorInhibitor, FT mFT)
    {
        cColorInhibitor.life -= mFT;
        if(cColorInhibitor.life < 0) entity.removeComponent<CColorInhibitor>();
    }
    inline void process(Entity&, CSprite& cSprite, CColorInhibitor&)
    {
        cSprite.sprite.setFillColor(sf::Color::Blue);
    }

    inline void added(Entity&, CSprite& cSprite, CColorInhibitor&)
    {
        cSprite.sprite.scale(2.f, 2.f);
    }

    inline void removed(Entity&, CSprite& cSprite, CColorInhibitor&)
    {
        cSprite.sprite.scale(2.f, 2.f);
    }
};

constexpr int spawnCount{20000};

int main()
{
    using namespace std;
    using namespace ssvu;
    using namespace sf;

    ssvs::GameWindow gameWindow;
    gameWindow.setTitle("component tests");
    gameWindow.setTimer<ssvs::TimerStatic>(0.5f, 0.5f);
    gameWindow.setSize(1024, 768);
    gameWindow.setFullscreen(false);
    gameWindow.setFPSLimited(true);
    gameWindow.setMaxFPS(200);

    Manager manager;
    SMovement sMovement;
    SDeath sDeath;
    SDraw sDraw{gameWindow};
    SColorInhibitor sColorInhibitor;
    SNonColorInhibitor sNonColorInhibitor;

    manager.registerSystem(sMovement);
    manager.registerSystem(sDeath);
    manager.registerSystem(sNonColorInhibitor);
    manager.registerSystem(sDraw);
    manager.registerSystem(sColorInhibitor);

    ssvu::Benchmark::start("Test");
    {
        for(int k = 0; k < 5; ++k)
        {
            for(int i = 0; i < spawnCount; ++i)
            {
                auto e = manager.createEntity();
                e.createComponent<CPosition>(
                    ssvu::getRndI(512 - 100, 512 + 100),
                    ssvu::getRndI(384 - 100, 384 + 100));
                e.createComponent<CVelocity>(
                    ssvu::getRndR(-1.f, 1.f), ssvu::getRndR(-1.f, 1.f));
                e.createComponent<CAcceleration>(
                    ssvu::getRndR(-1.f, 1.f), ssvu::getRndR(-1.f, 1.f));
                e.createComponent<CLife>(1);
            }

            manager.refresh();
            sMovement.update(5);
            sDeath.update(5);

            manager.refresh();
            sMovement.update(5);
            sDeath.update(5);

            manager.refresh();
            sMovement.update(5);
            sDeath.update(5);
        }

        for(int k = 0; k < 5; ++k)
        {
            for(int i = 0; i < spawnCount; ++i)
            {
                auto e = manager.createEntity();
                e.createComponent<CPosition>(
                    ssvu::getRndI(512 - 100, 512 + 100),
                    ssvu::getRndI(384 - 100, 384 + 100));
                e.createComponent<CVelocity>(
                    ssvu::getRndR(-1.f, 1.f), ssvu::getRndR(-1.f, 1.f));
                e.createComponent<CAcceleration>(
                    ssvu::getRndR(-1.f, 1.f), ssvu::getRndR(-1.f, 1.f));
                e.createComponent<CLife>(25);
            }

            manager.refresh();
            sMovement.update(5);
            sDeath.update(5);

            manager.refresh();
            sMovement.update(5);
            sDeath.update(5);

            manager.refresh();
            sMovement.update(5);
            sDeath.update(5);
        }
    }
    ssvu::Benchmark::endLo();

    float counter{99};
    ssvs::GameState gameState;
    gameState.onUpdate += [&, counter](FT mFT) mutable
    {
        counter += mFT;
        if(counter > 100.f)
        {
            counter = 0;
            for(int i = 0; i < spawnCount; ++i)
            {
                auto e = manager.createEntity();
                e.createComponent<CPosition>(
                    ssvu::getRndI(512 - 100, 512 + 100),
                    ssvu::getRndI(384 - 100, 384 + 100));
                e.createComponent<CVelocity>(
                    ssvu::getRndR(-1.f, 1.f), ssvu::getRndR(-1.f, 1.f));
                e.createComponent<CAcceleration>(
                    ssvu::getRndR(-0.5f, 0.5f), ssvu::getRndR(-0.5f, 0.5f));
                e.createComponent<CSprite>();
                e.createComponent<CLife>(ssvu::getRndI(50, 100));
                e.createComponent<CColorInhibitor>(ssvu::getRndI(5, 85));
                e.addGroups(0);
            }
        }

        manager.refresh();
        sMovement.update(mFT);
        sDeath.update(mFT);
        sColorInhibitor.update(mFT);

        gameWindow.setTitle("up: " + toStr(gameWindow.getMsUpdate()) +
                            "\t dw: " + toStr(gameWindow.getMsDraw()) +
                            "\t ent: " + toStr(manager.getEntityCount()) +
                            "\t cmp: " + toStr(manager.getComponentCount()));

        // if(gameWindow.getFPS() < 60) ssvu::lo() << gameWindow.getFPS() <<
        // "\n";
        // ssvu::lo() << manager.getEntityCount(0) << "\n";
    };
    gameState.onDraw += [&]
    {
        sNonColorInhibitor.draw();
        sDraw.draw();
        sColorInhibitor.draw();
    };


    gameWindow.setGameState(gameState);
    gameWindow.run();

    return 0;
}
