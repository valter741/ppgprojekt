#include "object.h"
#include "scene.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

class Particle: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 speed;


    Particle(glm::vec3 pos, glm::vec3 sp){

        position = pos;
        rotation = glm::vec3 (0, 0,0);
        speed = sp;
        scale *= 0.3f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("mywotah.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");

        timer = 0;


    }

    bool update(Scene &scene, float dt) override{
        this->timer += dt;
        float roundtime = std::round(timer*10);

        position = position+(speed*dt);

        generateModelMatrix();
        if(timer > 2.5f){
            return false;
        }
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor2", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Bubble: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 speed;


    Bubble(glm::vec3 pos, glm::vec3 sp){

        position = pos;
        speed = sp;
        scale *= 0.3f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("bubble.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");

        timer = 0;


    }

    bool update(Scene &scene, float dt) override{

        position += speed;

        generateModelMatrix();
        if(position.y > 10){
            return false;
        }
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor2", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class BubbleGenerator: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 speed;


    BubbleGenerator(glm::vec3 pos){

        position = pos;
        generateModelMatrix();

    }

    bool update(Scene &scene, float dt) override{



        if(glm::linearRand(0.0f, 120.0f) < 1.0f){
            auto bubble = std::make_unique<Bubble>(glm::vec3(this->position.x + glm::linearRand(-0.2f, 0.2f),this->position.y,this->position.z), glm::vec3(0,0.02,0));
            scene.objects.push_back(move(bubble));

        }


        return true;
    }


    void render(Scene &scene) override {

    }
};

class Raindrop: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;


    Raindrop(glm::vec3 pos){

        position = pos;
        scale *= 0.3f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("bubble.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");

        timer = 0;
    }

    bool update(Scene &scene, float dt) override{

        position += scene.gravity*dt;

        position += scene.wind*dt;

        generateModelMatrix();
        if(position.y < -6){
            return false;
        }
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor2", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("MaterialDiff", glm::vec3(.2,.5,.7));

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Rain: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 speed;


    Rain(glm::vec3 pos){

        position = pos;
        generateModelMatrix();

    }

    bool update(Scene &scene, float dt) override{

        if(scene.isRaining){
            auto raind = std::make_unique<Raindrop>(glm::vec3(this->position.x + glm::linearRand(-30.0f, 30.0f),this->position.y,this->position.z + glm::linearRand(-30.0f, 30.0f)));
            scene.objects.push_back(move(raind));
        }
        return true;
    }


    void render(Scene &scene) override {

    }
};

class Bobor: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 posFrames[4] = {
            {-15, 2.5, 15},
            {-11, 2.5, 15},
            {-7, 5, 15},
            {-7, -4, 15}
    };

    glm::vec3 rotFrames[4] = {
            {0, 0, 0},
            {0, 0, 0},
            {0, glm::radians(45.0f), 0},
            {0, -4, glm::radians(-90.0f)}
    };

    float inter = 0.0f;

    int animstage = 0;


    Bobor(){

        position = glm::vec3(-15,2.5,15);
        rotation = glm::vec3 (0, 0,0);
        scale *= 0.2;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("bobor.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("bobor2.obj");

        timer = 0;

    }

    void animate(int p1, int p2, float length, float dt){
        inter += 1.0f*(dt/length);

        if(inter >= 1){
            inter = 0;
            position = posFrames[p2];
            rotation = rotFrames[p2];
            animstage++;
            return;
        }
        rotation = glm::lerp(rotFrames[p1], rotFrames[p2], {inter,inter,inter});
        position = glm::lerp(posFrames[p1], posFrames[p2], {inter,inter,inter});
    }

    bool update(Scene &scene, float dt) override{
        this->timer += dt;
        //float roundtime = std::round(timer*10);

        //std::cout << roundtime << " ";

        if(scene.scenar == 1){
            if(timer > 15 && animstage == 0){
                animate(0, 1, 1.5f, dt);
            }else if(animstage == 1){
                animate(1, 2, 1.5f, dt);
            }else if(animstage == 2){
                animate(2, 3, 1.5f, dt);
            }
        }

        if(scene.scenar == 2){
            if(timer > 15 && this->position.y > -5){
                this->position.y -= 0.02;
            }
        }

        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("Filtr", scene.filtr);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Fish: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    int scared = 0;

    Object* bobor = NULL;


    Fish(){

        position = glm::vec3(glm::linearRand(-4.0f, 4.0f),glm::linearRand(-3.5f, 4.0f),0);
        if(position.x > 0){
            rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),glm::radians(-90.0f));
            speed = glm::linearRand(-2.0f, -0.5f);
        }else{
            rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),glm::radians(90.0f));
            speed = glm::linearRand(0.5f, 2.0f);
        }
        scale *= 0.025f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("fish.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("fish.obj");

    }

    void collision(Scene &scene){
        auto start = scene.objects.begin();
        std::advance(start, 1);
        for(int i = 2; i < 10; i++){
            std::advance(start, 1);
            Object* fish2 = start->get();
            if(this->position != fish2->position){
                //std::cout << this->position.x << " " << fish2->position.x << " ";
                if(abs(this->position.x - fish2->position.x) <= 0.6 && abs(this->position.y - fish2->position.y) <= 0.35){
                    //std::cout<<"collision";
                    if(this->position.x < fish2->position.x)
                        this->position.x -= 0.1;
                    if(this->position.x > fish2->position.x)
                        this->position.x += 0.1;
                    if((this->speed < 0 && fish2->speed < 0) || (this->speed > 0 && fish2->speed > 0)){
                        if(this->speed > fish2->speed){
                            this->speed *= -1;
                            this->rotation.z *= -1;
                        }else{
                            fish2->speed *= -1;
                            fish2->rotation.z *= -1;
                        }
                    }else{
                        this->speed *= -1;
                        fish2->speed *= -1;
                        this->rotation.z *= -1;
                        fish2->rotation.z *= -1;
                    }
                }
            }
        }
    }

    bool animate(Scene &scene, float dt){
        if(this->bobor == NULL){
            auto start = scene.objects.begin();
            std::advance(start, 13);
            bobor = start->get();
        }

        if(scared == 0){
            position.x += speed * dt;

            if(position.x > 4.0){
                position.x = 3.99;
                speed *= -1;
                rotation.z *= -1;
            }
            if(position.x < -4.0){
                position.x = -3.99;
                speed *= -1;
                rotation.z *= -1;
            }

            if(position.y > bobor->position.y){
                scared = 1;
            }

        }
        if(scared == 1){
            position.x += 3 * speed * dt;
            if((position.x > 0 && speed < 0) || (position.x < 0 && speed > 0)){
                speed *= -1;
                rotation.z *= -1;
            }
            if(position.x > 5 || position.x < -5)
                return false;
        }


        return true;
    };

    bool update(Scene &scene, float dt) override{

        collision(scene);

        bool alive = animate(scene, dt);

        generateModelMatrix();
        return alive;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("Filtr", scene.filtr);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Koryto1: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Koryto1(){

        position = glm::vec3(0,0,0);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),0);
        scale *= 2.5f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("grass.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("koryto1.obj");

    }

    bool update(Scene &scene, float dt) override{
        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("Filtr", scene.filtr);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Koryto2: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Koryto2(){

        position = glm::vec3(0,-5,0);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),0);
        scale *= 2.0f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("ground2.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("koryto2.obj");

    }

    bool update(Scene &scene, float dt) override{
        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("Filtr", scene.filtr);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Voda1: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    glm::vec2 textureOffset;

    bool splash = false;

    int animstage = 0;

    Voda1(){

        position = glm::vec3(0,-0.3,0);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),0);
        scale *= 2.5f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("mywotah.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("vodaplane1.obj");



    }

    bool update(Scene &scene, float dt) override{

        textureOffset.y -= dt/15;

        auto start = scene.objects.begin();
        std::advance(start, 1);
        Object* bobor = start->get();
        if(bobor->position.y < this->position.y && !splash && scene.scenar == 1){
            //position = position = glm::vec3(0,5,0);
            splash = true;
            for(int i = 0; i < 10; i++){
                auto particle = std::make_unique<Particle>(bobor->position, glm::vec3(glm::linearRand(-2.0f, 2.0f),4,glm::linearRand(-2.0f, 2.0f)));
                scene.objects.push_back(move(particle));
            }
        }

        if(scene.zaplava){
            timer += dt;
            if(timer > 13){
                if(position.y < 2.5 && animstage == 0){
                    position.y += dt;
                }else if(scale.x < 8 && animstage == 1){
                    scale.x += dt;
                    scale.z += dt;
                }else if(position.y > -0.3 && animstage == 2){
                    position.y -=  dt/3.0f;
                }else if (animstage < 4){
                    animstage++;
                }
            }
        }

        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("Filtr", scene.filtr);

        shader->setUniform("TextureOffset", textureOffset);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class VlnaL: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    glm::vec2 textureOffset;

    int animstage = 0;

    VlnaL(){

        position = glm::vec3(0,-0.3,0);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),0);
        scale *= 2.5f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("mywotah.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("vlnaL2.obj");

    }

    bool update(Scene &scene, float dt) override{

        textureOffset.x -= dt/15;

        if(scene.zaplava){
            timer += dt;
            if(timer > 13){
                if(position.y < 2.5 && animstage == 0){
                    position.y += dt;
                }else if(scale.x < 8 && animstage == 1){
                    scale.x += dt;
                    scale.z += dt;
                }else if(position.y > -0.3 && animstage == 2){
                    position.y -= dt/3.0f;
                }else if (animstage < 4){
                    animstage++;
                }
            }
        }

        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("Filtr", scene.filtr);

        shader->setUniform("TextureOffset", textureOffset);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class VlnaR: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    glm::vec2 textureOffset;

    int animstage = 0;

    VlnaR(){

        position = glm::vec3(0,-0.3,0);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),0);
        scale *= 2.5f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("mywotah.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("vlnaR2.obj");

    }

    bool update(Scene &scene, float dt) override{

        textureOffset.x -= dt/15;

        if(scene.zaplava){
            timer += dt;
            if(timer > 13){
                if(position.y < 2.5 && animstage == 0){
                    position.y += dt;
                }else if(scale.x < 8 && animstage == 1){
                    scale.x += dt/2;
                    scale.z += dt/2;
                }else if(position.y > -0.3 && animstage == 2){
                    position.y -= dt/3.0f;
                }else if (animstage < 4){
                    animstage++;
                }
            }
        }

        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("Filtr", scene.filtr);

        shader->setUniform("TextureOffset", textureOffset);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Tree: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    bool rotLock = false;

    glm::vec3 positions[10] = {
            {-20,2.5,20},
            {-22,2.5,12},
            {-14,2.5,10},
            {-10,2.5,-5},
            {-16,2.5,0},
            {-20,2.5,-7},
            {-16,2.5,-14},
            {-11,2.5,-19},
            {-22,2.5,-21}
    };

    Tree(){

        position = glm::vec3(-20,2.5,20);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),0);
        scale *= 0.015;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("tree.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("treerotated.obj");

    }

    void animate(Scene &scene, float dt){
        if(this->position == positions[2] && scene.zaplava == true){
            if(this->rotation.y > glm::radians(-85.0f))
                this->rotation.y -= dt/7;
        }else {
            if(this->rotation.x < 0.15 && !this->rotLock){
                this->rotation.x += dt/7;
            }else {
                this->rotLock = true;
            }

            if(this->rotation.x > -0.15 && this->rotLock){
                this->rotation.x -= dt/7;
            }else {
                this->rotLock = false;
            }
        }
    }

    bool update(Scene &scene, float dt) override{

        animate(scene, dt);

        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("MaterialDiff", {0.07568,0.61424,0.07568});
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("Filtr", scene.filtr);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Log: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Log(){

        position = glm::vec3(-8,3.5,20);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(180.0f),0);
        scale.x *= 0.28;
        scale.y *= 0.14;
        scale.z *= 0.14;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("log.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("log.obj");

    }

    bool update(Scene &scene, float dt) override{
        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("Filtr", scene.filtr);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Skybox: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Skybox(){

        position = glm::vec3(0,0,0);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),glm::radians(0.0f));
        scale *= 25;
        if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("skyy.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("boxx.obj");

    }

    bool update(Scene &scene, float dt) override{
        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        //glDepthFunc(GL_ALWAYS);
        glFrontFace(GL_CW);
        // Set up light
        /*shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);*/
        shader->setUniform("Filtr", scene.filtr);

        shader->setUniform("LightColor", scene.lightColor);
        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();

        glFrontFace(GL_CCW);
        //glDepthFunc(GL_LEQUAL);

    }
};

class Car: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Car(){

        position = glm::vec3(glm::linearRand(4.0f, 8.0f),2.5,glm::linearRand(-1.0f, 0.0f));
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),glm::radians(180.0f));
        scale *= 3.0f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("car.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("car.obj");

        generateModelMatrix();
    }

    bool update(Scene &scene, float dt) override{

        if(scene.zaplava){
            timer += dt;
            if(timer > 20 && position.x < 30){
                position.x += dt*4;
            }
        }
        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("Filtr", scene.filtr);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Man: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 posFrames[6] = {
            {0,0,0},
            {0,0,8.5},
            {-7,0,8.5},
            {0,0,8.5},
            {0,0,-7},
            {0,0,8.5}
    };

    glm::vec3 rotFrames[6] = {
            {0,0,glm::radians(90.0f)},
            {0,0,glm::radians(0.0f)},
            {0,0,glm::radians(0.0f)},
            {0,0,glm::radians(-90.0f)},
            {0,0,glm::radians(90.0f)},
            {0,0,glm::radians(0.0f)}
    };

    glm::mat4 matFrames[6];

    float timer = 0.0f;
    float zaplavatimer = 0.0f;
    float inter = 0.0f;
    int animstage = 0;

    Man(glm::mat4 car){

        modelMatrix = car;

        //position = glm::vec3(modelMatrix[3][0] + 4, modelMatrix[3][1],glm::linearRand(modelMatrix[3][2]-2.0f, modelMatrix[3][2]+2.0f));
        //rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),glm::radians(90.0f));
        scale *= 0.7f;

        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("asteroid.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("man.obj");

        posFrames[0].z += glm::linearRand(-2.0f, +2.0f);

        for(int i = 0; i<6; i++) {
            glm::vec3 tempframe = posFrames[i] + glm::vec3(car[3][0], car[3][1], car[3][2]);
            tempframe.x += 4;
            matFrames[i] = returnModelMatrix(tempframe, rotFrames[i], scale);
        }

        modelMatrix = matFrames[0];
    }

    void animate(int p1, int p2, float length, float dt){
        inter += 1.0f*(dt/length);

        if(inter >= 1){
            inter = 0;
            modelMatrix = matFrames[p2];
            animstage++;
            if(animstage > 4)
                animstage = 1;
            return;
        }
        modelMatrix = glm::mix(matFrames[p1], matFrames[p2], inter);
    }

    bool update(Scene &scene, float dt) override{
        this->timer += dt;

        auto start = scene.objects.begin();
        std::advance(start, 1);
        Object* car = start->get();

        for(int i = 0; i<6; i++) {
            glm::vec3 tempframe = posFrames[i] + glm::vec3(car->modelMatrix[3][0], car->modelMatrix[3][1], car->modelMatrix[3][2]);
            tempframe.x += 4;
            matFrames[i] = returnModelMatrix(tempframe, rotFrames[i], scale);
        }

        if(timer > 2 && !scene.zaplava){
            animate(animstage, animstage+1, 3.0f, dt);
        }else{
            if(scene.zaplava){
                zaplavatimer += dt;
                if(zaplavatimer > 20 && modelMatrix[3][0] < 30){
                    modelMatrix = glm::translate(modelMatrix, {4*dt,0,0});
                }
            }
        }

        //generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("Filtr", scene.filtr);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Sunhat: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Sunhat(glm::mat4 man){

        modelMatrix = man;

        /*position = glm::vec3(modelMatrix[3][0], modelMatrix[3][1] + 4.2,modelMatrix[3][2]);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),glm::radians(90.0f));
        scale *= 0.2f;*/

        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sunhat.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sunhat.obj");

    }

    bool update(Scene &scene, float dt) override{

        auto start = scene.objects.begin();
        std::advance(start, 2);
        Object* man = start->get();


        modelMatrix = man->modelMatrix;
        modelMatrix = glm::translate(modelMatrix, {0, 5.5f,0});
        modelMatrix = glm::scale(modelMatrix, {.3f, .3f,.3f});

        //generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("Filtr", scene.filtr);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Shadow: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Shadow(glm::mat4 man){

        modelMatrix = man;

        /*position = glm::vec3(modelMatrix[3][0], modelMatrix[3][1] + 4.2,modelMatrix[3][2]);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),glm::radians(90.0f));
        scale *= 0.2f;*/

        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sunhat.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sunhat.obj");

    }

    bool update(Scene &scene, float dt) override{

        auto start = scene.objects.begin();
        std::advance(start, 2);
        Object* man = start->get();


        modelMatrix = man->modelMatrix;
        modelMatrix = glm::translate(modelMatrix, {0, 0.1f,0});
        modelMatrix = glm::scale(modelMatrix, {.2f, .0001f,.2f});

        //generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);
        shader->setUniform("LightColor", scene.lightColor);
        shader->setUniform("LightDirection2", scene.lightDirection2);
        shader->setUniform("LightColor2", scene.lightColor2);
        shader->setUniform("Filtr", scene.filtr);
        shader->setUniform("MaterialDiff", glm::vec3(.01,.01,.01));

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

