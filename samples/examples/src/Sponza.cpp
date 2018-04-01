#include "SamplesGame.h"
#include "renderer/BGFXRenderer.h"
#include "FirstPersonCamera.h"
#include <imgui/imgui.h>
#include <events/EventManager.h>
#include <core/FileWatcher.h>

/**
 * Sponza scene for rendering experiments.
 * Material reloading for all drawable nodes is performed when
 * detecting any changes in the shader directory (live shaders coding)
 */
class Sponza : public Example
{
    Scene* _scene;
    Font* _font;
    FirstPersonCamera _fpCamera;

    // ambient light color
    Vector3 _uAmbientColor;

    static const int MAX_DIR_LIGHTS = 1;
    Vector4 _uDirLightDirection[MAX_DIR_LIGHTS];
    Vector4 _uDirLightColor[MAX_DIR_LIGHTS];

    static const int MAX_POINT_LIGHTS = 1;
    Vector4 _uPointLightPosition[MAX_POINT_LIGHTS];
    Vector4 _uPointLightColor[MAX_POINT_LIGHTS];

    static const int MAX_SPOT_LIGHTS = 1;
    Vector4 _uSpotLightPosition[MAX_SPOT_LIGHTS];
    Vector4 _uSpotLightDirection[MAX_SPOT_LIGHTS];
    Vector4 _uSpotLightColor[MAX_SPOT_LIGHTS];


    Node* _pointLightNode[1];
    Node* _boxNode;

    Light* _lightPoint[1];

    /*
     * point light attenuation shortcut
    Linear = 4.5 / LightRange
    Quadratic = 75.0 / LightRange^2
    */


public:

    // get methods used for material automatic binding

    const Vector3 getAmbientLightColor() const
    {
        return _uAmbientColor;
    }

    const Vector4* getDirectionalLightColor() const
    {
        return &_uDirLightColor[0];
    }

    const Vector4* getDirectionalLightDirection() const
    {
        return &_uDirLightDirection[0];
    }

    unsigned int getDirectionalLightCount() const
    {
        return MAX_DIR_LIGHTS;
    }



    // Point Lights

    const Vector4* getPointLightColor() const
    {
        return &_uPointLightColor[0];
    }

    const Vector4* getPointLightPosition() const
    {
        return &_uPointLightPosition[0];
    }

    unsigned int getPointLightCount() const
    {
        return MAX_POINT_LIGHTS;
    }



    // Spot Lights

    const Vector4* getSpotLightColor() const
    {
        return &_uSpotLightColor[0];
    }

    const Vector4* getSpotLightPosition() const
    {
        return &_uSpotLightPosition[0];
    }

    const Vector4* getSpotLightDirection() const
    {
        return &_uSpotLightDirection[0];
    }

    unsigned int getSpotLightCount() const
    {
        return MAX_SPOT_LIGHTS;
    }



    //---------------------------------

    Sponza()
        : _font(nullptr)
        , _scene(nullptr)
    {
    }

    void finalize()
    {
        SAFE_RELEASE(_scene);
        SAFE_RELEASE(_font);
    }

    void initialize()
    {
        // Create the font for drawing the framerate.
        _font = Font::create("res/coredata/ui/arial.gpb");

        // Load sponza scene
        _scene = Scene::load("res/data/samples/examples/sponza/sponza.gpb");


        // Create a point light and create a reference icon for the light
        _lightPoint[0] = Light::createPoint(Vector3::one(), 50);
        _lightPoint[0]->setColor(1.0, 1.0, 1.0);
        _lightPoint[0]->setRange(1000);


        _pointLightNode[0] = Node::create("pointLight");
        _pointLightNode[0]->setLight(_lightPoint[0]);
        _scene->addNode(_pointLightNode[0]);








        // Initialise materials for all models
        _scene->visit(this, &Sponza::initializeMaterials);





        Bundle* bundle = Bundle::create("res/data/scenes/box.gpb");
        Mesh* mesh = bundle->loadMesh("box_Mesh");
        Model* model = Model::create(mesh);
        model->setMaterial("res/data/materials/color.material");
        SAFE_RELEASE(mesh);
        SAFE_RELEASE(bundle);
         _pointLightNode[0]->setDrawable(model);
        _pointLightNode[0]->setScale(3.0f);




        // set fps camera
        _fpCamera.initialize(1.0, 100000.0f);
        _fpCamera.setPosition(Vector3(0,10,0));
        _fpCamera.rotate(MATH_DEG_TO_RAD(90.0f), 0.0f);
        _scene->addNode(_fpCamera.getRootNode());
        _scene->setActiveCamera(_fpCamera.getCamera());
        _scene->getActiveCamera()->setAspectRatio(getAspectRatio());

        // Add watcher to shader directory and bind event to be notified on changes
        FileWatcher::Get()->addDirectory("res/coredata/shaders", true);
        EventManager::get()->addListener(GP_EVENT_LISTENER(this, Sponza::onShaderDirectoryEvent), FileWatcherEvent::ID());
    }





#define UNAME(S,I) std::string(S + '[' + std::to_string(I) + ']').c_str()

    void setMaterialLight(Material* material, Light* light, int index)
    {
        if(light->getLightType() == Light::POINT)
        {
            material->getParameter(UNAME("u_pointLightPosition", index))->bindValue(light->getNode(), &Node::getTranslationView);
            material->getParameter(UNAME("u_pointLightColor", index))->bindValue(light, &Light::getColor);
            //material->getParameter(UNAME("u_pointLightAttenuation", index))->bindValue(light, &Light::getAttenuation);
        }
    }



    bool initializeMaterials(Node* node)
    {
        Model* model = dynamic_cast<Model*>(node->getDrawable());
        if (model)
        {
            Material* material = model->getMaterial(0);

            if(material)
            {
                // when computing lighting in view-space
                material->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX);
                // when computing lighting in world-space
                //material->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", RenderState::INVERSE_TRANSPOSE_WORLD_MATRIX);


                material->setParameterAutoBinding("u_viewMatrix", RenderState::VIEW_MATRIX);
                material->setParameterAutoBinding("u_worldMatrix", RenderState::WORLD_MATRIX);
                material->setParameterAutoBinding("u_worldViewMatrix", RenderState::WORLD_VIEW_MATRIX);
                material->setParameterAutoBinding("u_cameraPosition", RenderState::CAMERA_WORLD_POSITION);


                // Bind material uniform to method
                material->getParameter("u_ambientColor")->bindValue(this, &Sponza::getAmbientLightColor);

                // 2 options : uniform can be automatically binded using bindValue and ptr to methods
                // or set manually with setValue in update() or render().
                // First option need to declare methods for that.
                // Second option need to add code inside render or update methods.
                material->getParameter("u_directionalLightDirection")->bindValue(this, &Sponza::getDirectionalLightDirection, &Sponza::getDirectionalLightCount);
                material->getParameter("u_directionalLightColor")->bindValue(this, &Sponza::getDirectionalLightColor, &Sponza::getDirectionalLightCount);



                //material->getParameter("u_pointLightPosition")->bindValue(this, &Sponza::getPointLightPosition, &Sponza::getPointLightCount);
                material->getParameter("u_pointLightPosition[0]")->bindValue(_pointLightNode[0], &Node::getTranslationView);
                material->getParameter("u_pointLightColor")->bindValue(this, &Sponza::getPointLightColor, &Sponza::getPointLightCount);
                //material->getParameter("u_pointLightRange[0]")->bindValue(_lightPoint[0], &Light::getRange);

                // todo: Light need a new method to get attenuation based on range.
                float LightRange = 50;
                Vector3 lightAttenuation;
                lightAttenuation.x = 1;
                lightAttenuation.y = 4.5 / LightRange;
                lightAttenuation.z = 75.0 / pow(LightRange, 2);
                material->getParameter("u_pointLightAttenuation[0]")->setValue(lightAttenuation);




                material->getParameter("u_spotLightPosition")->bindValue(this, &Sponza::getSpotLightPosition, &Sponza::getSpotLightCount);
                material->getParameter("u_spotLightDirection")->bindValue(this, &Sponza::getSpotLightDirection, &Sponza::getSpotLightCount);
                material->getParameter("u_spotLightColor")->bindValue(this, &Sponza::getSpotLightColor, &Sponza::getSpotLightCount);

                /*float LightRange = 50;
                Vector3 lightAttenuation;
                lightAttenuation.x = 1;
                lightAttenuation.y = 4.5 / LightRange;
                lightAttenuation.z = 75.0 / pow(LightRange, 2);*/
                material->getParameter("u_spotLightAttenuation[0]")->setValue(lightAttenuation);



                /*Light* light = _pointLightNode->getLight();
                //material->getParameter("u_pointLightPosition[0]")->bindValue(_pointLightNode, &Sponza::getPointLightPosition, &Sponza::getPointLightCount);
                //material->getParameter("u_pointLightColor")->bindValue(this, &Sponza::getPointLightColor, &Sponza::getPointLightCount);
                material->getParameter("u_pointLightPosition[0]")->bindValue(_pointLightNode, &Node::getTranslationView);
                material->getParameter("u_pointLightColor[0]")->setValue(_uPointLightColor[0]);*/

            }
        }
        return true;
    }

    void onShaderDirectoryEvent(EventDataRef eventData)
    {
        auto watchFileEvent = std::dynamic_pointer_cast<FileWatcherEvent>(eventData);

        // if change on shader directory reload material for all scene nodes
        if(watchFileEvent)
            _scene->visit(this, &Sponza::reloadMaterials);
    }

    bool reloadMaterials(Node* node)
    {
        Model* model = dynamic_cast<Model*>(node->getDrawable());
        if (model)
        {
            Material* material = model->getMaterial(0);
            material->reload();
        }
        return true;
    }

    void update(float elapsedTime)
    {
        // update camera
        _fpCamera.updateCamera(elapsedTime);

        // Create some ImGui controls
        static float ambient[3] = { 0.2f, 0.2f, 0.2f };
        static float direction[3] = { 0.0f, -1.0f, 0.0f };
        static float color[3] = { 0.75f, 0.75f, 0.75f };

        static float pointLightPos[3] = { 0.0f, 1.0f, 0.0f };
        static float pointLightColor[3] = { 0.75f, 0.75f, 0.75f };

        static float spotLightPos[3] = { 0.0f, 1.0f, 0.0f };
        static float spotLightDir[3] = { 0.0f, -1.0f, 0.0f };
        static float spotLightColor[3] = { 0.75f, 0.75f, 0.75f };

        ImGui::SetNextWindowSize(ImVec2(200,200), ImGuiCond_FirstUseEver);
        ImGui::Begin("Light control");
        ImGui::SliderFloat3("Ambient", ambient, 0.0f, 1.0f);

        ImGui::Separator();
        ImGui::SliderFloat3("DirDirection", direction, -100.0f, 100.0f);
        ImGui::SliderFloat3("DirColor", color, 0.0f, 1.0f);

        ImGui::Separator();
        ImGui::SliderFloat3("PointPos", pointLightPos, -100.0f, 100.0f);
        ImGui::SliderFloat3("PointColor", pointLightColor, 0.0f, 1.0f);


        ImGui::Separator();
        ImGui::SliderFloat3("SpotPos", spotLightPos, -100.0f, 100.0f);
        ImGui::SliderFloat3("SpotDir", spotLightDir, -1.0f, 1.0f);
        ImGui::SliderFloat3("SpotColor", spotLightColor, 0.0f, 1.0f);

        ImGui::End();

        // update uniform values
        _uDirLightColor[0].set(color);
        _uAmbientColor.set(ambient);

        // Compute light direction in eye-space before to send it to shader
        Matrix viewMatrix = _scene->getActiveCamera()->getViewMatrix();
        Vector4 lightDirEyeSpace(direction);
        _uDirLightDirection[0] = viewMatrix * lightDirEyeSpace;
        // or in world space computation
        /*Vector4 lightDirEyeSpace(direction);
        _uDirLightDirection[0] = lightDirEyeSpace;*/


         Vector4 lightPosEyeSpace(pointLightPos[0], pointLightPos[1], pointLightPos[2], 1.0);
         _uPointLightPosition[0] = viewMatrix * lightPosEyeSpace;
         _uPointLightColor[0].set(pointLightColor);
         //_pointLightNode->setTranslation(Vector3(pointLightPos[0], pointLightPos[1], pointLightPos[2]));
         _pointLightNode[0]->setTranslation(lightPosEyeSpace.x, lightPosEyeSpace.y, lightPosEyeSpace.z);


         Vector4 spotlightPosEyeSpace(spotLightPos[0], spotLightPos[1], spotLightPos[2], 1.0);
         _uSpotLightPosition[0] = viewMatrix * spotlightPosEyeSpace;
         Vector4 spotlightDirEyeSpace(spotLightDir[0], spotLightDir[1], spotLightDir[2], 0.0);
         _uSpotLightDirection[0] = viewMatrix * spotlightDirEyeSpace;

         //_uSpotLightPosition[0].set(spotLightPos);
         //_uSpotLightDirection[0].set(spotLightDir);
         _uSpotLightColor[0].set(spotLightColor);


        Model* model = (Model*)_pointLightNode[0]->getDrawable();
        model->getMaterial()->getParameter("u_color")->setValue(_uPointLightColor[0]);
    }

    void render(float elapsedTime)
    {
        _scene->visit(this, &Sponza::drawScene);
        drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
    }

    bool drawScene(Node* node)
    {

        Drawable* drawable = node->getDrawable();
        if (drawable)
        {
            // apply manually material uniform
            Model* model = dynamic_cast<Model*>(drawable);
            if (model)
            {
                Material* material = model->getMaterial(0);
                //material->getParameter("u_directionalLightColor")->setValue(getDirectionalLightColor(), MAX_DIR_LIGHTS);
                //material->getParameter("u_directionalLightDirection")->setValue(getDirectionalLightDirection(), MAX_DIR_LIGHTS);


                //material->getParameter("u_directionalLightDirection[0]")->setValue(_uDirLightDirection[0]);//, MAX_DIR_LIGHTS);


            }

            drawable->draw();
        }
        return true;
    }

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
    {
        _fpCamera.touchEvent(evt, x, y, contactIndex);
    }

    void keyEvent(Keyboard::KeyEvent evt, int key)
    {
        _fpCamera.keyEvent(evt, key);
    }

};


#if defined(ADD_SAMPLE)
ADD_SAMPLE("Graphics", "Sponza", Sponza, 4)
#endif
