#include "../BGFX/BGFXGpuProgram.h"
#include "../FileSystem.h"

#include <brtshaderc/brtshaderc.h>

namespace gameplay {

BGFXGpuProgram::BGFXGpuProgram() :
    _vsh(BGFX_INVALID_HANDLE)
    , _fsh(BGFX_INVALID_HANDLE)
    , _csh(BGFX_INVALID_HANDLE)
    , _program(BGFX_INVALID_HANDLE)
{
}

BGFXGpuProgram::~BGFXGpuProgram()
{
    if(bgfx::isValid(_program))
        bgfx::destroy(_program);
    if(bgfx::isValid(_vsh))
        bgfx::destroy(_vsh);
    if(bgfx::isValid(_fsh))
        bgfx::destroy(_fsh);
    if(bgfx::isValid(_csh))
        bgfx::destroy(_csh);
}

bool BGFXGpuProgram::set(const char* vshPath, const char* fshPath, const char* defines)
{
    // use custom varying def file if exists or default "varying.def.sc"
    std::string basename = FileSystem::getBaseName(vshPath);
    std::string varyingFile = basename + ".io";
    bool useCustomVaryingDef = FileSystem::fileExists(varyingFile.c_str());
    if(!useCustomVaryingDef)
        varyingFile = FileSystem::getDirectoryName(vshPath) + "varying.def.sc";

    // Compile shaders using brtshaderc library
    const bgfx::Memory* memVsh = shaderc::compileShader(shaderc::ST_VERTEX, vshPath, defines, varyingFile.c_str());
    const bgfx::Memory* memFsh = shaderc::compileShader(shaderc::ST_FRAGMENT, fshPath, defines, varyingFile.c_str());

    if(!memVsh)
    {
        GP_WARN("Error while compiling vertex shader %s.", vshPath);
        return false;
    }

    if(!memFsh)
    {
        GP_WARN("Error while compiling fragment shader %s.", fshPath);
        return false;
    }

    // Create shaders.
    _vsh = bgfx::createShader(memVsh);
    _fsh = bgfx::createShader(memFsh);

    // Create bgfx program.
    _program = bgfx::createProgram(_vsh, _fsh, true);

    if(!bgfx::isValid(_program))
    {
        GP_WARN("Error while creating bgfx program with shaders [%s], [%s], [%s].", vshPath, fshPath, defines);
        return false;
    }

    // Query uniforms from shaders.
    getUniformsFromShader(_vsh);
    getUniformsFromShader(_fsh);

    return true;
}

void BGFXGpuProgram::getUniformsFromShader(bgfx::ShaderHandle shaderHandle)
{
    bgfx::UniformHandle uniforms[32];
    uint16_t activeUniforms = bgfx::getShaderUniforms(shaderHandle, &uniforms[0], 32);    

    for (int i = 0; i < activeUniforms; ++i)
    {
        bgfx::UniformInfo info;
        bgfx::getUniformInfo(uniforms[i], info);

        gameplay::UniformInfo uinfo;
        uinfo.name = info.name;
        uinfo.num = info.num;
        switch(info.type)
        {
            case bgfx::UniformType::Int1: // Int, used for samplers only.
                uinfo.type = UniformType::UT_SAMPLER;
                break;
            case bgfx::UniformType::Vec4: // 4 floats vector.
                uinfo.type = UniformType::UT_VECTOR4;
                break;
            case bgfx::UniformType::Mat3: // 3x3 matrix.
                uinfo.type = UniformType::UT_MATRIX3;
                break;
            case bgfx::UniformType::Mat4: // 4x4 matrix.
                uinfo.type = UniformType::UT_MATRIX4;
                break;
            default:
                GP_ERROR("Uniform type is unknown.");
        };

        _uniformsInfo.push_back(uinfo);        
    }
}

void BGFXGpuProgram::bind()
{
}

const bgfx::ProgramHandle BGFXGpuProgram::getProgram() const
{
    return _program;
}

} // end namespace gameplay

