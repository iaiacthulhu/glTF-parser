// example
#include "GLTF.h"
#include <stdio.h>
#include <iostream>

int main(int argc, char const* argv[])
{
    GLTF::glTF model("duck.gltf");
    puts("accessors");
    for ( auto it = model.accessors.begin(); it != model.accessors.end(); it++)
    {
        puts(it->first.c_str());
        printf("byteOffset %d, byteStride %d, componentType %d, count %d, type %s\n\n", it->second->byteOffset, it->second->byteStride, it->second->componentType, it->second->count, it->second->type.c_str());

    }
    puts("animations");
    for ( auto it = model.animations.begin(); it != model.animations.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "channels" << std::endl
            ;
        for ( auto jt = it->second->channels.begin(); jt != it->second->channels.end(); jt++)
        {
            std::cout
                << "      " << "sampler : " << jt->sampler << std::endl
                << "      " << "targets" << std::endl
                << "        " << "id : " << jt->targets->id << std::endl
                << "        " << "path : " << jt->targets->path << std::endl
                ;
        }
        puts("    parameters");
        for ( auto jt = it->second->parameters.begin(); jt != it->second->parameters.end(); jt++)
        {
            std::cout << "      " << jt->first << " : " << jt->second << std::endl;
        }
        puts("    samplers");
        for ( auto jt = it->second->samplers.begin(); jt != it->second->samplers.end(); jt++)
        {
            std::cout
                << "      " << jt->first << std::endl
                << "        " << "input : " << jt->second->input << std::endl
                << "        " << "interpolation : " << jt->second->interpolation << std::endl
                << "        " << "output : " << jt->second->output << std::endl
                ;
        }
        std::cout << "    " << "name : " << it->second->name << std::endl;
    }
    
    if ( model.asset )
    {
        std::cout
            << "asset" << std::endl
            << "  " << "copyright : " << model.asset->copyright << std::endl
            << "  " << "generator : " << model.asset->generator << std::endl
            << "  " << "premultipliedAlpha : " << (model.asset->premultipliedAlpha? "true":"false") << std::endl
            << "  " << "profile" << std::endl
            ;
        if ( model.asset->profile )
        {
            std::cout
                << "    " << "api : " << model.asset->profile->api << std::endl
                << "    " << "version : " << model.asset->profile->version << std::endl
                ;
        }
        std::cout
            << "  " << "version : " << model.asset->version << std::endl
            ;
    }

    for ( auto it = model.buffers.begin();     it != model.buffers.end(); it++)
    {
        std::cout
            << it->first << std::endl
            << "  " << it->second->uri << std::endl
            << "  " << it->second->byteLength << std::endl
            << "  " << it->second->type << std::endl
            << "  " << it->second->name << std::endl
            ;
    }
    puts("bufferViews");
    for ( auto it = model.bufferViews.begin(); it != model.bufferViews.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "buffer : " << it->second->buffer << std::endl
            << "    " << "byteOffset : " << it->second->byteOffset << std::endl
            << "    " << "byteLength : " << it->second->byteLength << std::endl
            << "    " << "target : " << it->second->target << std::endl
            << "    " << "name : " << it->second->name << std::endl
            ;

    }
    puts("cameras");
    for ( auto it = model.cameras.begin();     it != model.cameras.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "type : " << it->second->type << std::endl
            << "    " << "name : " << it->second->name << std::endl
            << "    " << "projection : " << it->second->projection << std::endl
            ;
        if ( it->second->orthographic )
        {
            std::cout << "    orthographic" << std::endl
                << "      " << "xmag : " << it->second->orthographic->xmag << std::endl
                << "      " << "ymag : " << it->second->orthographic->ymag << std::endl
                << "      " << "zfar : " << it->second->orthographic->zfar << std::endl
                << "      " << "znear : " << it->second->orthographic->znear << std::endl
                ;
        }
        if ( it->second->perspective )
        {
            std::cout << "    perspective" << std::endl
                << "      " << "aspectRatio : " << it->second->perspective->aspectRatio << std::endl
                << "      " << "yfov : " << it->second->perspective->yfov << std::endl
                << "      " << "zfar : " << it->second->perspective->zfar << std::endl
                << "      " << "znear : " << it->second->perspective->znear << std::endl
                ;
        }
    }
    puts("images");
    for ( auto it = model.images.begin();      it != model.images.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "uri : " << it->second->uri << std::endl
            << "    " << "name : " << it->second->name << std::endl
            ;
    }
    puts("materials");
    for ( auto it = model.materials.begin();   it != model.materials.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "technique : " << it->second->technique << std::endl
            << "    " << "name : " << it->second->name << std::endl
            << "    " << "values" << std::endl
            ;
        if ( it->second->values )
        {
            for ( auto jt = it->second->values->data.begin(); jt != it->second->values->data.end(); jt++)
            {
                std::cout
                    << "      " << jt->first << " : " ;
                if ( it->second->values->datatype[jt->first] == "string" )
                    std::cout << (*static_cast<std::string *>(jt->second)) << std::endl;
                else if ( it->second->values->datatype[jt->first] == "boolean" )
                    std::cout << ((*static_cast<bool *>(jt->second))? "true" : "false") << std::endl;
                else if ( it->second->values->datatype[jt->first] == "number" )
                    std::cout << (*static_cast<double *>(jt->second)) << std::endl;
                else if ( it->second->values->datatype[jt->first] == "integer" )
                    std::cout << (*static_cast<int *>(jt->second)) << std::endl;
                else if ( it->second->values->datatype[jt->first] == "object" )
                    std::cout << "error. this property can not contains \"object\" type data" << std::endl;
                else if ( it->second->values->datatype[jt->first] == "array" )
                {
                    std::cout << "[ ";
                    for ( auto kt = (*static_cast<GLTF::array_ *>(jt->second)).data.begin();
                               kt != (*static_cast<GLTF::array_ *>(jt->second)).data.end(); kt++)
                    {
                        if ( (*static_cast<GLTF::array_ *>(jt->second)).datatype[
                                std::distance((*static_cast<GLTF::array_ *>(jt->second)).data.begin(), kt)] == "number" )
                            std::cout << (*static_cast<double *>(*kt)) << ", ";
                   else if ( (*static_cast<GLTF::array_ *>(jt->second)).datatype[
                                std::distance((*static_cast<GLTF::array_ *>(jt->second)).data.begin(), kt)] == "integer" )
                            std::cout << (*static_cast<int *>(*kt)) << ", ";
                   else if ( (*static_cast<GLTF::array_ *>(jt->second)).datatype[
                                std::distance((*static_cast<GLTF::array_ *>(jt->second)).data.begin(), kt)] == "boolean" )
                            std::cout << ((*static_cast<bool *>(*kt)) ? "true" : "false") << ", ";
                   else if ( (*static_cast<GLTF::array_ *>(jt->second)).datatype[
                                std::distance((*static_cast<GLTF::array_ *>(jt->second)).data.begin(), kt)] == "string" )
                            std::cout << (*static_cast<std::string *>(*kt)) << ", ";
                        else
                            std::cout << "error";
                    }
                    std::cout << " ]" << std::endl;
                }
                else if ( it->second->values->datatype[jt->first] == "void *" )
                    std::cout << "null" << std::endl;
            }
        }
    }
    puts("meshes");
    for ( auto it = model.meshes.begin();      it != model.meshes.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "name : " << it->second->name << std::endl
            << "    " << "primitives" << std::endl
            ;
        for ( auto jt = it->second->primitives.begin(); jt != it->second->primitives.end(); jt++)
        {
            std::cout 
                << "      " << "indices : " << jt->indices << std::endl
                << "      " << "material : " << jt->material << std::endl
                << "      " << "attributes" << std::endl
                ;
            for ( auto kt = jt->attributes.begin(); kt != jt->attributes.end(); kt++ )
            {
                std::cout << "        " << kt->first << " : " << kt->second << std::endl;
            }
        }
    }
    puts("nodes");
    for ( auto it = model.nodes.begin();       it != model.nodes.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "camera : " << it->second->camera << std::endl
            << "    " << "skin : " << it->second->skin << std::endl
            << "    " << "jointName : " << it->second->jointName << std::endl
            << "    " << "name : " << it->second->name << std::endl
            << "    " << "rotation [ " << it->second->rotation.x << ", "
                                       << it->second->rotation.y << ", "
                                       << it->second->rotation.z << ", "
                                       << it->second->rotation.w << " ]" << std::endl
            << "    " << "scale [ " << it->second->scale.x << ", "
                                    << it->second->scale.y << ", "
                                    << it->second->scale.z << " ]" << std::endl
            << "    " << "translation [ " << it->second->translation.x << ", "
                                          << it->second->translation.y << ", "
                                          << it->second->translation.z << " ]" << std::endl
            ;
        puts("    matrix");
        for ( unsigned int i = 0; i < 4; i++ )
        {
            std::cout
                << "      " << it->second->matrix[i*4+0] << ", "
                << "\t" << it->second->matrix[i*4+1] << ", "
                << "\t" << it->second->matrix[i*4+2] << ", "
                << "\t" << it->second->matrix[i*4+3] << ", " << std::endl;
        }
        puts("    children");
        for ( auto jt = it->second->children.begin(); jt != it->second->children.end(); jt++ )
        {
            std::cout << "      " << *jt << std::endl;
        }
        puts("    skeletons");
        for ( auto jt = it->second->skeletons.begin(); jt != it->second->skeletons.end(); jt++ )
        {
            std::cout << "      " << *jt << std::endl;
        }
        puts("    meshes");
        for ( auto jt = it->second->meshes.begin(); jt != it->second->meshes.end(); jt++ )
        {
            std::cout << "      " << *jt << std::endl;
        }
    }
    puts("programs");
    for ( auto it = model.programs.begin();    it != model.programs.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "fragmentShader : " << it->second->fragmentShader << std::endl
            << "    " << "vertexShader : " << it->second->vertexShader << std::endl
            << "    " << "name : " << it->second->name << std::endl
            ;
        puts("    attributes");
        for ( auto jt = it->second->attributes.begin(); jt != it->second->attributes.end(); jt++ )
        {
            std::cout << "      " << *jt << std::endl;
        }

    }
    puts("samplers");
    for ( auto it = model.samplers.begin();    it != model.samplers.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "magFilter : " << it->second->magFilter << std::endl
            << "    " << "minFilter : " << it->second->minFilter << std::endl
            << "    " << "wrapS : " << it->second->wrapS << std::endl
            << "    " << "wrapT : " << it->second->wrapT << std::endl
            << "    " << "name : " << it->second->name << std::endl
            ;
    }
    std::cout << "scene : " << model.scene << std::endl;
    puts("scenes");
    for ( auto it = model.scenes.begin();      it != model.scenes.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "name : " << it->second->name << std::endl
            ;
        puts("    nodes");
        for ( auto jt = it->second->nodes.begin(); jt != it->second->nodes.end(); jt++ )
        {
            std::cout << "      " << *jt << std::endl;
        }
    }
    puts("shaders");
    for ( auto it = model.shaders.begin();     it != model.shaders.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "uri : " << it->second->uri << std::endl
            << "    " << "type : " << it->second->type << std::endl
            << "    " << "name : " << it->second->name << std::endl
            ;
    }
    puts("skins");
    for ( auto it = model.skins.begin();       it != model.skins.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "inverseBindMatrices : " << it->second->inverseBindMatrices << std::endl
            << "    " << "name : " << it->second->name << std::endl
            ;
        puts("    matrix");
        for ( unsigned int i = 0; i < 4; i++ )
        {
            std::cout
                << "      " << it->second->bindShapeMatrix[i*4+0] << ", "
                << "      " << it->second->bindShapeMatrix[i*4+1] << ", "
                << "      " << it->second->bindShapeMatrix[i*4+2] << ", "
                << "      " << it->second->bindShapeMatrix[i*4+3] << ", " << std::endl;
        }
        puts("    jointNames");
        for ( auto jt = it->second->jointNames.begin(); jt != it->second->jointNames.end(); jt++ )
        {
            std::cout << "      " << *jt << std::endl;
        }
    }
    puts("techniques");
    for ( auto it = model.techniques.begin();  it != model.techniques.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "program : " << it->second->program << std::endl
            << "    " << "name : " << it->second->name << std::endl
            << "    " << "states" << std::endl
            ;
        if ( it->second->states )
        {
            puts("      enable");
            for ( auto jt = it->second->states->enable.begin(); jt != it->second->states->enable.end(); jt++ )
            {
                std::cout << "        " << *jt << "," << std::endl;
            }
            puts("      functions");
            if ( it->second->states->functions )
            {
            std::cout
                << "        " << "blendColor : [ " << it->second->states->functions->blendColor[0] << ", "
                                                   << it->second->states->functions->blendColor[1] << ", "
                                                   << it->second->states->functions->blendColor[2] << ", "
                                                   << it->second->states->functions->blendColor[3] << " ]" << std::endl
                << "        " << "blendEquationSeparate : [ " << it->second->states->functions->blendEquationSeparate[0] << ", "
                                                              << it->second->states->functions->blendEquationSeparate[1] << " ]" << std::endl
                << "        " << "blendFuncSeparate : [ " << it->second->states->functions->blendFuncSeparate[0] << ", "
                                                          << it->second->states->functions->blendFuncSeparate[1] << ", "
                                                          << it->second->states->functions->blendFuncSeparate[2] << ", "
                                                          << it->second->states->functions->blendFuncSeparate[3] << " ]" << std::endl
                << "        " << "colorMask : [ " << (it->second->states->functions->colorMask[0] ? "true" : "false") << ", "
                                                  << (it->second->states->functions->colorMask[1] ? "true" : "false") << ", "
                                                  << (it->second->states->functions->colorMask[2] ? "true" : "false") << ", "
                                                  << (it->second->states->functions->colorMask[3] ? "true" : "false") << " ]" << std::endl
                << "        " << "cullFace : [ " << it->second->states->functions->cullFace[0] << " ]" << std::endl
                << "        " << "depthFunc : [ " << it->second->states->functions->depthFunc[0] << " ]" << std::endl
                << "        " << "depthMask : [ " << (it->second->states->functions->depthMask[0] ? "true" : "false") << " ]" << std::endl
                << "        " << "depthRange : [ " << it->second->states->functions->depthRange[0] << ", "
                                                   << it->second->states->functions->depthRange[1] << " ]" << std::endl
                << "        " << "frontFace : [ " << it->second->states->functions->frontFace[0] << " ]" << std::endl
                << "        " << "lineWidth : [ " << it->second->states->functions->lineWidth[0] << " ]" << std::endl
                << "        " << "polygonOffset : [ " << it->second->states->functions->polygonOffset[0] << ", "
                                                      << it->second->states->functions->polygonOffset[1] << " ]" << std::endl
                << "        " << "scissor : [ " << it->second->states->functions->scissor[0] << ", "
                                                << it->second->states->functions->scissor[1] << ", "
                                                << it->second->states->functions->scissor[2] << ", "
                                                << it->second->states->functions->scissor[3] << " ]" << std::endl
                ;
            }
        }

        puts("    parameters");
        for ( auto jt = it->second->parameters.begin(); jt != it->second->parameters.end(); jt++ )
        {
            std::cout
                << "      " << jt->first << std::endl
                << "        " << "count : " << jt->second->count << std::endl
                << "        " << "node : " << jt->second->node << std::endl
                << "        " << "type : " << jt->second->type << std::endl
                << "        " << "semantic : " << jt->second->semantic << std::endl
                << "        " << "value : [ ";
            if ( jt->second->value )
            {
                for ( auto kt = jt->second->value->data.begin(); kt != jt->second->value->data.end(); kt++)
                {
                    if ( jt->second->value->datatype[std::distance(jt->second->value->data.begin(), kt)] == "string" )
                        std::cout << (*static_cast<std::string *>(*kt)) << ", ";
                    else if ( jt->second->value->datatype[std::distance(jt->second->value->data.begin(), kt)] == "boolean" )
                        std::cout << ((*static_cast<bool *>(*kt)) ? "true" : "false") << ", ";
                    else if ( jt->second->value->datatype[std::distance(jt->second->value->data.begin(), kt)] == "number" )
                        std::cout << (*static_cast<double *>(*kt)) << ", ";
                    else if ( jt->second->value->datatype[std::distance(jt->second->value->data.begin(), kt)] == "integer" )
                        std::cout << (*static_cast<int *>(*kt)) << ", ";
                    else if ( jt->second->value->datatype[std::distance(jt->second->value->data.begin(), kt)] == "array" )
                        std::cout << "error. this property can not contain multidimensional array";
                    else
                        std::cout << "error";
                }
            }
            std::cout << " ]" << std::endl;
        }
        puts("    attributes");
        for ( auto jt = it->second->attributes.begin(); jt != it->second->attributes.end(); jt++ )
        {
            std::cout << "      " << jt->first << " : " << jt->second << std::endl;
        }
        puts("    uniforms");
        for ( auto jt = it->second->uniforms.begin(); jt != it->second->uniforms.end(); jt++ )
        {
            std::cout << "      " << jt->first << " : " << jt->second << std::endl;
        }

    }
    puts("textures");
    for ( auto it = model.textures.begin();    it != model.textures.end(); it++)
    {
        std::cout
            << "  " << it->first << std::endl
            << "    " << "format : " << it->second->format << std::endl
            << "    " << "internalFormat : " << it->second->internalFormat << std::endl
            << "    " << "sampler : " << it->second->sampler << std::endl
            << "    " << "source : " << it->second->source << std::endl
            << "    " << "target : " << it->second->target << std::endl
            << "    " << "type : " << it->second->type << std::endl
            << "    " << "name : " << it->second->name << std::endl
            ;
    }
    std::cout << *static_cast<std::string *>(model.extensions->data["test"]) << std::endl;
    return 0;
}
