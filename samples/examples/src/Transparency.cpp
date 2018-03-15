#include "gameplay.h"
#include "Example.h"
#include "SamplesGame.h"
#include "FirstPersonCamera.h"
#include <imgui/imgui.h>

using namespace gameplay;

class Transparency : public Example
{
    FirstPersonCamera _fpCamera;
    Font* _font;
    Scene* _scene;

public:

    Transparency()
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
        _font = Font::create("res/ui/arial.gpb");

        // Create a new empty scene.
        _scene = Scene::create();

        // set fps camera
        Vector3 cameraPosition(0, 1, 5);
        _fpCamera.initialize(0.1f, 10000.0f);
        _fpCamera.setPosition(cameraPosition);
        _scene->addNode(_fpCamera.getRootNode());
        _scene->setActiveCamera(_fpCamera.getCamera());
        _scene->getActiveCamera()->setAspectRatio(getAspectRatio());

        // load box shape
        Bundle* bundle = Bundle::create("res/common/box.gpb");
        Mesh* meshBox = bundle->loadMesh("box_Mesh");
        SAFE_RELEASE(bundle);

        // Create a plane.
        Model* planeModel = Model::create(meshBox);
        planeModel->setMaterial("res/common/box.material");
        Node* planeNode = Node::create("plane");
        planeNode->setDrawable(planeModel);
        planeNode->setScale(8.0f, 0.001f, 8.0f);
        _scene->addNode(planeNode);


        // Create a base colored material with blending
        Material* material = Material::create("res/shaders/colored.vert", "res/shaders/colored.frag");
        material->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
        material->getStateBlock()->setCullFace(false);
        material->getStateBlock()->setDepthTest(true);
        material->getStateBlock()->setDepthWrite(true);
        material->getStateBlock()->setBlend(true);
        material->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
        material->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);




        /*int maxDim = 4;
        float scale = 0.8f;
        float offset = maxDim / 2.0f * 0.8f;

        for(int y=0; y<maxDim; y++)
            for(int z=0; z<maxDim; z++)
                for(int x=0; x<maxDim; x++)
                {
                        Material* materialClone = material->clone();
                        materialClone->getParameter("u_diffuseColor")->setValue(Vector4(x/float(maxDim), y/float(maxDim), 1, 0.45));
                        materialClone->getStateBlock()->setBlend(true);
                        materialClone->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
                        materialClone->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);

                        Model* model = Model::create(meshBox);
                        model->setMaterial(materialClone);
                        Node* node = Node::create("box");
                        node->setDrawable(model);
                        node->setScale(scale);
                        node->setTranslation(-offset+x, maxDim+1.0f-y, -offset+z);
                        _scene->addNode(node);
                }*/





        {
            Material* materialClone = material->clone();
            materialClone->getParameter("u_diffuseColor")->setValue(Vector4(1.0f, 0.0f, 0.0, 0.3f));

            Model* model = Model::create(meshBox);
            model->setMaterial(materialClone);
            Node* node = Node::create("box");
            node->setDrawable(model);
            node->setScale(6.0f, 1.0f, 0.25f);
            node->setTranslation(0, 0.5, 0);
            _scene->addNode(node);
        }

        {
            Material* materialClone = material->clone();
            materialClone->getParameter("u_diffuseColor")->setValue(Vector4(0.0f, 1.0f, 0.0, 0.3f));

            Model* model = Model::create(meshBox);
            model->setMaterial(materialClone);
            Node* node = Node::create("box");
            node->setDrawable(model);
            node->setScale(6.0f, 1.0f, 0.25f);
            node->setTranslation(0, 0.5, 0);
            node->rotate(Vector3(0,1,0), MATH_DEG_TO_RAD(40.0f));
            _scene->addNode(node);
        }

        {
            Material* materialClone = material->clone();
            materialClone->getParameter("u_diffuseColor")->setValue(Vector4(0.0f, 0.0f, 1.0, 0.3f));

            Model* model = Model::create(meshBox);
            model->setMaterial(materialClone);
            Node* node = Node::create("box");
            node->setDrawable(model);
            node->setScale(6.0f, 1.0f, 0.25f);
            node->setTranslation(0, 0.5, 0);
            node->rotate(Vector3(0,1,0), MATH_DEG_TO_RAD(-40.0f));
            _scene->addNode(node);
        }


        SAFE_RELEASE(material);
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
        // Clear the color and depth buffers
        clear(CLEAR_COLOR_DEPTH, 0.1f, 0.1f, 0.2f, 1.0f, 1.0f, 0);

        // Visit all the nodes in the scene, drawing the models.
        _scene->visit(this, &Transparency::drawScene);

        drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
    }

    bool drawScene(Node* node)
    {
        Drawable* drawable = node->getDrawable();
        if (drawable)
            drawable->draw();
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
    ADD_SAMPLE("Graphics", "Transparency", Transparency, 8);
#endif

