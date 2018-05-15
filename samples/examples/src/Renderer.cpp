#include "gplay3d.h"
#include "Example.h"
#include "SamplesGame.h"
#include "FirstPersonCamera.h"
#include <imgui/imgui.h>

using namespace gameplay;

class NewRenderer : public Example
{
    FirstPersonCamera _fpCamera;
    Font* _font;
    Scene* _scene;



    FrameBuffer* _gBuffer;
    Material* _matDeferred;
    Material* _matGBuffer;


    Model* _quadModel[4];

    Model* _screenQuad;

public:

    NewRenderer()
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

        // Create a new empty scene.
        _scene = Scene::create();

        // set fps camera
        Vector3 cameraPosition(0, 1, 5);
        _fpCamera.initialize(0.1f, 10000.0f);
        _fpCamera.setPosition(cameraPosition);
        _scene->addNode(_fpCamera.getRootNode());
        _scene->setActiveCamera(_fpCamera.getCamera());
        _scene->getActiveCamera()->setAspectRatio(getAspectRatio());















        _matGBuffer = Material::create("res/coredata/shaders/test.vert", "res/coredata/shaders/test.frag");
        _matGBuffer->getStateBlock()->setCullFace(true);
        _matGBuffer->getStateBlock()->setDepthTest(true);
        _matGBuffer->getStateBlock()->setDepthWrite(true);
        _matGBuffer->getStateBlock()->setDepthWrite(true);
        _matGBuffer->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);
        _matGBuffer->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX);
        _matGBuffer->setParameterAutoBinding("u_worldMatrix", RenderState::WORLD_MATRIX);
        _matGBuffer->setParameterAutoBinding("u_viewProjectionMatrix", RenderState::VIEW_PROJECTION_MATRIX);
        _matGBuffer->setParameterAutoBinding("u_viewMatrix", RenderState::VIEW_MATRIX);
        _matGBuffer->setParameterAutoBinding("u_projectionMatrix", RenderState::PROJECTION_MATRIX);
        _matGBuffer->setParameterAutoBinding("u_worldViewMatrix", RenderState::WORLD_VIEW_MATRIX);


        Texture::Sampler* sampler = _matGBuffer->getParameter("s_diffuse")->setValue("res/data/textures/brick.png", true);
        sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);

        Texture::Sampler* specSampler = _matGBuffer->getParameter("s_specular")->setValue("res/data/textures/spec.png", true);
        specSampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);



#if 0
        // Load box shape
        Bundle* bundle = Bundle::create("res/data/scenes/shapes.gpb");
        //Model* model = Model::create(bundle->loadMesh("Cube_Mesh"));
        //model->setMaterial(material);

        for(int i=0; i<25; i++)
            for(int j=0; j<25; j++)
        {
        Model* model = Model::create(bundle->loadMesh("Cube_Mesh"));
        model->setMaterial(_matGBuffer->clone());
        Node* node = _scene->addNode();
        node->setScale(1.0f);
        node->setDrawable(model);
        node->setTranslation(Vector3(i*3,0,-j*3));
        }

#else




        // load shapes bundle
        Bundle* bundle = Bundle::create("res/data/scenes/shapes.gpb");

        // create a plane
        Model* modelPlane = Model::create(bundle->loadMesh("Plane_Mesh"));
        modelPlane->setMaterial(_matGBuffer->clone());
        Node* nodePlane = Node::create("plane");
        nodePlane->setDrawable(modelPlane);
        nodePlane->setScale(10.0f);
        _scene->addNode(nodePlane);

        // create a teapot
        Model* modelTeapot = Model::create(bundle->loadMesh("Teapot_Mesh"));
        modelTeapot->setMaterial(_matGBuffer->clone());
        Node* nodeTeapot = Node::create("teapot");
        nodeTeapot->setDrawable(modelTeapot);
        nodeTeapot->setScale(0.5f);
        _scene->addNode(nodeTeapot);

        // create a torus
        Model* modelTorus = Model::create(bundle->loadMesh("Torus_Mesh"));
        modelTorus->setMaterial(_matGBuffer->clone());
        Node* nodeTorus = Node::create("torus");
        nodeTorus->setDrawable(modelTorus);
        nodeTorus->setScale(0.5f);
        nodeTorus->setTranslation(-2, 0.5, -2);
        _scene->addNode(nodeTorus);

        // create a torus knot
        Model* modelTorusKnot = Model::create(bundle->loadMesh("TorusKnot_Mesh"));
        modelTorusKnot->setMaterial(_matGBuffer->clone());
        Node* nodeTorusKnot = Node::create("torusKnot");
        nodeTorusKnot->setDrawable(modelTorusKnot);
        nodeTorusKnot->setScale(0.25f);
        nodeTorusKnot->setTranslation(2, 0.5, -2);
        _scene->addNode(nodeTorusKnot);

        // create a cone
       Model* modelCone = Model::create(bundle->loadMesh("Cone_Mesh"));
        modelCone->setMaterial(_matGBuffer->clone());
        Node* nodeCone = Node::create("cone");
        nodeCone->setDrawable(modelCone);
        nodeCone->setScale(0.5f);
        nodeCone->setTranslation(2, 0.5, 2);
        _scene->addNode(nodeCone);

        // create suzanne
        Model* modelMonkey = Model::create(bundle->loadMesh("Monkey_Mesh"));
        modelMonkey->setMaterial(_matGBuffer->clone());
        Node* nodeMonkey = Node::create("monkey");
        nodeMonkey->setDrawable(modelMonkey);
        nodeMonkey->setScale(0.5f);
        nodeMonkey->setTranslation(-2, 0.5, 2);
        _scene->addNode(nodeMonkey);

        SAFE_RELEASE(bundle);

#endif





    const int FRAMEBUFFER_WIDTH = getWidth();
    const int FRAMEBUFFER_HEIGHT = getHeight();

        // create views
        Game * game = Game::getInstance();

        View defaultView;
        defaultView.clearColor = 0x111122ff;
        defaultView.clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
        defaultView.depth = 1.0f;
        defaultView.stencil = 0;
        defaultView.rectangle = Rectangle(game->getWidth(), game->getHeight());
        game->insertView(0, defaultView);

        View secondView;
        secondView.clearColor = 0x303030ff;
        secondView.clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
        secondView.depth = 1.0f;
        secondView.stencil = 0;
        secondView.rectangle = Rectangle(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
        game->insertView(1, secondView);


        std::vector<Texture*> textures;

        {
        // - position color buffer
        Texture::TextureInfo texInfo;
        texInfo.id = "PositionBuffer";
        texInfo.width = FRAMEBUFFER_WIDTH;
        texInfo.height = FRAMEBUFFER_HEIGHT;
        texInfo.type = Texture::TEXTURE_RT;
        texInfo.format = Texture::Format::RGBA16F;
        texInfo.flags = BGFX_TEXTURE_RT;
        Texture* tex = Texture::create(texInfo);
        textures.push_back(tex);
        }

        {
        // - normal color buffer
        Texture::TextureInfo texInfo;
        texInfo.id = "NormalBuffer";
        texInfo.width = FRAMEBUFFER_WIDTH;
        texInfo.height = FRAMEBUFFER_HEIGHT;
        texInfo.type = Texture::TEXTURE_RT;
        texInfo.format = Texture::Format::RGBA16F;
        texInfo.flags = BGFX_TEXTURE_RT;
        Texture* tex = Texture::create(texInfo);
        textures.push_back(tex);
        }

        {
        // - color + specular color buffer
        Texture::TextureInfo texInfo;
        texInfo.id = "AlbedoSpec";
        texInfo.width = FRAMEBUFFER_WIDTH;
        texInfo.height = FRAMEBUFFER_HEIGHT;
        texInfo.type = Texture::TEXTURE_RT;
        texInfo.format = Texture::Format::RGBA;
        texInfo.flags = BGFX_TEXTURE_RT;
        Texture* tex = Texture::create(texInfo);
        textures.push_back(tex);
        }


        {
        // - color + specular color buffer
        Texture::TextureInfo texInfo;
        texInfo.id = "Depth";
        texInfo.width = FRAMEBUFFER_WIDTH;
        texInfo.height = FRAMEBUFFER_HEIGHT;
        texInfo.type = Texture::TEXTURE_RT;
        texInfo.format = Texture::Format::D16;
        texInfo.flags = BGFX_TEXTURE_RT;
        Texture* tex = Texture::create(texInfo);
        textures.push_back(tex);
        }

        // create gbuffer mrt
        _gBuffer = FrameBuffer::create("GBuffer", textures);





        // Create quads for gbuffer preview
        for(int i=0; i<4; i++)
        {
            Mesh* meshQuad = Mesh::createQuad(-1 + i*0.5, -1, 0.5 ,0.5);
            _quadModel[i] = Model::create(meshQuad);
            _quadModel[i]->setMaterial("res/coredata/shaders/debug.vert", "res/coredata/shaders/debug.frag", "SHOW_TEXTURE");
            Texture::Sampler* sampler = Texture::Sampler::create(_gBuffer->getRenderTarget(i));
            _quadModel[i]->getMaterial()->getParameter("u_texture")->setValue(sampler);
            SAFE_RELEASE(meshQuad);
        }










        _matDeferred = Material::create("res/coredata/shaders/deferred.vert", "res/coredata/shaders/deferred.frag");
        _matDeferred->getStateBlock()->setCullFace(false);
        _matDeferred->getStateBlock()->setDepthTest(true);
        _matDeferred->getStateBlock()->setDepthWrite(true);

            // NOTE : this does not works because ther is no node attached for node binding
            _matDeferred->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);
            _matDeferred->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX);

        _matDeferred->getParameter("viewPos")->bindValue(_fpCamera.getRootNode(), &Node::getTranslationWorld);
        _matDeferred->getParameter("u_inverseProjectionMatrix")->bindValue(_fpCamera.getRootNode(), &Node::getInverseProjectionMatrix);
        _matDeferred->getParameter("u_inverseViewMatrix")->bindValue(_fpCamera.getRootNode(), &Node::getInverseViewMatrix);



        Texture::Sampler* sampler1 = Texture::Sampler::create(_gBuffer->getRenderTarget(0));
        _matDeferred->getParameter("gPosition")->setValue(sampler1);
        Texture::Sampler* sampler2 = Texture::Sampler::create(_gBuffer->getRenderTarget(1));
        _matDeferred->getParameter("gNormal")->setValue(sampler2);
        Texture::Sampler* sampler3 = Texture::Sampler::create(_gBuffer->getRenderTarget(2));
        _matDeferred->getParameter("gAlbedoSpec")->setValue(sampler3);
        Texture::Sampler* sampler4 = Texture::Sampler::create(_gBuffer->getRenderTarget(3));
        _matDeferred->getParameter("s_depthBuffer")->setValue(sampler4);






        /*Mesh* fullScreenQuadMesh = Mesh::createQuadFullscreen();
        _screenQuad = Model::create(fullScreenQuadMesh);
        _screenQuad->setMaterial(_matDeferred);
        //Texture::Sampler* sampler = Texture::Sampler::create(_gBuffer->getRenderTarget(i));
        //_quadModel[i]->getMaterial()->getParameter("u_texture")->setValue(sampler);
        SAFE_RELEASE(fullScreenQuadMesh);*/



        Mesh* meshQuad = Mesh::createQuad(-1,-1,2,2);
        //Mesh* meshQuad = Mesh::createQuadFullscreen();

        _screenQuad = Model::create(meshQuad);
        _screenQuad->setMaterial(_matDeferred);
        //Texture::Sampler* sampler = Texture::Sampler::create(_gBuffer->getRenderTarget(i));
        //_quadModel[i]->getMaterial()->getParameter("u_texture")->setValue(sampler);
        SAFE_RELEASE(meshQuad);


    }

    void update(float elapsedTime)
    {
        // show toolbox
        showToolbox();

        // update camera
        _fpCamera.updateCamera(elapsedTime);
    }

    void render(float elapsedTime)
    {
        Game::getInstance()->bindView(1);
        _gBuffer->bind();
        _scene->visit(this, &NewRenderer::drawScene);


        Game::getInstance()->bindView(0);
        //_scene->visit(this, &NewRenderer::drawScene, (void*)0);


        _screenQuad->draw();


        for(int i=0; i<4; i++)
            _quadModel[i]->draw();

        drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
    }

    bool drawScene(Node* node)
    {
        Drawable* drawable = node->getDrawable();
        if (drawable)
        {
            /*Model* model = dynamic_cast<Model*>(drawable);

            if((int*)cookie == (int*)1)
            {
                //->getMaterial(0)->setTechnique("shadowmap");
                model->setMaterial(_matGBuffer);
            }
            else
            {
                //model->getMaterial(0)->setTechnique("forward");
                model->setMaterial(_matDeferred);
            }*/

            /*Model* model = dynamic_cast<Model*>(drawable);
            model->getMaterial()->getParameter("u_worldMatrix")->setValue(node->getWorldMatrix());*/

            drawable->draw();
        }
        return true;
    }

    void showToolbox()
    {
        ImGui::Begin("Toolbox");
        ImGui::End();
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
    ADD_SAMPLE("Graphics", "Renderer", NewRenderer, 255);
#endif

