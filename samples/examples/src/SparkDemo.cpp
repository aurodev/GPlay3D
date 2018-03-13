#include "SparkDemo.h"
#include "SamplesGame.h"


#include <spark/SPARK.h>
#include "GpSpark/SparkQuadRenderer.h"
#include "GpSpark/SparkParticleEmitter.h"


const float ZOOM_DEFAULT = 4.0f;

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Spark Demo", SparkDemo, 15);
#endif

static Mesh* createCubeMesh(float size = 1.0f)
{
    float a = size * 0.5f;
    float vertices[] =
    {
        -a, -a,  a,    0.0,  0.0,  1.0,   0.0, 0.0,
         a, -a,  a,    0.0,  0.0,  1.0,   1.0, 0.0,
        -a,  a,  a,    0.0,  0.0,  1.0,   0.0, 1.0,
         a,  a,  a,    0.0,  0.0,  1.0,   1.0, 1.0,
        -a,  a,  a,    0.0,  1.0,  0.0,   0.0, 0.0,
         a,  a,  a,    0.0,  1.0,  0.0,   1.0, 0.0,
        -a,  a, -a,    0.0,  1.0,  0.0,   0.0, 1.0,
         a,  a, -a,    0.0,  1.0,  0.0,   1.0, 1.0,
        -a,  a, -a,    0.0,  0.0, -1.0,   0.0, 0.0,
         a,  a, -a,    0.0,  0.0, -1.0,   1.0, 0.0,
        -a, -a, -a,    0.0,  0.0, -1.0,   0.0, 1.0,
         a, -a, -a,    0.0,  0.0, -1.0,   1.0, 1.0,
        -a, -a, -a,    0.0, -1.0,  0.0,   0.0, 0.0,
         a, -a, -a,    0.0, -1.0,  0.0,   1.0, 0.0,
        -a, -a,  a,    0.0, -1.0,  0.0,   0.0, 1.0,
         a, -a,  a,    0.0, -1.0,  0.0,   1.0, 1.0,
         a, -a,  a,    1.0,  0.0,  0.0,   0.0, 0.0,
         a, -a, -a,    1.0,  0.0,  0.0,   1.0, 0.0,
         a,  a,  a,    1.0,  0.0,  0.0,   0.0, 1.0,
         a,  a, -a,    1.0,  0.0,  0.0,   1.0, 1.0,
        -a, -a, -a,   -1.0,  0.0,  0.0,   0.0, 0.0,
        -a, -a,  a,   -1.0,  0.0,  0.0,   1.0, 0.0,
        -a,  a, -a,   -1.0,  0.0,  0.0,   0.0, 1.0,
        -a,  a,  a,   -1.0,  0.0,  0.0,   1.0, 1.0
    };
    short indices[] =
    {
        0, 1, 2, 2, 1, 3, 4, 5, 6, 6, 5, 7, 8, 9, 10, 10, 9, 11, 12, 13, 14, 14, 13, 15, 16, 17, 18, 18, 17, 19, 20, 21, 22, 22, 21, 23
    };
    unsigned int vertexCount = 24;
    unsigned int indexCount = 36;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setVertexData(vertices, 0, vertexCount);
    MeshPart* meshPart = mesh->addPart(Mesh::TRIANGLES, Mesh::INDEX16, indexCount, false);
    meshPart->setIndexData(indices, 0, indexCount);
    return mesh;
}

SparkDemo::SparkDemo()
    : _font(NULL), _scene(NULL), _cubeNode(NULL)
{
}

void SparkDemo::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");

    // Create a new empty scene.
    _scene = Scene::create();

    // Create the camera.
    Camera* camera = Camera::createPerspective(45.0f, getAspectRatio(), 1.0f, 1000.0f);
    _cameraNode = _scene->addNode("camera");

    // Attach the camera to a node. This determines the position of the camera.
    _cameraNode->setCamera(camera);

    // Make this the active camera of the scene.
    _scene->setActiveCamera(camera);
    SAFE_RELEASE(camera);

    // Move the camera to look at the origin.
    _cameraNode->translate(-3, 2, 10);
    _cameraNode->rotateX(MATH_DEG_TO_RAD(-10.0f));
    _cameraNode->rotateY(MATH_DEG_TO_RAD(-30.0f));







    // Create a white light.
    Light* light = Light::createDirectional(0.75f, 0.75f, 0.75f);
    Node* lightNode = _scene->addNode("light");
    lightNode->setLight(light);
    // Release the light because the node now holds a reference to it.
    SAFE_RELEASE(light);
    lightNode->rotateX(MATH_DEG_TO_RAD(-45.0f));



    // Create the cube mesh and model.
    Mesh* cubeMesh = createCubeMesh();
    Model* cubeModel = Model::create(cubeMesh);
    // Release the mesh because the model now holds a reference to it.
    SAFE_RELEASE(cubeMesh);

    // Create the material for the cube model and assign it to the first mesh part.
    Material* material = cubeModel->setMaterial("res/shaders/textured.vert", "res/shaders/textured.frag", "DIRECTIONAL_LIGHT_COUNT=1");

    // These parameters are normally set in a .material file but this example sets them programmatically.
    // Bind the uniform "u_worldViewProjectionMatrix" to use the WORLD_VIEW_PROJECTION_MATRIX from the scene's active camera and the node that the model belongs to.
    material->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
    material->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX");
    // Set the ambient color of the material.
    material->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));

    // Bind the light's color and direction to the material.
    material->getParameter("u_directionalLightColor[0]")->setValue(lightNode->getLight()->getColor());
    material->getParameter("u_directionalLightDirection[0]")->bindValue(lightNode, &Node::getForwardVectorWorld);

    // Load the texture from file.
    Texture::Sampler* sampler = material->getParameter("u_diffuseTexture")->setValue("res/png/dirt.png", true);
    sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);
    material->getStateBlock()->setCullFace(false);
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthWrite(true);

    _cubeNode = _scene->addNode("cube");
    _cubeNode->setDrawable(cubeModel);
    //_cubeNode->rotateY(MATH_PIOVER4);
    _cubeNode->setScale(Vector3(5.0f, 0.01f, 5.0f));
    SAFE_RELEASE(cubeModel);











    // Create the material for particles
    Material* material2 = Material::create("res/shaders/sprite2.vert", "res/shaders/sprite2.frag");
    //material2->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);
    //material2->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
    //material2->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX");
    //material2->getParameter("u_worldViewProjectionMatrix")->setValue(_worldViewProjectionMatrix);
    material2->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::VIEW_PROJECTION_MATRIX);

    // Load the texture from file.
    Texture::Sampler* sampler2 = material2->getParameter("u_diffuseTexture")->setValue("res/png/f.png", true);
    sampler2->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);
    material2->getStateBlock()->setCullFace(true);
    material2->getStateBlock()->setDepthTest(true);
    material2->getStateBlock()->setDepthWrite(false);
    // blending
    material2->getStateBlock()->setBlend(true);
    material2->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    material2->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);





    // Create fountain particle effect

    SPK::Ref<SPK::System> system_ = SPK::System::create(true);
    system_->setName("Foutain");

    // Renderer
    SPK::Ref<SPK::GP3D::SparkQuadRenderer> renderer = SPK::GP3D::SparkQuadRenderer::create();
    renderer->setTexturingMode(SPK::TEXTURE_MODE_2D);
    renderer->setScale(0.1f,0.1f);
    renderer->setMaterial(material2);
    renderer->setOrientation(SPK::OrientationPreset::CAMERA_PLANE_ALIGNED);

    // Emitter
    SPK::Ref<SPK::SphericEmitter> particleEmitter = SPK::SphericEmitter::create(SPK::Vector3D(0.0f,1.0f,0.0f),0.1f * M_PI, 0.1f * M_PI);
    particleEmitter->setZone(SPK::Point::create(SPK::Vector3D(0.0f,0.015f,0.0f)));
    particleEmitter->setFlow(30);
    particleEmitter->setForce(1.5f,1.5f);

    // Obstacle
    SPK::Ref<SPK::Plane> groundPlane = SPK::Plane::create();
    SPK::Ref<SPK::Obstacle> obstacle = SPK::Obstacle::create(groundPlane,0.9f,1.0f);
    obstacle->setBouncingRatio(0.6f);
    obstacle->setFriction(1.0f);

    // Group
    SPK::Ref<SPK::Group> particleGroup = system_->createGroup(5000);
    particleGroup->addEmitter(particleEmitter);
    particleGroup->addModifier(obstacle);
    particleGroup->setRenderer(renderer);
    particleGroup->addModifier(SPK::Gravity::create(SPK::Vector3D(0.0f,-1.0f,0.0f)));
    particleGroup->setLifeTime(14.2f,14.5f);
    particleGroup->setColorInterpolator(SPK::ColorSimpleInterpolator::create(0xff0000ff,0x0000ffff));
    particleGroup->enableSorting(true);




     myspksystem = SparkParticleEmitter::create(system_, false);

    _particleNode = _scene->addNode("spark");
    _particleNode->setDrawable(myspksystem);
    _particleNode->setTranslationZ(0);
    _particleNode->setScale(Vector3(2.0f, 1.0f, 1.0f));

    material2->setNodeBinding(_particleNode);



}

void SparkDemo::finalize()
{
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_scene);
}



void SparkDemo::update(float elapsedTime)
{
    // Rotate the directional light.
    //_cubeNode->rotateY(elapsedTime * 0.001 * MATH_PI);

    _particleNode->rotateY(elapsedTime * 0.001 * MATH_PI);
    _particleNode->rotateX(elapsedTime * 0.0001 * MATH_PI);

    static float t = 0;
    t += elapsedTime;
    _particleNode->translateX( sin(t * 0.001) * 0.01 );
    _particleNode->translateZ( cos(t * 0.001) * 0.01 );

    myspksystem->update(elapsedTime);
}

void SparkDemo::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0);

    // Visit all the nodes in the scene, drawing the models.
    _scene->visit(this, &SparkDemo::drawScene);

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void SparkDemo::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
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

bool SparkDemo::drawScene(Node* node)
{
    Drawable* drawable = node->getDrawable();
    if (drawable)
        drawable->draw();
    return true;
}
