#include "./GLTF.h"

namespace GLTF
{
    GLTF::glTF::glTF(const std::string filename)
    {
        bool parserStart = false;
        FILE * fp = NULL;
        if( (fp = fopen(filename.c_str(), "r")) != NULL )
        {
            char c;
            while ( (c = fgetc(fp)) != EOF )
            {
                if (c == ' ' || c == '\n' || c == '\r')
                {
                    continue;
                }
                else if (c == '{')
                {
                    if (parserStart)
                    {
                        puts("謎の括弧発見");
                        exit(EXIT_FAILURE);
                    }
                    else
                    {
                        parserStart = true;
                    }
                }
                else if (c == '"')
                {
                    ungetc(c, fp);
                    std::string buf = GLTF::common::getString(fp);

                    if ( ( buf == "accessors"   )
                            ||   ( buf == "animations"  )
                            ||   ( buf == "buffers"     )
                            ||   ( buf == "bufferViews" )
                            ||   ( buf == "cameras"     )
                            ||   ( buf == "images"      )
                            ||   ( buf == "materials"   )
                            ||   ( buf == "meshes"      )
                            ||   ( buf == "nodes"       )
                            ||   ( buf == "programs"    )
                            ||   ( buf == "samplers"    )
                            ||   ( buf == "scenes"      )
                            ||   ( buf == "shaders"     )
                            ||   ( buf == "skins"       )
                            ||   ( buf == "techniques"  )
                            ||   ( buf == "textures"    ) )
                    {
                        char c;
                        while ( (c = fgetc(fp)) != '}' )
                        {
                            if ( c == '"' )
                            {
                                ungetc(c, fp);
                                std::string temp = GLTF::common::getString(fp);
                                if ( buf == "accessors"   ) accessors[temp].reset(new accessor_(fp));
                                else if ( buf == "animations"  ) animations[temp].reset(new animation_(fp));
                                else if ( buf == "buffers"     ) buffers[temp].reset(new buffer_(fp));
                                else if ( buf == "bufferViews" ) bufferViews[temp].reset(new bufferView_(fp));
                                else if ( buf == "cameras"     ) cameras[temp].reset(new camera_(fp));
                                else if ( buf == "images"      ) images[temp].reset(new image_(fp));
                                else if ( buf == "materials"   ) materials[temp].reset(new material_(fp));
                                else if ( buf == "meshes"      ) meshes[temp].reset(new mesh_(fp));
                                else if ( buf == "nodes"       ) nodes[temp].reset(new node_(fp));
                                else if ( buf == "programs"    ) programs[temp].reset(new program_(fp));
                                else if ( buf == "samplers"    ) samplers[temp].reset(new sampler_(fp));
                                else if ( buf == "scenes"      ) scenes[temp].reset(new scene_(fp));
                                else if ( buf == "shaders"     ) shaders[temp].reset(new shader_(fp));
                                else if ( buf == "skins"       ) skins[temp].reset(new skin_(fp));
                                else if ( buf == "techniques"  ) techniques[temp].reset(new technique_(fp));
                                else if ( buf == "textures"    ) textures[temp].reset(new texture_(fp));
                            }
                            else continue;
                        }
                    }
                    else if ( buf == "asset" )
                    {
                        asset.reset(new asset_(fp));
                    }
                    else if ( buf == "scene" )
                    {
                        char c;
                        while ( (c = fgetc(fp)) != ',' )
                        {
                            if ( c == '"' )
                            {
                                ungetc(c, fp);
                                this->scene = GLTF::common::getString(fp);
                                break;
                            }
                        }
                    }
                    else if ( buf == "extensionsUsed" )
                    {
                        char c;
                        while ( (c = fgetc(fp)) != ']' )
                        {
                            if ( c == '"' )
                            {
                                ungetc(c, fp);
                                extensionsUsed.push_back(GLTF::common::getString(fp));
                            }
                            else continue;
                        }
                    }
                    else if ( buf == "extensions" )
                    {
                        extensions.reset(new GLTF::object_(fp));
                    }
                    else if ( buf == "extras" )
                    {
                        extras.reset(new GLTF::object_(fp));
                    }
                }
                else if (c == '}')
                {
                    break;
                }
            }
            fclose(fp);
        }
        else
        {
            fprintf(stderr, _("%s :error: failed to fopen(). filename:[%s]\n"), __func__, filename.c_str() );
            return;
        }
    }

    accessor_::accessor_(FILE * fp) : byteStride()
    {
        this->max.reserve(16);
        this->min.reserve(16);

        std::vector<GLTF::parseinfo> a = {
            {"bufferView",      parseinfo::STRING,  &this->bufferView,      "", 0, true},
            {"byteOffset",      parseinfo::INTEGER, &this->byteOffset,      "", 0, true},
            {"byteStride",      parseinfo::INTEGER, &this->byteStride,      "", 0, false},
            {"componentType",   parseinfo::INTEGER, &this->componentType,   "", 0, true},
            {"count",           parseinfo::INTEGER, &this->count,           "", 0, true},
            {"type",            parseinfo::STRING,  &this->type,            "", 0, true},
            {"max",             parseinfo::NUMBER_VECTOR, &this->max,       "", 0, false},
            {"min",             parseinfo::NUMBER_VECTOR, &this->min,       "", 0, false},
            {"name",            parseinfo::STRING,  &this->name,            "", 0, false},
            {"extensions",      parseinfo::OBJECT,  &this->extensions,      "", 0, false},
            {"extras",          parseinfo::OBJECT,  &this->extras,          "", 0, false},
        };
        parser(a, fp);

        if (this->bufferView.size() < 1) puts("err a");//TODO

        if (this->byteStride > 255)puts("err b");//TODO

        if (      (this->componentType >= 5120 && this->componentType <= 5123)
                || this->componentType == 5126 
           );
        else puts("err c");//TODO

        if (this->count < 0)puts("err d");//TODO

        if (       this->type == "SCALAR"
                || this->type == "VEC2"
                || this->type == "VEC3"
                || this->type == "VEC4"
                || this->type == "MAT2"
                || this->type == "MAT3"
                || this->type == "MAT4"
           );
        else puts("unknown type");//TODO

        if (this->max.size() > 16)
        {
            fprintf(stderr, _("%s :error: max.size()が16を超えています。超えた分の要素は無視されます。"), __func__);// TODO 
            this->max.resize(16);
        }
        else this->max.shrink_to_fit();

        if (this->min.size() > 16)
        {
            fprintf(stderr, _("%s :error: min.size()が16を超えています。超えた分の要素は無視されます。"), __func__);// TODO 
            this->min.resize(16);
        }
        else this->min.shrink_to_fit();

        if (max.size() != min.size()) fprintf(stderr, _("%s :error: max.size() != min.size()"), __func__);
#ifdef GLTF_PARSER_DEBUG
        puts("end of accessor --------------------");
#endif
    } // accessor_::accessor_()

    animation_::animation_(FILE * fp)
    {
        std::vector<GLTF::parseinfo> a = {
            {"channels",    parseinfo::SUB_CLASS_VECTOR,     &this->channels,   "animation_::channel_", 0, false},
            {"parameters",  parseinfo::DICTIONARY,           &this->parameters, "",                     0, false},
            {"samplers",    parseinfo::SUB_CLASS_DICTIONARY, &this->samplers,   "animation_::sampler_", 0, false},
            {"name",        parseinfo::STRING,               &this->name,       "",                     0, false},
            {"extensions",  parseinfo::OBJECT,               &this->extensions, "",                     0, false},
            {"extras",      parseinfo::OBJECT,               &this->extras,     "",                     0, false},
        };
        parser(a,fp);
#ifdef GLTF_PARSER_DEBUG
        puts("end of animation --------------------");
#endif
    }
    animation_::channel_::channel_(FILE * fp)
    {
        std::vector<GLTF::parseinfo> a = {
            {"sampler",     parseinfo::STRING,      &this->sampler,     "",                              0, true},
            {"target",      parseinfo::SUB_CLASS,   &this->targets,     "animation_::channel_::target_", 0, true},
            {"extensions",  parseinfo::OBJECT,      &this->extensions,  "",                              0, false},
            {"extras",      parseinfo::OBJECT,      &this->extras,      "",                              0, false},
        };
        parser(a,fp);
        if (this->sampler.size() < 1) puts("err");
        if (!targets) puts("err");

    }
    animation_::channel_::target_::target_(FILE * fp)
    {
        std::vector<GLTF::parseinfo> a = {
            {"id",          parseinfo::STRING, &this->id,         "", 0, true},
            {"path",        parseinfo::STRING, &this->path,       "", 0, true},
            {"extensions",  parseinfo::OBJECT, &this->extensions, "", 0, false},
            {"extras",      parseinfo::OBJECT, &this->extras,     "", 0, false},
        };
        parser(a,fp);
        if (id.size() < 1) puts("err");

        if (this->path == "translation");
        else if (this->path == "rotation");
        else if (this->path == "scale");
        else
        {
            puts("err");
        }

    }
    const char animation_::sampler_::interpolation_default[] = "LINEAR";
    animation_::sampler_::sampler_(FILE * fp) : interpolation(interpolation_default)
    {
        std::vector<GLTF::parseinfo> a = {
            {"input",         parseinfo::STRING, &this->input,          "", 0, true},
            {"interpolation", parseinfo::STRING, &this->interpolation,  "", 0, false},
            {"output",        parseinfo::STRING, &this->output,         "", 0, true},
            {"extensions",    parseinfo::OBJECT, &this->extensions,     "", 0, false},
            {"extras",        parseinfo::OBJECT, &this->extras,         "", 0, false},
        };
        parser(a,fp);
        if (this->input.size() < 1) puts("err");

        if (this->interpolation == "LINEAR");
        else
        {
            puts("err");
        }

        if (this->output.size() < 1) puts("err");

    }

    asset_::asset_(FILE * fp) : premultipliedAlpha(false)
    {
        std::vector<GLTF::parseinfo> a = {
            {"copyright",           parseinfo::STRING,    &this->copyright,          "",                 0, false},
            {"generator",           parseinfo::STRING,    &this->generator,          "",                 0, false},
            {"premultipliedAlpha",  parseinfo::BOOLEAN,   &this->premultipliedAlpha, "",                 0, false},
            {"profile",             parseinfo::SUB_CLASS, &this->profile,            "asset_::profile_", 0, false},
            {"version",             parseinfo::STRING,    &this->version,            "",                 0, true},
            {"extensions",          parseinfo::OBJECT,    &this->extensions,         "",                 0, false},
            {"extras",              parseinfo::OBJECT,    &this->extras,             "",                 0, false},
        };
        parser(a,fp);
        if(this->version.empty()) puts("err");

#ifdef GLTF_PARSER_DEBUG
        puts("end of asset --------------------");
#endif
    }
    const char asset_::profile_::api_default[] = "WebGL";
    const char asset_::profile_::version_default[] = "1.0.3";
    asset_::profile_::profile_(FILE * fp) : api(api_default), version(version_default)
    {
        std::vector<GLTF::parseinfo> a = {
            {"api",         parseinfo::STRING, &this->api,        "", 0, false},
            {"version",     parseinfo::STRING, &this->version,    "", 0, false},
            {"extensions",  parseinfo::OBJECT, &this->extensions, "", 0, false},
            {"extras",      parseinfo::OBJECT, &this->extras,     "", 0, false},
        };
        parser(a,fp);
    }

    const char buffer_::buffer_::type_default[] = "arraybuffer";
    buffer_::buffer_(FILE * fp) : byteLength(), type(type_default)
    {
        std::vector<GLTF::parseinfo> a = {
            {"uri",         parseinfo::STRING,  &this->uri,         "", 0, true},
            {"byteLength",  parseinfo::INTEGER, &this->byteLength,  "", 0, false},
            {"type",        parseinfo::STRING,  &this->type,        "", 0, false},
            {"name",        parseinfo::STRING,  &this->name,        "", 0, false},
            {"extensions",  parseinfo::OBJECT,  &this->extensions,  "", 0, false},
            {"extras",      parseinfo::OBJECT,  &this->extras,      "", 0, false},
        }; 
        parser(a,fp);
        if ( type == "arraybuffer" );
        else if ( type == "text" );
        else
        {
            puts("err");
        }
#ifdef GLTF_PARSER_DEBUG
        puts("end of buffer --------------------");
#endif
    }

    bufferView_::bufferView_(FILE * fp) : byteLength()
    {
        std::vector<GLTF::parseinfo> a = {
            {"buffer",      parseinfo::STRING,  &this->buffer,      "", 0, true},
            {"byteOffset",  parseinfo::INTEGER, &this->byteOffset,  "", 0, true},
            {"byteLength",  parseinfo::INTEGER, &this->byteLength,  "", 0, false},
            {"target",      parseinfo::INTEGER, &this->target,      "", 0, false},
            {"name",        parseinfo::STRING,  &this->name,        "", 0, false},
            {"extensions",  parseinfo::OBJECT,  &this->extensions,  "", 0, false},
            {"extras",      parseinfo::OBJECT,  &this->extras,      "", 0, false},
        }; 
        parser(a,fp);
        if (this->buffer.empty()) puts("err");
        if (this->byteOffset < 0) puts("err");
        if (this->byteLength < 0) puts("err");

        if ( this->target == 34962 || this->target == 34963 );
        else
        {
            puts("err");
        }
#ifdef GLTF_PARSER_DEBUG
        puts("end of bufferview --------------------");
#endif
    }

    camera_::camera_(FILE * fp) : orthographic(nullptr), perspective(nullptr)
    {
        std::vector<GLTF::parseinfo> a = {
            {"orthographic", parseinfo::SUB_CLASS,  &this->orthographic, "camera_::orthographic_", 0, false},
            {"perspective",  parseinfo::SUB_CLASS,  &this->perspective,  "camera_::perspective_",  0, false},
            {"type",         parseinfo::STRING,     &this->type,         "",                       0, true},
            {"name",         parseinfo::STRING,     &this->name,         "",                       0, false},
            {"extensions",   parseinfo::OBJECT,     &this->extensions,   "",                       0, false},
            {"extras",       parseinfo::OBJECT,     &this->extras,       "",                       0, false},
        }; 
        parser(a,fp);
        if (type == "orthographic");
        else if (type == "perspective");
        else
        {
            puts("err");
        }

#ifdef GLTF_PARSER_DEBUG
        puts("end of camera --------------------");
#endif
    }
    camera_::orthographic_::orthographic_(FILE * fp)
    {
        std::vector<GLTF::parseinfo> a = {
            {"xmag",        parseinfo::NUMBER, &this->xmag,       "", 0, true},
            {"ymag",        parseinfo::NUMBER, &this->ymag,       "", 0, true},
            {"zfar",        parseinfo::NUMBER, &this->zfar,       "", 0, true},
            {"znear",       parseinfo::NUMBER, &this->znear,      "", 0, true},
            {"extensions",  parseinfo::OBJECT, &this->extensions, "", 0, false},
            {"extras",      parseinfo::OBJECT, &this->extras,     "", 0, false},
        }; 
        parser(a,fp);
        if (this->zfar <= 0.0)
        {}
        if (this->znear <= 0.0)
        {}
    }
    camera_::perspective_::perspective_(FILE * fp)
    {
        std::vector<GLTF::parseinfo> a = {
            {"aspectRatio", parseinfo::NUMBER, &this->aspectRatio,  "", 0, false},
            {"yfov",        parseinfo::NUMBER, &this->yfov,         "", 0, true},
            {"zfar",        parseinfo::NUMBER, &this->zfar,         "", 0, true},
            {"znear",       parseinfo::NUMBER, &this->znear,        "", 0, true},
            {"extensions",  parseinfo::OBJECT, &this->extensions,   "", 0, false},
            {"extras",      parseinfo::OBJECT, &this->extras,       "", 0, false},
        }; 
        parser(a,fp);

        if (this->aspectRatio <= 0.0)//TODO 未定義時にはキャンバスの値
        {}
        if (this->yfov <= 0.0)
        {}
        if (this->zfar < 0.0)
        {}
        if (this->znear < 0.0)
        {}

    }

    image_::image_(FILE * fp)
    {
        std::vector<GLTF::parseinfo> a = {
            {"uri",         parseinfo::STRING, &this->uri,          "", 0, true},
            {"name",        parseinfo::STRING, &this->name,         "", 0, false},
            {"extensions",  parseinfo::OBJECT, &this->extensions,   "", 0, false},
            {"extras",      parseinfo::OBJECT, &this->extras,       "", 0, false},
        }; 
        parser(a, fp);
#ifdef GLTF_PARSER_DEBUG
        puts("end of image --------------------");
#endif
    }

    material_::material_(FILE * fp)
    {
        std::vector<GLTF::parseinfo> a = {
            {"technique",  parseinfo::STRING, &this->technique,  "", 0, false},
            {"values",     parseinfo::OBJECT, &this->values,     "", 0, false},
            {"name",       parseinfo::STRING, &this->name,       "", 0, false},
            {"extensions", parseinfo::OBJECT, &this->extensions, "", 0, false},
            {"extras",     parseinfo::OBJECT, &this->extras,     "", 0, false},
        }; 
        parser(a, fp);
#ifdef GLTF_PARSER_DEBUG
        puts("end of material --------------------");
#endif
    }

    mesh_::mesh_(FILE * fp)
    {
        std::vector<GLTF::parseinfo> a = {
            {"primitives", parseinfo::SUB_CLASS_VECTOR, &this->primitives,  "mesh_::primitive_", 0, false},
            {"name",       parseinfo::STRING,           &this->name,        "",                  0, false},
            {"extensions", parseinfo::OBJECT,           &this->extensions,  "",                  0, false},
            {"extras",     parseinfo::OBJECT,           &this->extras,      "",                  0, false},
        }; 
        parser(a, fp);
#ifdef GLTF_PARSER_DEBUG
        puts("end of mesh --------------------");
#endif
    }

    mesh_::primitive_::primitive_(FILE * fp) : mode(4)
    {
        std::vector<GLTF::parseinfo> a = {
            {"attributes",  parseinfo::DICTIONARY,  &this->attributes,  "", 0, false},
            {"indices",     parseinfo::STRING,      &this->indices,     "", 0, false},
            {"material",    parseinfo::STRING,      &this->material,    "", 0, true},
            {"mode",        parseinfo::INTEGER,     &this->mode,        "", 0, false},
            {"extensions",  parseinfo::OBJECT,      &this->extensions,  "", 0, false},
            {"extras",      parseinfo::OBJECT,      &this->extras,      "", 0, false},
        }; 
        parser(a, fp);
        if (this->mode < 0 || this->mode > 6)
        {
            puts("err");
        }
    }

    node_::node_(FILE * fp) : matrix(mat4::I), rotation(), translation()
    {
        double rotation_[4]     = {0.0, 0.0, 0.0, 0.1};
        double scale_[3]        = {1.0, 1.0, 1.0};
        double translation_[3]  = {};

        std::vector<GLTF::parseinfo> a = {
            {"camera",      parseinfo::STRING,        &this->camera,      "", 0, false},
            {"children",    parseinfo::STRING_VECTOR, &this->children,    "", 0, false},
            {"skeletons",   parseinfo::STRING_VECTOR, &this->skeletons,   "", 0, false},
            {"skin",        parseinfo::STRING,        &this->skin,        "", 0, false},
            {"jointName",   parseinfo::STRING,        &this->jointName,   "", 0, false},
            {"matrix",      parseinfo::NUMBER_ARRAY,  this->matrix.getDouble(),      "", 16, false},
            {"meshes",      parseinfo::STRING_VECTOR, &this->meshes,      "", 0, false},
            {"rotation",    parseinfo::NUMBER_ARRAY,  &rotation_,         "", 4, false},
            {"scale",       parseinfo::NUMBER_ARRAY,  &scale_,            "", 3, false},
            {"translation", parseinfo::NUMBER_ARRAY,  &translation_,      "", 3, false},
            {"name",        parseinfo::STRING,        &this->name,        "", 0, false},
            {"extensions",  parseinfo::OBJECT,        &this->extensions,  "", 0, false},
            {"extras",      parseinfo::OBJECT,        &this->extras,      "", 0, false},
        }; 
        parser(a, fp);
        this->rotation.x = rotation_[0];
        this->rotation.y = rotation_[1];
        this->rotation.z = rotation_[2];
        this->rotation.w = rotation_[3];
        this->scale.x = scale_[0];
        this->scale.y = scale_[1];
        this->scale.z = scale_[2];
        this->translation.x = translation_[0];
        this->translation.y = translation_[1];
        this->translation.z = translation_[2];
        this->matrix.update();
#ifdef GLTF_PARSER_DEBUG
        puts("end of node --------------------");
#endif
    }

    program_::program_(FILE * fp)
    {
        this->attributes.reserve(255);
        std::vector<GLTF::parseinfo> a = {
            {"attributes",      parseinfo::STRING_VECTOR, &this->attributes,     "", 0, false},
            {"fragmentShader",  parseinfo::STRING,        &this->fragmentShader, "", 0, true},
            {"vertexShader",    parseinfo::STRING,        &this->vertexShader,   "", 0, true},
            {"name",            parseinfo::STRING,        &this->name,           "", 0, false},
            {"extensions",      parseinfo::OBJECT,        &this->extensions,     "", 0, false},
            {"extras",          parseinfo::OBJECT,        &this->extras,         "", 0, false},
        }; 
        parser(a, fp);
        if ( attributes.size() > 256)
        {
            puts("err");
        }
        else
            attributes.shrink_to_fit();
        if ( this->fragmentShader.empty() ) puts("err");
        if ( this->vertexShader.empty() ) puts("err");

#ifdef GLTF_PARSER_DEBUG
        puts("end of program --------------------");
#endif
    }

    sampler_::sampler_(FILE * fp) : magFilter(9729), minFilter(9986), wrapS(10497), wrapT(10497)
    {
        std::vector<GLTF::parseinfo> a = {
            {"magFilter",  parseinfo::INTEGER, &this->magFilter,  "", 0, false},
            {"minFilter",  parseinfo::INTEGER, &this->minFilter,  "", 0, false},
            {"wrapS",      parseinfo::INTEGER, &this->wrapS,      "", 0, false},
            {"wrapT",      parseinfo::INTEGER, &this->wrapT,      "", 0, false},
            {"name",       parseinfo::STRING,  &this->name,       "", 0, false},
            {"extensions", parseinfo::OBJECT,  &this->extensions, "", 0, false},
            {"extras",     parseinfo::OBJECT,  &this->extras,     "", 0, false},
        }; 
        parser(a, fp);
        if (this->magFilter == 9728);
        else if (this->magFilter == 9729);
        else
        {
            puts("err");
        }

        if (this->minFilter == 9728);
        else if (this->minFilter == 9729);
        else if (this->minFilter == 9984);
        else if (this->minFilter == 9985);
        else if (this->minFilter == 9986);
        else if (this->minFilter == 9987);
        else
        {
            puts("err");
        }

        if (this->wrapS == 33071);
        else if (this->wrapS == 33648);
        else if (this->wrapS == 10497);
        else
        {
            puts("err");
        }

        if (this->wrapT == 33071);
        else if (this->wrapT == 33648);
        else if (this->wrapT == 10497);
        else
        {
            puts("err");
        }
#ifdef GLTF_PARSER_DEBUG
        puts("end of sampler --------------------");
#endif
    }

    scene_::scene_(FILE * fp)
    {
        std::vector<GLTF::parseinfo> a = {
            {"nodes",      parseinfo::STRING_VECTOR, &this->nodes,      "", 0, false},
            {"name",       parseinfo::STRING,        &this->name,       "", 0, false},
            {"extensions", parseinfo::OBJECT,        &this->extensions, "", 0, false},
            {"extras",     parseinfo::OBJECT,        &this->extras,     "", 0, false},
        }; 
        parser(a, fp);
#ifdef GLTF_PARSER_DEBUG
        puts("end of scene --------------------");
#endif
    }

    shader_::shader_(FILE * fp)
    {
        std::vector<GLTF::parseinfo> a = {
            {"uri",        parseinfo::STRING,  &this->uri,        "", 0, true},
            {"type",       parseinfo::INTEGER, &this->type,       "", 0, true},
            {"name",       parseinfo::STRING,  &this->name,       "", 0, false},
            {"extensions", parseinfo::OBJECT,  &this->extensions, "", 0, false},
            {"extras",     parseinfo::OBJECT,  &this->extras,     "", 0, false},
        }; 
        parser(a, fp);
        if (uri.empty()) puts("err");
        if (this->type == 35632);
        else if (this->type == 35633);
        else
        {
            puts("err");
        }
#ifdef GLTF_PARSER_DEBUG
        puts("end of shader --------------------");
#endif
    }

    skin_::skin_(FILE * fp) : bindShapeMatrix(mat4::I)
    {
        std::vector<GLTF::parseinfo> a = {
            {"bindShapeMatrix",     parseinfo::NUMBER_ARRAY,  &this->bindShapeMatrix,     "", 16, false},
            {"inverseBindMatrices", parseinfo::STRING,        &this->inverseBindMatrices, "", 0, true},
            {"jointNames",          parseinfo::STRING_VECTOR, &this->jointNames,          "", 0, true},
            {"name",                parseinfo::STRING,        &this->name,                "", 0, false},
            {"extensions",          parseinfo::OBJECT,        &this->extensions,          "", 0, false},
            {"extras",              parseinfo::OBJECT,        &this->extras,              "", 0, false},
        }; 
        parser(a, fp);
        if (this->inverseBindMatrices.empty()) puts("err");
        if (this->jointNames.empty()) puts("err");
#ifdef GLTF_PARSER_DEBUG
        puts("end of skin --------------------");
#endif
    }

    technique_::technique_(FILE * fp)
    {
        std::vector<GLTF::parseinfo> a = {
            {"parameters",  parseinfo::SUB_CLASS_DICTIONARY, &this->parameters, "technique_::parameters_", 0, false},
            {"attributes",  parseinfo::DICTIONARY,           &this->attributes, "",                        0, false},
            {"program",     parseinfo::STRING,               &this->program,    "",                        0, true},
            {"uniforms",    parseinfo::DICTIONARY,           &this->uniforms,   "",                        0, false},
            {"states",      parseinfo::SUB_CLASS,            &this->states,     "technique_::states_",     0, false},
            {"name",        parseinfo::STRING,               &this->name,       "",                        0, false},
            {"extensions",  parseinfo::OBJECT,               &this->extensions, "",                        0, false},
            {"extras",      parseinfo::OBJECT,               &this->extras,     "",                        0, false},
        }; 
        parser(a, fp);
        if (this->program.empty()) puts("err");
#ifdef GLTF_PARSER_DEBUG
        puts("end of technique --------------------");
#endif
    }
    technique_::parameters_::parameters_(FILE * fp) : count(-1)
    {
        std::vector<GLTF::parseinfo> a = {
            {"count",       parseinfo::INTEGER, &this->count,       "", 0, false},
            {"node",        parseinfo::STRING,  &this->node,        "", 0, false},
            {"type",        parseinfo::INTEGER, &this->type,        "", 0, true},
            {"semantic",    parseinfo::STRING,  &this->semantic,    "", 0, false},
            {"value",       parseinfo::ARRAY,   &this->value,       "", 0, false},
            {"extensions",  parseinfo::OBJECT,  &this->extensions,  "", 0, false},
            {"extras",      parseinfo::OBJECT,  &this->extras,      "", 0, false},
        }; 
        parser(a, fp);
#ifdef GLTF_PARSER_DEBUG
        if (this->count < 1)
        {
            puts("technique.parameters.count was not defined.");
        }
#endif

        if ( !(this->node.empty()) && !(this->type == 35676) )
        {
            puts("err");//crtcl
        }

        if (this->type >= 5120 && this->type <= 5126);
        else if (this->type >= 35664 && this->type <= 35676);
        else if (this->type == 35678 );
        else
        {
            puts("err");
        }
        // TODO _{N} の対応
        // XXX  _までチェックして{N}は無視することでお茶を濁す
        if (     this->semantic.empty() );
        else if (this->semantic == "LOCAL");
        else if (this->semantic == "MODEL");
        else if (this->semantic == "VIEW");
        else if (this->semantic == "PROJECTION");
        else if (this->semantic == "MODELVIEW");
        else if (this->semantic == "MODELVIEWPROJECTION");
        else if (this->semantic == "MODELINVERSE");
        else if (this->semantic == "VIEWINVERSE");
        else if (this->semantic == "PROJECTIONINVERSE");
        else if (this->semantic == "MODELVIEWINVERSE");
        else if (this->semantic == "MODELVIEWPROJECTIONINVERSE");
        else if (this->semantic == "MODELINVERSETRANSPOSE");
        else if (this->semantic == "MODELVIEWINVERSETRANSPOSE");
        else if (this->semantic == "VIEWPORT");
        // attributes
        else if (this->semantic == "POSITION");
        else if (this->semantic == "NORMAL");
        else if (this->semantic == "TEXCOORD");
        else if (this->semantic == "COLOR");
        else if (this->semantic == "JOINT");
        else if (this->semantic == "JOINTMATRIX");
        else if (this->semantic == "WEIGHT");
        else if (this->semantic.compare(0, 9,  "POSITION_")    == 0);
        else if (this->semantic.compare(0, 7,  "NORMAL_")      == 0);
        else if (this->semantic.compare(0, 9,  "TEXCOORD_")    == 0);
        else if (this->semantic.compare(0, 6,  "COLOR_")       == 0);
        else if (this->semantic.compare(0, 6,  "JOINT_")       == 0);
        else if (this->semantic.compare(0, 12, "JOINTMATRIX_") == 0); // MAT4のこいつがattributesに居るのはおかしいような・・・
        else if (this->semantic.compare(0, 7,  "WEIGHT_")      == 0);
        else
        {
            fprintf(stderr, _("%s :error: unknown semantic [%s]"), __func__, this->semantic.c_str());
        }
    }
    technique_::states_::states_(FILE * fp)
    {
        std::vector<GLTF::parseinfo> a = {
            {"enable",      parseinfo::INTEGER_VECTOR, &this->enable,     "",                                0, false},
            {"functions",   parseinfo::SUB_CLASS,      &this->functions,  "technique_::states_::functions_", 0, false},
            {"extensions",  parseinfo::OBJECT,         &this->extensions, "",                                0, false},
            {"extras",      parseinfo::OBJECT,         &this->extras,     "",                                0, false},
        }; 
        parser(a, fp);
        std::vector<int>::iterator it;
        for (it = enable.begin(); it != enable.end(); ++it)
        {
            if (*it == 3042);
            else if (*it == 2884);
            else if (*it == 2929);
            else if (*it == 32823);
            else if (*it == 32926);
            else if (*it == 3089);
            else
            {
                puts("err");
            }
        }
    }
    technique_::states_::functions_::functions_(FILE * fp) : blendColor(), blendFuncSeparate(), depthRange(), polygonOffset(), scissor()
    {
        blendEquationSeparate[0] = blendEquationSeparate[1] = 32774;
        blendFuncSeparate[0] = blendFuncSeparate[1] = 1;
        colorMask[0] = colorMask[1] = colorMask[2] = colorMask[3] = true;
        cullFace[0] = 1029;
        depthFunc[0] = 513;
        depthRange[0] = 1;
        depthMask[0] = true;
        frontFace[0] = 2305;
        lineWidth[0] = 1.0;
        std::vector<GLTF::parseinfo> a = {
            {"blendColor",            parseinfo::NUMBER_ARRAY,  &this->blendColor,            "", 4, false},
            {"blendEquationSeparate", parseinfo::INTEGER_ARRAY, &this->blendEquationSeparate, "", 2, false},
            {"blendFuncSeparate",     parseinfo::INTEGER_ARRAY, &this->blendFuncSeparate,     "", 4, false},
            {"colorMask",             parseinfo::BOOLEAN_ARRAY, &this->colorMask,             "", 4, false},
            {"cullFace",              parseinfo::INTEGER_ARRAY, &this->cullFace,              "", 1, false},
            {"depthFunc",             parseinfo::INTEGER_ARRAY, &this->depthFunc,             "", 1, false},
            {"depthMask",             parseinfo::BOOLEAN_ARRAY, &this->depthMask,             "", 1, false},
            {"depthRange",            parseinfo::NUMBER_ARRAY,  &this->depthRange,            "", 2, false},
            {"frontFace",             parseinfo::INTEGER_ARRAY, &this->frontFace,             "", 1, false},
            {"lineWidth",             parseinfo::NUMBER_ARRAY,  &this->lineWidth,             "", 1, false},
            {"polygonOffset",         parseinfo::NUMBER_ARRAY,  &this->polygonOffset,         "", 2, false},
            {"scissor",               parseinfo::NUMBER_ARRAY,  &this->scissor,               "", 4, false},
            {"extensions",            parseinfo::OBJECT,        &this->extensions,            "", 0, false},
            {"extras",                parseinfo::OBJECT,        &this->extras,                "", 0, false},
        }; 
        parser(a, fp);
        for (unsigned int i = 0; i < 2; i++)
        {
            if (this->blendEquationSeparate[i] == 32774);
            else if (this->blendEquationSeparate[i] == 32778);
            else if (this->blendEquationSeparate[i] == 32779);
            else
            {
            }
        }
        for (unsigned int i = 0; i < 4; i++)
        {
            if (this->blendFuncSeparate[i] == 0);
            else if (this->blendFuncSeparate[i] == 1);
            else if (this->blendFuncSeparate[i] >= 768 && this->blendFuncSeparate[i] <= 776);
            else if (this->blendFuncSeparate[i] >= 32769 && this->blendFuncSeparate[i] <= 32772);
            else
            {
            }
        }
        if (this->cullFace[0] == 1028);
        else if (this->cullFace[0] == 1029);
        else if (this->cullFace[0] == 1032);
        else
        {
        }
        if (this->depthFunc[0] >= 512 && this->depthFunc[0] <= 519);
        else
        {
        }
        if (this->frontFace[0] == 2304);
        else if (this->frontFace[0] == 2305);
        else
        {
        }
    }

    texture_::texture_(FILE * fp) : format(6408), internalFormat(6408), target(3553), type(5121)
    {
        std::vector<GLTF::parseinfo> a = {
            {"format",          parseinfo::INTEGER, &this->format,          "", 0, false},
            {"internalFormat",  parseinfo::INTEGER, &this->internalFormat,  "", 0, false},
            {"sampler",         parseinfo::STRING,  &this->sampler,         "", 0, true},
            {"source",          parseinfo::STRING,  &this->source,          "", 0, true},
            {"target",          parseinfo::INTEGER, &this->target,          "", 0, false},
            {"type",            parseinfo::INTEGER, &this->type,            "", 0, false},
            {"name",            parseinfo::STRING,  &this->name,            "", 0, false},
            {"extensions",      parseinfo::OBJECT,  &this->extensions,      "", 0, false},
            {"extras",          parseinfo::OBJECT,  &this->extras,          "", 0, false},
        }; 
        parser(a, fp);
        if (this->format >= 6406 && this->format <= 6410);
        else
        {
            puts("err");
        }

        if (this->internalFormat >= 6406 && this->internalFormat <= 6410);
        else
        {
            puts("err");
        }
        if (this->sampler.empty()) puts("err");
        if (this->source.empty()) puts("err");

        if (this->target == 3553);
        else
        {
            puts("err");
        }

        if (this->type == 5121);
        else if (this->type == 33635);
        else if (this->type == 32819);
        else if (this->type == 32820);
        else
        {
            puts("err");
        }
#ifdef GLTF_PARSER_DEBUG
        puts("end of texture --------------------");
#endif
    }


    object_::object_(FILE * fp)
    {
        char c;
        char * key = NULL;

        while ( (c = fgetc(fp)) != '}' )
        {
            if ( c == '"' )
            {
                ungetc(c, fp);
                if ( key )
                {
                    delete[] key;
                    key = NULL;
                }
                key = GLTF::common::getString(fp);

                char c;
                while ( (c = fgetc(fp)) != EOF )
                {
                    if (       c == '"' //文字列
                            || c == 't' //true
                            || c == 'f' //false
                            || c == 'n' //null
                            || c == '-' //マイナス
                            || ( c >= '0' && c <= '9' ) //整数or実数
                            || c == '[' //配列
                            || c == '{' //オブジェクト
                       )
                    {
                        ungetc(c, fp);
                        break;
                    }
                }
                if ( c == '"' )
                {
                    this->datatype[key] = "string";
                    char * temp = GLTF::common::getString(fp);
                    this->data[key] = new std::string(temp);
                    delete[] temp;
                }
                else if ( c == 't' || c == 'f' )
                {
                    this->datatype[key] = "boolean";
                    this->data[key] = new bool(GLTF::common::getBoolean(fp));
                }
                else if ( c == 'n' )
                {
                    this->datatype[key] = "void *";
                    this->data[key] = NULL;
                }
                else if ( c == '-' || (c >= '0' && c <= '9') )
                {//XXX
                    long retFtell = ftell(fp);
                    size_t charsiz = 0;
                    char c;
                    while ( (c = fgetc(fp)) != EOF)
                    {
                        if ( !((c>='0'&&c<='9')||c=='.'||c=='-'||c=='e'||c=='E'||c=='-'||c=='+') ) break;
                        ++charsiz;
                    }
                    char * temp = new char[charsiz+1];
                    temp[charsiz] = 0;
                    fseek(fp, retFtell, SEEK_SET);
                    for(unsigned int i=0; i < charsiz; i++)
                        temp[i] = fgetc(fp);
                    bool isDouble = false;
                    for (unsigned int i = 0; i < charsiz; i++)
                    {
                        if (temp[i] == '.')
                        {
                            isDouble = true;
                            break;
                        }
                    }
                    if (isDouble)
                    {
                        this->datatype[key] = "number";
                        this->data[key] = new double(strtod(temp, NULL));
                    }
                    else
                    {
                        this->datatype[key] = "integer";
                        this->data[key] = new int(atoi(temp));
                    }
                    delete[] temp;
                }
                else if ( c == '{' )
                {
                    this->datatype[key] = "object";
                    this->data[key] = new object_(fp);
                }
                else if ( c == '[' )
                {
                    this->datatype[key] = "array";
                    this->data[key] = new array_(fp);
                }
            }
        }
    }

    std::string object_::findType(std::string arg) const
    {
        if (datatype.find(arg) != datatype.end()) return datatype.find(arg)->second;
        else return "n/a";
    }

    std::map<std::string, void *>::const_iterator object_::find(std::string arg) const
    {
        return data.find(arg);
    }

    array_::array_(FILE * fp)
    {
        fgetc(fp); // 最初の'['の読み飛ばし
        char c;
        while ( (c = fgetc(fp)) != ']' )
        {
            if (       c == '"' //文字列
                    || c == '[' //配列
                    || c == 't' //true
                    || c == 'f' //false
                    || c == 'n' //null
                    || c == '-' //マイナス
                    || ( c >= '0' && c <= '9' ) //整数or実数
                    || c == '{' //オブジェクト
               )
            {
                ungetc(c, fp);
            }
            else if (c == EOF)
                break;
            else
                continue;
            if ( c == '"' )
            {
                this->datatype.push_back("string");
                char * temp = GLTF::common::getString(fp);
                this->data.push_back(new std::string(temp));
                delete[] temp;
            }
            else if ( c == 't' || c == 'f' )
            {
                this->datatype.push_back("boolean");
                this->data.push_back( new bool(GLTF::common::getBoolean(fp)) );
            }
            else if ( c == 'n' )
            {
                this->datatype.push_back("void *");
                this->data.push_back(NULL);
            }
            else if ( c == '-' || (c >= '0' && c <= '9') )
            {
                long retFtell = ftell(fp);
                size_t charsiz = 0;
                char c;
                while ( (c = fgetc(fp)) != EOF)
                {
                    if ( !((c>='0'&&c<='9')||c=='.'||c=='-'||c=='e'||c=='E'||c=='-'||c=='+') ) break;
                    ++charsiz;
                }
                char * temp = new char[charsiz+1];
                temp[charsiz] = 0;
                fseek(fp, retFtell, SEEK_SET);
                for(unsigned int i=0; i < charsiz; i++)
                    temp[i] = fgetc(fp);
                bool isDouble = false;
                for (unsigned int i = 0; i < charsiz; i++)
                {
                    if (temp[i] == '.')
                    {
                        isDouble = true;
                        break;
                    }
                }
                if (isDouble)
                {
                    this->datatype.push_back("number");
                    this->data.push_back(new double(strtod(temp, NULL)));
                }
                else
                {
                    this->datatype.push_back("integer");
                    this->data.push_back(new int(atoi(temp)));
                }
                delete[] temp;
            }
            else if ( c == '{' )
            {
                this->datatype.push_back("object");
                this->data.push_back(new object_(fp));
            }
            else if ( c == '[' )
            {
                this->datatype.push_back("array");
                this->data.push_back(new array_(fp));
            }
        }
    }

    std::string array_::getType(size_t arg) const
    {
        return datatype[arg];
    }

    void * array_::operator[](size_t arg) const
    {
        return data[arg];
    }

} // namespace GLTF
