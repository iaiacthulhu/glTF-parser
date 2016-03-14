#ifndef GLTF_HPP
#define GLTF_HPP

#include <map>
#include <vector>
#include <string>
#include <memory>
#include "./linear_algebra.hpp"
// node matrixをmat4に変更

namespace GLTF
{
    class object_
    {
        public:
            std::map<std::string, std::string> datatype;
            std::map<std::string, void *> data;
            std::string findType(std::string) const ;
            std::map<std::string, void*>::const_iterator find(std::string) const ;
            bool empty(void) const {return this->data.empty();}
            object_(FILE *);
    };
    class array_
    {
        public:
            std::vector<std::string> datatype;
            std::vector<void *> data;
            std::string getType(size_t) const ;
            void * operator[](size_t) const ;
            bool empty(void) const {return this->data.empty();}
            array_(FILE *);
    };
    class exts_
    {
        protected:
            exts_(){}
            exts_(const exts_&){}
            ~exts_(){}
        public:
            std::unique_ptr<object_> extensions;
            std::unique_ptr<object_> extras;
    };

    class accessor_ : public exts_
    {
        public:
            std::string bufferView;
            int byteOffset;
            int byteStride;
            int componentType;
            int count;
            std::string type;
            std::vector<double> max; //1-16
            std::vector<double> min; //1-16
            std::string name;
            accessor_(FILE * fp);
    };
    class animation_ : public exts_
    {
        public:
            class channel_ : public exts_
        {
            public:
                std::string sampler;
                class target_ : public exts_
            {
                public:
                    std::string id;
                    std::string path;
                    target_(FILE *);
            };
                std::unique_ptr<target_> targets;
                channel_(FILE *);
        };
            std::vector<channel_> channels;
            std::map<std::string, std::string> parameters;
            class sampler_ : public exts_
        {
            public:
                static const char interpolation_default[];

                std::string input;
                std::string interpolation;
                std::string output;
                sampler_(FILE *);
        };
            std::map<std::string, std::unique_ptr<GLTF::animation_::sampler_>> samplers;
            std::string name;
            animation_(FILE *);
    };
    class asset_ : public exts_
    {
        public:
            std::string copyright;
            std::string generator;
            bool        premultipliedAlpha;
            class profile_ : public exts_
        {
            public:
                static const char api_default[];
                static const char version_default[];

                std::string api;
                std::string version;
                profile_(FILE *);
        };
            std::unique_ptr<profile_> profile;
            std::string version;
            asset_(FILE *);
    };
    class buffer_ : public exts_
    {
        public:
            static const char type_default[];

            std::string     uri;
            unsigned int    byteLength;
            std::string     type;
            std::string     name;
            buffer_(FILE *);
    };
    class bufferView_ : public exts_
    {
        public:
            std::string buffer;
            int    byteOffset;
            int    byteLength;
            int    target;
            std::string name;
            bufferView_(FILE *);
    };
    class camera_ : public exts_
    {
        public:
            class orthographic_ : public exts_
            {
            public:
                double xmag;
                double ymag;
                double zfar;
                double znear;
                orthographic_(FILE *);
            };
            class perspective_ : public exts_
            {
            public:
                double aspectRatio;
                double yfov;
                double zfar;
                double znear;
                perspective_(FILE *);
            };
            std::unique_ptr<orthographic_> orthographic;
            std::unique_ptr<perspective_>  perspective;
            std::string type;
            std::string name;
            std::string projection;
            camera_(FILE *);
    };
    class image_ : public exts_
    {
        public:
            std::string uri;
            std::string name;
            image_(FILE *);
    };
    class material_ : public exts_
    {
        public:
            std::string technique;
            std::unique_ptr<GLTF::object_> values;
            std::string name;
            material_(FILE *);
    };
    class mesh_ : public exts_
    {
        public:
            class primitive_ : public exts_
        {
            public:
                std::map<std::string, std::string> attributes;
                std::string indices;
                std::string material;
                int    mode;
                primitive_(FILE *);
        };
            std::vector<primitive_> primitives;
            std::string name;
            mesh_(FILE *);
    };
    class node_ : public exts_
    {
        public:
            std::string  camera;
            std::vector<std::string> children;
            std::vector<std::string> skeletons;
            std::string  skin;
            std::string  jointName;
            mat4 matrix;
            std::vector<std::string> meshes;
            vec4  rotation;
            vec3  scale;
            vec3  translation;
            std::string  name;
            node_(FILE *);
    };
    class program_ : public exts_
    {
        public:
            std::vector<std::string> attributes;
            std::string  fragmentShader;
            std::string  vertexShader;
            std::string  name;
            program_(FILE *);
    };
    class sampler_ : public exts_
    {
        public:
            int    magFilter;
            int    minFilter;
            int    wrapS;
            int    wrapT;
            std::string name;
            sampler_(FILE *);
    };
    class scene_ : public exts_
    {
        public:
            std::vector<std::string> nodes;
            std::string  name;
            scene_(FILE *);
    };
    class shader_ : public exts_
    {
        public:
            std::string uri;
            int    type;
            std::string name;
            shader_(FILE *);
    };
    class skin_ : public exts_
    {
        public:
            mat4  bindShapeMatrix;
            std::string  inverseBindMatrices;
            std::vector<std::string> jointNames;
            std::string  name;
            skin_(FILE *);
    };
    class technique_ : public exts_
    {
        public:
            class parameters_ : public exts_
        {
            public:
                int    count;
                std::string node;
                int    type;
                std::string semantic;
                std::unique_ptr<GLTF::array_> value;
                parameters_(FILE *);
        };
            std::map<std::string, std::unique_ptr<parameters_> > parameters;
            std::map<std::string, std::string> attributes;
            std::string program;
            std::map<std::string, std::string> uniforms;
            class states_ : public exts_
        {
            public:
                std::vector<int> enable;
                class functions_ : public exts_
            {
                public:
                    double blendColor[4];
                    int    blendEquationSeparate[2];
                    int    blendFuncSeparate[4];
                    bool   colorMask[4];
                    int    cullFace[1];
                    int    depthFunc[1];
                    bool   depthMask[1];
                    double depthRange[2];
                    int    frontFace[1];
                    double lineWidth[1];
                    double polygonOffset[2];
                    double scissor[4];
                    functions_(FILE *);
            };
                std::unique_ptr<functions_> functions;
                states_(FILE *);
        };
            std::unique_ptr<states_> states;
            std::string name;
            technique_(FILE *);
    };
    class texture_ : public exts_
    {
        public:
            int    format;
            int    internalFormat;
            std::string sampler;
            std::string source;
            int    target;
            int    type;
            std::string name;
            texture_(FILE *);
    };
    class glTF : public exts_
    {
        public:
            std::map<std::string, std::unique_ptr<accessor_>>       accessors;
            std::map<std::string, std::unique_ptr<animation_>>      animations;
            std::unique_ptr<asset_>                                 asset;
            std::map<std::string, std::unique_ptr<buffer_>>         buffers;
            std::map<std::string, std::unique_ptr<bufferView_>>     bufferViews;
            std::map<std::string, std::unique_ptr<camera_>>         cameras;
            std::map<std::string, std::unique_ptr<image_>>          images;
            std::map<std::string, std::unique_ptr<material_>>       materials;
            std::map<std::string, std::unique_ptr<mesh_>>           meshes;
            std::map<std::string, std::unique_ptr<node_>>           nodes;
            std::map<std::string, std::unique_ptr<program_>>        programs;
            std::map<std::string, std::unique_ptr<sampler_>>        samplers;
            std::string                                             scene;
            std::map<std::string, std::unique_ptr<scene_>>          scenes;
            std::map<std::string, std::unique_ptr<shader_>>         shaders;
            std::map<std::string, std::unique_ptr<skin_>>           skins;
            std::map<std::string, std::unique_ptr<technique_>>      techniques;
            std::map<std::string, std::unique_ptr<texture_>>        textures;
            std::vector<std::string>                                extensionsUsed;
            glTF(const std::string);
    };

}

#endif
