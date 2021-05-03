#include <iostream>
#include <chrono>
#include <random>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

class Entity {
    protected: volatile long long dummy;

    public: virtual void Update() = 0;
};

class Ball : public Entity {
    virtual void Update() override {
        dummy++;
    }
};

class Brick : public Entity {

    virtual void Update() override {
        dummy--;
    }
};

class Component {
    public: volatile long long dummy;
};

class BallComponent : public Component {
    
};

class BrickComponent : public Component {

};

class BallSystem {
    public: vector<BallComponent> components;

    void Update() {
        for (auto& c : components) {
            c.dummy++;
        }
    }
};

class BrickSystem {
    public: vector<BrickComponent> components;
    
    void Update() {
        for (auto& c : components) {
            c.dummy--;
        }
    }
};

bool chooseRandom() {
    return rand() % 2; // Terrible stuff, I know ;D
}

int main(const char** argc, int argv) {
    cout << "Starting ECS comparison test" << endl;

    const int entityNumber = 10'000'000;

    vector<Entity*> entities;

    entities.resize(entityNumber);

    for (int i = 0; i < entityNumber; i++) {
        if (chooseRandom()) {
            entities[i] = new Ball();
        } else {
            entities[i] = new Brick();
        }
    }

    auto start = std::chrono::system_clock::now();
    
    for (int i = 0; i < entityNumber; i++) {
        entities[i]->Update();
    }

    auto end = std::chrono::system_clock::now();
    
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Standard ECS took " << elapsed.count() << "s to update.\n";

    for (int i = 0; i < entityNumber; i++) {
        delete entities[i];
    }

    BallSystem ballSystem;
    BrickSystem brickSystem;

    brickSystem.components.resize(entityNumber / 2);
    ballSystem.components.resize(entityNumber / 2);

    start = std::chrono::system_clock::now();

    ballSystem.Update();
    brickSystem.Update();

    end = std::chrono::system_clock::now();

    elapsed = end - start;

    std::cout << "Systemic ECS took " << elapsed.count() << "s to update.\n";

    return 0;
}