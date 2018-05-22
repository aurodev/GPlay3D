#include "gplay3d.h"
#include "Example.h"
#include "SamplesGame.h"
#include "FirstPersonCamera.h"
#include <imgui/imgui.h>

using namespace gameplay;



class DeferredRenderer
{
private:

    static const int PASS_GEOMETRY_ID   = 0;
    static const int PASS_LIGHT_ID      = 1;
    static const int PASS_COMBINE_ID    = 2;
    static const int PASS_DEBUG_ID      = 3;

    FrameBuffer* _gBuffer;
    FrameBuffer* _lightBuffer;

    Model* _finalQuad;
    Model* _lightQuad;

    Scene* _scene;


    std::vector<Light*> _dirLights;
    std::vector<Light*> _pointLights;
    std::vector<Light*> _spotLights;

    Material* _matGBuffer;


    Model* _quadModel[4];
    SpriteBatch* _spriteBatch;

public:
    void create(Scene* scene)
    {
        _scene = scene;
        _scene->addRef();


        Game* game = Game::getInstance();

        Rectangle viewRect = game->getViewport();

        // create views

        View viewGeometry;
        viewGeometry.clearColor = 0x111122ff;
        viewGeometry.clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
        viewGeometry.depth = 1.0f;
        viewGeometry.stencil = 0;
        viewGeometry.rectangle = Rectangle(viewRect.width, viewRect.height);
        game->insertView(PASS_GEOMETRY_ID, viewGeometry);

        View viewLight;
        viewLight.clearColor = 0x303030ff;
        viewLight.clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
        viewLight.depth = 1.0f;
        viewLight.stencil = 0;
        viewLight.rectangle = Rectangle(viewRect.width, viewRect.height);
        game->insertView(PASS_LIGHT_ID, viewLight);

        View viewCombine;
        viewCombine.clearColor = 0x303030ff;
        viewCombine.clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
        viewCombine.depth = 1.0f;
        viewCombine.stencil = 0;
        viewCombine.rectangle = Rectangle(viewRect.width, viewRect.height);
        game->insertView(PASS_COMBINE_ID, viewCombine);




        // create the gbuffer

        std::vector<Texture*> textures;

        {
        // normal buffer
        Texture::TextureInfo texInfo;
        texInfo.id = "NormalBuffer";
        texInfo.width = viewRect.width;
        texInfo.height = viewRect.height;
        texInfo.type = Texture::TEXTURE_RT;
        texInfo.format = Texture::Format::RGBA16F;
        texInfo.flags = BGFX_TEXTURE_RT;
        Texture* tex = Texture::create(texInfo);
        textures.push_back(tex);
        }

        {
        // color + specular buffer
        Texture::TextureInfo texInfo;
        texInfo.id = "AlbedoSpecBuffer";
        texInfo.width = viewRect.width;
        texInfo.height = viewRect.height;
        texInfo.type = Texture::TEXTURE_RT;
        texInfo.format = Texture::Format::RGBA;
        texInfo.flags = BGFX_TEXTURE_RT;
        Texture* tex = Texture::create(texInfo);
        textures.push_back(tex);
        }

        {
        // depth buffer
        Texture::TextureInfo texInfo;
        texInfo.id = "DepthBuffer";
        texInfo.width = viewRect.width;
        texInfo.height = viewRect.height;
        texInfo.type = Texture::TEXTURE_RT;
        texInfo.format = Texture::Format::D24F;
        texInfo.flags = BGFX_TEXTURE_RT;
        Texture* tex = Texture::create(texInfo);
        textures.push_back(tex);
        }

        // create gbuffer mrt
        _gBuffer = FrameBuffer::create("GBuffer", textures);








        _matGBuffer = Material::create("res/core/shaders/gbuffer/gbuffer.vert", "res/core/shaders/gbuffer/gbuffer.frag");
        _matGBuffer->getStateBlock()->setCullFace(true);
        _matGBuffer->getStateBlock()->setDepthTest(true);
        _matGBuffer->getStateBlock()->setDepthWrite(true);
        _matGBuffer->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);
        _matGBuffer->setParameterAutoBinding("u_worldMatrix", RenderState::WORLD_MATRIX);

        Texture::Sampler* sampler = _matGBuffer->getParameter("u_diffuseTexture")->setValue("res/data/textures/grey.png", true);
        sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);

        Texture::Sampler* specSampler = _matGBuffer->getParameter("u_specularTexture")->setValue("res/data/textures/spec.png", true);
        specSampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);














        Material* lightingMaterial = Material::create("res/core/materials/lighting.material");

        lightingMaterial->getStateBlock()->setCullFace(true);
        lightingMaterial->getStateBlock()->setDepthTest(false);
        lightingMaterial->getStateBlock()->setDepthWrite(false);

        lightingMaterial->getParameter("u_viewPos")->bindValue(_scene->getActiveCamera()->getNode(), &Node::getTranslationWorld);
        lightingMaterial->getParameter("u_inverseProjectionMatrix")->bindValue(_scene->getActiveCamera()->getNode(), &Node::getInverseProjectionMatrix);
        lightingMaterial->getParameter("u_inverseViewMatrix")->bindValue(_scene->getActiveCamera()->getNode(), &Node::getInverseViewMatrix);

        Texture::Sampler* sampler2 = Texture::Sampler::create(_gBuffer->getRenderTarget("NormalBuffer"));
        lightingMaterial->getParameter("gNormal")->setValue(sampler2);
        Texture::Sampler* sampler3 = Texture::Sampler::create(_gBuffer->getRenderTarget("AlbedoSpecBuffer"));
        lightingMaterial->getParameter("gAlbedoSpec")->setValue(sampler3);
        Texture::Sampler* sampler4 = Texture::Sampler::create(_gBuffer->getRenderTarget("DepthBuffer"));
        lightingMaterial->getParameter("gDepth")->setValue(sampler4);



        _lightBuffer = FrameBuffer::create("LightBuffer", viewRect.width, viewRect.height, Texture::Format::RGB);


        Mesh* fullScreenQuad = Mesh::createQuadFullscreen();
        _lightQuad = Model::create(fullScreenQuad);
        _lightQuad->setMaterial(lightingMaterial);





        Material* _matCombine;
        _matCombine = Material::create("res/core/shaders/gbuffer/viewport.vert", "res/core/shaders/gbuffer/viewport.frag");
        _matCombine->getStateBlock()->setCullFace(true);
        _matCombine->getStateBlock()->setDepthTest(false); //false
        _matCombine->getStateBlock()->setDepthWrite(false);
        /*_matCombine->getStateBlock()->setBlend(true);
        _matCombine->getStateBlock()->setBlendSrc(RenderState::BLEND_ONE);
        _matCombine->getStateBlock()->setBlendDst(RenderState::BLEND_ONE);*/
        _matCombine->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);
        _matCombine->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX);

        {
        Texture::Sampler* sampler1 = Texture::Sampler::create(_gBuffer->getRenderTarget("AlbedoSpecBuffer"));
        _matCombine->getParameter("s_albedo")->setValue(sampler1);

        Texture::Sampler* sampler2 = Texture::Sampler::create(_lightBuffer->getRenderTarget(0));
        _matCombine->getParameter("s_light")->setValue(sampler2);
        }






        //Mesh* meshQuad = Mesh::createQuad(-1,-1,2,2);
        Mesh* meshQuad = Mesh::createQuadFullscreen();

        _finalQuad = Model::create(meshQuad);
        _finalQuad->setMaterial(_matCombine);
        //Texture::Sampler* sampler = Texture::Sampler::create(_gBuffer->getRenderTarget(i));
        //_quadModel[i]->getMaterial()->getParameter("u_texture")->setValue(sampler);
        SAFE_RELEASE(meshQuad);






        // Create quads for gbuffer preview
        for(int i=0; i<3; i++)
        {
            Mesh* meshQuad = Mesh::createQuad(-1 + i*0.5, -1, 0.5 ,0.5);
            _quadModel[i] = Model::create(meshQuad);
            _quadModel[i]->setMaterial("res/core/shaders/debug/texture.vert", "res/core/shaders/debug/texture.frag");
            Texture::Sampler* sampler = Texture::Sampler::create(_gBuffer->getRenderTarget(i));
            _quadModel[i]->getMaterial()->getParameter("s_texture")->setValue(sampler);
            SAFE_RELEASE(meshQuad);
        }

        _spriteBatch = SpriteBatch::create("res/data/textures/logo.png");




        // analyse scene and add lights to the light manager
        _scene->visit(this, &DeferredRenderer::initializeScene);
    }


    bool initializeScene(Node* node)
    {
        Light* light = dynamic_cast<Light*>(node->getLight());
        if (light)
        {
            switch(light->getLightType())
            {
            case Light::POINT:
                _pointLights.push_back(light);
                break;
            case Light::DIRECTIONAL:
                _dirLights.push_back(light);
                break;
            case Light::SPOT:
                _spotLights.push_back(light);
                break;
            }
        }


        Drawable* drawable = node->getDrawable();
        if (drawable)
        {
            Model* model = dynamic_cast<Model*>(drawable);
            if(model)
            {
                /*for(unsigned int i=0; i<model->getMeshPartCount(); i++)
                {
                   //Material* material = model->getMaterial(i);
                   //if(material)
                   //    initLightForMaterial(material);

                    model->setMaterial(_matGBuffer->clone());
                }*/

                model->setMaterial(_matGBuffer->clone());
            }


        }


        return true;
    }






    bool drawNode(Node* node)
    {
        Drawable* drawable = node->getDrawable();
        if (drawable)
        {
            drawable->draw();
        }
        return true;
    }


    void draw()
    {


        static int dim = 5;
        static float offset = 1.0f;
        static float pointlightRadius = 5.0f;
        static float pointLightPosition[3] = { 0.0, 3.0f, 0.0f };
        static float pointLightColor[3] = { 1.0f, 1.0f, 1.0f };
        static bool showDebugScissorRect = false;
        static bool useScissor = true;

        ImGui::Begin("Toolbox");
        ImGui::SliderInt("Dim", &dim, 1, 100);
        ImGui::SliderFloat("Offset", &offset, -20.0f, 20.0f);
        ImGui::SliderFloat4("position", pointLightPosition, -500.0f, 500.0f);
        ImGui::SliderFloat3("color", pointLightColor, 0.0f, 10.0f);
        ImGui::SliderFloat("radius", &pointlightRadius, 0.0f, 1000.0f);
        ImGui::Checkbox("show scissor rect", &showDebugScissorRect);
        ImGui::Checkbox("apply scissor", &useScissor);
        ImGui::End();



        Rectangle src(0, 0, 256, 256);
        if(showDebugScissorRect)
        {
            _spriteBatch->start();
        }



        // 1. Geometry pass ---------

        Game::getInstance()->bindView(PASS_GEOMETRY_ID);
        _gBuffer->bind();
        _scene->visit(this, &DeferredRenderer::drawNode);


        // 2. Lighting pass ---------

        Game::getInstance()->bindView(PASS_LIGHT_ID);
        _lightBuffer->bind();

        if(_pointLights.size() > 0)
        {
            // activate PointLight technique
            _lightQuad->getMaterial()->setTechnique("PointLight");

            for(int i=0; i<_pointLights.size(); ++i)
            {
                Light* pointLight = _pointLights[i];
                const BoundingSphere& boundingSphere = pointLight->getNode()->getBoundingSphere();

                // perform frustum culling with light bounds
                if(boundingSphere.intersects(_scene->getActiveCamera()->getFrustum()))
                {
                    BoundingBox bbox;
                    bbox.set(boundingSphere);

                    // get 2D rectangle region of light influence
                    Rectangle region = getScissorRegion(bbox);

                    bgfx::setScissor(region.x, region.y, region.width, region.height);

                    // show scissor region for debug
                    if(showDebugScissorRect)
                        _spriteBatch->draw(region, src, Vector4::fromColor(0xffffffff));


                    _lightQuad->getMaterial()->getParameter("u_lightPos")->setValue(pointLight->getNode()->getTranslation());
                    _lightQuad->getMaterial()->getParameter("u_lightColor")->setValue(pointLight->getColor());
                    _lightQuad->getMaterial()->getParameter("u_lightRadius")->setValue(pointLight->getRange());

                    _lightQuad->draw();
                }
            }
        }

        if(_dirLights.size() > 0)
        {
            // activate DirectionnalLight technique
            _lightQuad->getMaterial()->setTechnique("DirectionnalLight");

            for(int i=0; i<_dirLights.size(); ++i)
            {
                Light* dirLight = _dirLights[i];

                _lightQuad->getMaterial()->getParameter("u_lightDirection")->setValue(dirLight->getNode()->getForwardVectorWorld());
                _lightQuad->getMaterial()->getParameter("u_lightColor")->setValue(dirLight->getColor());

                _lightQuad->draw();
            }
        }

        /*if(_lightManager.getSpotLightCount() > 0)
        {
            //...
        }*/


        // 3. Final pass, render to viewport, combine light buffer and diffuse

        Game::getInstance()->bindView(PASS_COMBINE_ID);
        _finalQuad->draw();




        // show rect scissor for debug
        if(showDebugScissorRect)
            _spriteBatch->finish();


                // show gbuffer for debug
        for(int i=0; i<3; i++)
            _quadModel[i]->draw();


    }


    Rectangle getScissorRegion(const BoundingBox& bbox)
    {
        Camera* camera = _scene->getActiveCamera();
        Game* game = Game::getInstance();
        Rectangle vp = game->getViewport();

        Vector3 corners[8];
        Vector2 min(FLT_MAX, FLT_MAX);
        Vector2 max(-FLT_MAX, -FLT_MAX);
        bbox.getCorners(corners);
        for (unsigned int i = 0; i < 8; ++i)
        {
            const Vector3& corner = corners[i];
            float x, y;
            camera->project(vp, corner, &x, &y);
            if (x < min.x)
                min.x = x;
            if (y < min.y)
                min.y = y;
            if (x > max.x)
                max.x = x;
            if (y > max.y)
                max.y = y;
        }

        // define final rectangle
        float x = min.x;
        float y = min.y;
        float width = max.x - min.x;
        float height = max.y - min.y;

        // clamp origin to viewport
        if(x < 0.0f)
        {
            width += x;
            x = 0.0f;
        }
        if(y < 0.0f)
        {
            height += y;
            y = 0.0f;
        }

        if(width > vp.width)
            width = vp.width;
        if(height > vp.height)
            height = vp.height;


        return Rectangle(x,y,width,height);
    }





    DeferredRenderer()
    {

    }

    ~DeferredRenderer()
    {
        _scene->release();
    }






};







class DeferredTestSample : public Example
{
    FirstPersonCamera _fpCamera;
    Font* _font;
    Scene* _scene;  
    DeferredRenderer* _deferredRenderer;

public:

    DeferredTestSample()
        : _font(nullptr), _scene(nullptr)
    {
    }

    void finalize()
    {
        SAFE_RELEASE(_font);
        SAFE_RELEASE(_scene);
    }

    void initialize()
    {
        // Create the font for drawing the framerate.
        _font = Font::create("res/coredata/ui/arial.gpb");

        /*// Create a new empty scene.
        _scene = Scene::create();

        // set fps camera
        Vector3 cameraPosition(0, 1, 5);
        _fpCamera.initialize(0.1f, 10000.0f);
        _fpCamera.setPosition(cameraPosition);
        _scene->addNode(_fpCamera.getRootNode());
        _scene->setActiveCamera(_fpCamera.getCamera());
        _scene->getActiveCamera()->setAspectRatio(getAspectRatio());*/


#if 0
        // Load box shape
       /* Bundle* bundle = Bundle::create("res/data/scenes/shapes.gpb");
        //Model* model = Model::create(bundle->loadMesh("Cube_Mesh"));
        //model->setMaterial(material);

        for(int i=0; i<25; i++)
            for(int j=0; j<25; j++)
        {
        Model* model = Model::create(bundle->loadMesh("Cube_Mesh"));
        Node* node = _scene->addNode();
        node->setScale(1.0f);
        node->setDrawable(model);
        node->setTranslation(Vector3(i*3,0,-j*3));
        }*/


        _scene = Scene::load("res/data/samples/examples/sponza/sponza.gpb");

        // set fps camera
        Vector3 cameraPosition(0, 1, 5);
        _fpCamera.initialize(0.1f, 10000.0f);
        _fpCamera.setPosition(cameraPosition);
        _scene->addNode(_fpCamera.getRootNode());
        _scene->setActiveCamera(_fpCamera.getCamera());
        _scene->getActiveCamera()->setAspectRatio(getAspectRatio());

        Node* node = _scene->findNode("Point");
        _scene->removeNode(node);

#else


        // Create a new empty scene.
        _scene = Scene::create();

        // set fps camera
        Vector3 cameraPosition(0, 1, 5);
        _fpCamera.initialize(0.1f, 10000.0f);
        _fpCamera.setPosition(cameraPosition);
        _scene->addNode(_fpCamera.getRootNode());
        _scene->setActiveCamera(_fpCamera.getCamera());
        _scene->getActiveCamera()->setAspectRatio(getAspectRatio());





        // load shapes bundle
        Bundle* bundle = Bundle::create("res/data/scenes/shapes.gpb");

        // create a plane
        Model* modelPlane = Model::create(bundle->loadMesh("Plane_Mesh"));
        Node* nodePlane = Node::create("plane");
        nodePlane->setDrawable(modelPlane);
        nodePlane->setScale(40.0f);
        _scene->addNode(nodePlane);

        // create a teapot
        Model* modelTeapot = Model::create(bundle->loadMesh("Teapot_Mesh"));
        Node* nodeTeapot = Node::create("teapot");
        nodeTeapot->setDrawable(modelTeapot);
        nodeTeapot->setScale(0.5f);
        _scene->addNode(nodeTeapot);

        // create a torus
        Model* modelTorus = Model::create(bundle->loadMesh("Torus_Mesh"));
        Node* nodeTorus = Node::create("torus");
        nodeTorus->setDrawable(modelTorus);
        nodeTorus->setScale(0.5f);
        nodeTorus->setTranslation(-2, 0.5, -2);
        _scene->addNode(nodeTorus);

        // create a torus knot
        Model* modelTorusKnot = Model::create(bundle->loadMesh("TorusKnot_Mesh"));
        Node* nodeTorusKnot = Node::create("torusKnot");
        nodeTorusKnot->setDrawable(modelTorusKnot);
        nodeTorusKnot->setScale(0.25f);
        nodeTorusKnot->setTranslation(2, 0.5, -2);
        _scene->addNode(nodeTorusKnot);

        // create a cone
       Model* modelCone = Model::create(bundle->loadMesh("Cone_Mesh"));
        Node* nodeCone = Node::create("cone");
        nodeCone->setDrawable(modelCone);
        nodeCone->setScale(0.5f);
        nodeCone->setTranslation(2, 0.5, 2);
        _scene->addNode(nodeCone);

        // create suzanne
        Model* modelMonkey = Model::create(bundle->loadMesh("Monkey_Mesh"));
        Node* nodeMonkey = Node::create("monkey");
        nodeMonkey->setDrawable(modelMonkey);
        nodeMonkey->setScale(0.5f);
        nodeMonkey->setTranslation(-2, 0.5, 2);
        _scene->addNode(nodeMonkey);

        SAFE_RELEASE(bundle);

#endif




        /*for(int i=0; i<1; i++)
            for(int j=0; j<0; j++)
        {
            float r = MATH_RANDOM_0_1();
            float g = MATH_RANDOM_0_1();
            float b = MATH_RANDOM_0_1();

            Light* pointLight = Light::createPoint(Vector3(r,g,b), 20.0f);
            Node* pointLightNode = Node::create("pointLight");
            pointLightNode->setLight(pointLight);
            pointLightNode->setTranslation(Vector3(-40+i*15, 5, -40+j*15));
            _scene->addNode(pointLightNode);
            SAFE_RELEASE(pointLight);
        }*/


        Light* pointLight = Light::createPoint(Vector3(1,0,0), 10.0f);
        Node* pointLightNode = Node::create("pointLight");
        pointLightNode->setLight(pointLight);
        pointLightNode->setTranslation(Vector3(2, 3, 3));
        _scene->addNode(pointLightNode);
        SAFE_RELEASE(pointLight);




        /*{
        Light* dirLight = Light::createDirectional(Vector3(0.7, 0.7, 0.7));
        Node* dirLightNode = Node::create("dirLight");
        dirLightNode->setLight(dirLight);
        dirLightNode->setDirection(Vector3(-1,-1,-1));
        _scene->addNode(dirLightNode);
        SAFE_RELEASE(dirLight);
        }*/



        _deferredRenderer = new DeferredRenderer();
        _deferredRenderer->create(_scene);
    }



    void update(float elapsedTime)
    {
        // update camera
        _fpCamera.updateCamera(elapsedTime);
    }



    void render(float elapsedTime)
    {
        _deferredRenderer->draw();

        drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
    }



    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
    {
        _fpCamera.touchEvent(evt, x, y, contactIndex);

        switch (evt)
        {
        case Touch::TOUCH_PRESS:
            if (x < 75 && y < 50)
            {
                // Toggle Vsync if the user touches the top left corner
                setVsync(!isVsync());
            }
            break;
        case Touch::TOUCH_RELEASE:
            break;
        case Touch::TOUCH_MOVE:
            break;
        };
    }

    void keyEvent(Keyboard::KeyEvent evt, int key)
    {
        _fpCamera.keyEvent(evt, key);
    }

};

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Def test", DeferredTestSample, 255);
#endif

