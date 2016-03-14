#include "./GLTF.h"

namespace GLTF
{
    namespace common
    {
        char * getString(FILE * &fp) //テスト完了！
        {
            size_t charsiz = 0;
            while ( fgetc(fp) != '"');
            long retFtell = ftell(fp);
            while ( fgetc(fp) != '"') ++charsiz;
            char *value = new char[charsiz + 1];
            value[charsiz] = 0;
            fseek(fp, retFtell, SEEK_SET);
            for (unsigned int i = 0; i < charsiz; i++) value[i] = fgetc(fp);
            fgetc(fp);// 最後の'"'の読み飛ばし
            return value;
        }

        void getStringArray(std::vector<std::string> &values, FILE * &fp) //テスト完了！
        {
            char c;
            while ((c = fgetc(fp)) != ']' )
            {
                if (c == '[' || c == '"')
                {
                    ungetc(c, fp);
                    break;
                }
            }
            if ( c == ']' ) return;
            else if ( c == '"' ) values.push_back(getString(fp));
            else
            {
                while ((c= fgetc(fp)) != ']')
                {
                    if (c == '"')
                    {
                        ungetc(c, fp);
                        values.push_back(getString(fp));
                    }
                }
            }
        }

        void getStringStaticArray(std::string * values, FILE * &fp, size_t n)
        {
            char c;
            while ((c = fgetc(fp)) != EOF )
            {
                if (c == '[' || c == '"')
                {
                    ungetc(c, fp);
                    break;
                }
            }
            if (c != '[') fprintf(stderr, _("%s : error. found string before '['.\n"), __func__);
            else
            {
                size_t i = 0;
                while ((c= fgetc(fp)) != ']')
                {
                    if (c == '"')
                    {
                        ungetc(c, fp);
                        values[i] = getString(fp);
                        if (++i == n -1) break;
                    }
                }
                if (n -1 -i) fprintf(stderr, "%s : warning : 指定された要素数に達する前に']'に到達しました。\n", __func__);
            }
        }

        void getStringStaticArray(char ** values, FILE * &fp, size_t n)
        {
            char c;
            while ((c = fgetc(fp)) != EOF )
            {
                if (c == '[' || c == '"')
                {
                    ungetc(c, fp);
                    break;
                }
            }
            if (c != '[') fprintf(stderr, _("%s : error. found string before '['.\n"), __func__);
            else
            {
                size_t i = 0;
                while ((c= fgetc(fp)) != ']')
                {
                    if (c == '"')
                    {
                        ungetc(c, fp);
                        values[i] = getString(fp);
                        if (++i == n -1) break;
                    }
                }
                if (n -1 -i) fprintf(stderr, "%s : warning : 指定された要素数に達する前に']'に到達しました。\n", __func__);
            }
        }

        bool getBoolean(FILE * &fp) //テスト完了。改善の余地あり
        {
            char c = 0;
            const size_t charsiz = 8;
            while ( (c = fgetc(fp)) != EOF )
            {
                if ( c == 't' || c == 'f' )
                {
                    ungetc(c, fp);
                    break;
                }
            }
            char buf[8] = {};
            for (unsigned int i = 0; i < charsiz -1; i++)
            {
                c  = fgetc(fp);
                if ( !(c >= 'a' && c <= 'z') ) break;
                buf[i] = c;
            }
            if ( !(strncmp(buf, "true", charsiz)) ) return true;
            else if ( !(strncmp(buf, "false", charsiz)) ) return false;
            else fprintf(stderr, "%s : error [%s] is not a boolean.\n", __func__, buf);
            return false;
        }

        void getBooleanArray(std::vector<bool> &values, FILE * &fp)
        {
            char c;
            while ( (c = fgetc(fp)) != EOF )
            {
                if (c == '[' || c == 't' || c == 'f' )
                {
                    ungetc(c, fp);
                    break;
                }
            }
            if (c != '[') values.push_back(getBoolean(fp));
            else
            {
                values.push_back(getBoolean(fp));
                while ((c = fgetc(fp)) != ']')
                {
                    if ( c == 't' || c == 'f' )
                    {
                        ungetc(c, fp);
                        values.push_back(getBoolean(fp));
                    }
                }
            }
        }

        void getBooleanStaticArray(bool * values, FILE * &fp, size_t n)
        {
            char c;
            while ( (c = fgetc(fp)) != EOF )
            {
                if (c == '[' || c == 't' || c == 'f' )
                {
                    ungetc(c, fp);
                    break;
                }
            }
            if (c != '[') fprintf(stderr, _("%s : error. found boolean before '['.\n"), __func__);
            else
            {
                size_t i = 0;
                while ((c = fgetc(fp)) != ']')
                {
                    if ( c == 't' || c == 'f' )
                    {
                        ungetc(c, fp);
                        values[i] = getBoolean(fp);
                        if (++i == n -1) break;
                    }
                }
                if (n -1 -i) fprintf(stderr, "%s : warning : 指定された要素数に達する前に']'に到達しました。\n", __func__);
            }
        }

        int getInteger(FILE * &fp)
        {
            char c;
            size_t charsiz = 0;
            long retFtell;
            while (1)
            {
                c = fgetc(fp);
                if ((c >= '0' && c <= '9') || c == '-')
                {
                    ungetc(c, fp);
                    retFtell = ftell(fp);
                    break;
                }
            }
            while (1)
            {
                c = fgetc(fp);
                if (c < '0' || c > '9') break;
                ++charsiz;
            }
            char * buf = new char[charsiz +1];
            buf[charsiz] = 0;
            fseek(fp, retFtell, SEEK_SET);
            for (unsigned int i = 0; i < charsiz; i++) buf[i] = fgetc(fp);
            int value = atoi(buf);
            delete[] buf;
            return value;
        }

        void getIntegerArray(std::vector<int> &values, FILE * &fp)
        {
            char c;
            while ( (c = fgetc(fp)) != EOF )
            {
                if (c == '[' || (c >= '0' && c <= '9') || c == '-')
                {
                    ungetc(c, fp);
                    break;
                }
            }
            if (c != '[') values.push_back(getInteger(fp));
            else
            {
                values.push_back(getInteger(fp));
                while ((c = fgetc(fp)) != ']')
                {
                    if ((c >= '0' && c <= '9') || c == '-')
                    {
                        ungetc(c, fp);
                        values.push_back(getInteger(fp));
                    }
                }
            }
        }

        void getIntegerStaticArray(int * values, FILE * &fp, size_t n)
        {
            char c;
            while ( (c = fgetc(fp)) != EOF )
            {
                if (c == '[' || (c >= '0' && c <= '9') || c == '-')
                {
                    ungetc(c, fp);
                    break;
                }
            }
            if (c != '[') fprintf(stderr, _("%s : error. found Integer before '['.\n"), __func__);
            else
            {
                size_t i = 0;
                while ((c = fgetc(fp)) != ']')
                {
                    if ((c >= '0' && c <= '9') || c == '-')
                    {
                        ungetc(c, fp);
                        values[i] = getInteger(fp);
                        if (++i == n -1) break;
                    }
                }
                if (n -1 -i) fprintf(stderr, "%s : warning : 指定された要素数に達する前に']'に到達しました。\n", __func__);
            }
        }

        double getNumber(FILE * &fp) //テスト完了！
        {
            char c;
            size_t charsiz = 0;
            long retFtell;
            while (1)
            {
                c = fgetc(fp);
                if ((c >= '0' && c <= '9') || c == '-')
                {
                    ungetc(c, fp);
                    retFtell = ftell(fp);
                    fgetc(fp);//先頭に一度だけ出現する'-'のための処理
                    ++charsiz;
                    break;
                }
            }
            while (1)
            {
                c = fgetc(fp);
                if ( !((c >= '0' && c <= '9') || c == '.') ) break;
                ++charsiz;
            }
            char * buf = new char[charsiz +1];
            buf[charsiz] = 0;
            fseek(fp, retFtell, SEEK_SET);
            for (unsigned int i = 0; i < charsiz; i++) buf[i] = fgetc(fp);
            double value = atof(buf);
            delete[] buf;
            return value;
        }

        void getNumberArray(std::vector<double> &values, FILE * &fp)
        {
            char c;
            while ((c = fgetc(fp)) != ']' )
            {
                if (c == '[' || (c >= '0' && c <= '9') || c == '-')
                {
                    ungetc(c, fp);
                    break;
                }
            }
            if ( c == ']' ) return;
            else if (c != '[') values.push_back(getNumber(fp));
            else
            {
                while ((c = fgetc(fp)) != ']')
                {
                    if ((c >= '0' && c <= '9') || c == '-')
                    {
                        ungetc(c, fp);
                        values.push_back(getNumber(fp));
                    }
                }
            }
        }

        void getNumberStaticArray(double * values, FILE * &fp, size_t n)
        {
            char c;
            while ((c = fgetc(fp)) != ']' )
            {
                if (c == '[' || (c >= '0' && c <= '9') || c == '-')
                {
                    ungetc(c, fp);
                    break;
                }
            }
            if ( c == ']' ) return;
            if (c != '[') fprintf(stderr, _("%s : error. found Number before '['.\n"), __func__);
            else
            {
                size_t i = 0;
                while ((c = fgetc(fp)) != ']')
                {
                    if ((c >= '0' && c <= '9') || c == '-')
                    {
                        ungetc(c, fp);
                        values[i] = getNumber(fp);
                        ++i;
                        if (i == n) break;
                    }
                }
                if ( i < n ) fprintf(stderr, "%s : warning : 指定された要素数に達する前に']'に到達しました。\n", __func__);
            }
        }
        void TODO(){puts("TODO");}
    }


    void subClassFactory(void * data, std::string name, FILE * fp)
    {
        if ( name == "animation_::channel_::target_" )
            (*static_cast<std::unique_ptr<GLTF::animation_::channel_::target_>*>(data)).reset(new GLTF::animation_::channel_::target_(fp));
        else if ( name == "animation_::sampler_" )
            (*static_cast<std::unique_ptr<GLTF::animation_::sampler_>*>(data)).reset(new GLTF::animation_::sampler_(fp));
        else if ( name == "asset_::profile_" )
            (*static_cast<std::unique_ptr<GLTF::asset_::profile_>*>(data)).reset(new GLTF::asset_::profile_(fp));
        else if ( name == "camera_::orthographic_" )
            (*static_cast<std::unique_ptr<GLTF::camera_::orthographic_>*>(data)).reset(new GLTF::camera_::orthographic_(fp));
        else if ( name == "camera_::perspective_" )
            (*static_cast<std::unique_ptr<GLTF::camera_::perspective_>*>(data)).reset(new GLTF::camera_::perspective_(fp));
        else if ( name == "technique_::states_" )
            (*static_cast<std::unique_ptr<GLTF::technique_::states_>*>(data)).reset(new GLTF::technique_::states_(fp));
        else if ( name == "technique_::states_::functions_" )
            (*static_cast<std::unique_ptr<GLTF::technique_::states_::functions_>*>(data)).reset(new GLTF::technique_::states_::functions_(fp));
        else
        {
            fprintf(stderr, _("%s :error: unknown subclass name [%s]\n"), __func__, name.c_str());
        }
    }

    void subClassFactoryVector(std::string name, void * data, FILE * fp)
    {
        char c;
        while ( (c = fgetc(fp)) != ']' )
        {
            if (c == '{' || c == ',')
            {
                if ( name == "animation_::channel_" )
                    (*static_cast<std::vector<GLTF::animation_::channel_>*>(data)).push_back(fp);
                else if ( name == "mesh_::primitive_" )
                    (*static_cast<std::vector<GLTF::mesh_::primitive_>*>(data)).push_back(fp);
            }
            else continue;
        }
    }

    void subClassFactoryMap(std::string name, void * data, FILE * fp)
    {
        char c;
        while ( (c= fgetc(fp)) != '}' )
        {
            if (c == '"')
            {
                ungetc(c, fp);
                char * tempB;
                tempB = GLTF::common::getString(fp);
                if ( name == "animation_::sampler_" )
                {
                    if ( (*static_cast<std::map <std::string, std::unique_ptr<GLTF::animation_::sampler_> >*>(data)).find(tempB) == 
                            (*static_cast<std::map <std::string, std::unique_ptr<GLTF::animation_::sampler_> >*>(data)).end() )
                    {
                        (*static_cast<std::map <std::string, std::unique_ptr<GLTF::animation_::sampler_> >*>(data))[tempB]
                            .reset(new GLTF::animation_::sampler_(fp));
                    }
                }
                else if ( name == "technique_::parameters_" )
                {
                    if ( (*static_cast<std::map <std::string, std::unique_ptr<GLTF::technique_::parameters_> >*>(data)).find(tempB) == 
                            (*static_cast<std::map <std::string, std::unique_ptr<GLTF::technique_::parameters_> >*>(data)).end() )
                    {
                        (*static_cast<std::map <std::string, std::unique_ptr<GLTF::technique_::parameters_> >*>(data))[tempB]
                            .reset(new GLTF::technique_::parameters_(fp));
                    }
                }
            }
            else if ( c == EOF ) break;
        }
    }

    void parser(std::vector<struct parseinfo> & options, FILE * &fp)
    {
        std::vector<bool> checklist;

        for (std::vector<struct parseinfo>::iterator it = options.begin(); it != options.end(); ++it)
        {
            checklist.push_back(it->isRequired ? false : true);
        }
        char c;
        char * buf = NULL;

        while ( (c = fgetc(fp)) != '}' )
        {
            if ( c == '"' )
            {
                ungetc(c, fp);
                if ( buf )
                {
                    delete[] buf;
                    buf = NULL;
                }
                buf = GLTF::common::getString(fp);
                
#ifdef GLTF_PARSER_DEBUG
                printf("key name : %s\n", buf);
#endif

                for (std::vector<struct parseinfo>::iterator it = options.begin(); it != options.end(); ++it)
                {// bufのstrncmp一回分
                    if ( std::string(buf) == it->key )
                    {
                        switch (it->info)
                        {
                            case parseinfo::XXX:
                                ;
                                break;

                            case parseinfo::STRING:
                                *static_cast<std::string *>(it->ret) = GLTF::common::getString(fp);
                                break;
                            case parseinfo::INTEGER:
                                *static_cast<int *>(it->ret) = GLTF::common::getInteger(fp);
                                break;
                            case parseinfo::NUMBER:
                                *static_cast<double *>(it->ret) = GLTF::common::getNumber(fp);
                                break;
                            case parseinfo::BOOLEAN:
                                *static_cast<bool *>(it->ret) = GLTF::common::getBoolean(fp);
                                break;
                            case parseinfo::STRING_VECTOR:
                                GLTF::common::getStringArray(*static_cast<std::vector<std::string>*>(it->ret), fp);
                                break;
                            case parseinfo::INTEGER_VECTOR:
                                GLTF::common::getIntegerArray(*static_cast<std::vector<int>*>(it->ret), fp);
                                break;
                            case parseinfo::NUMBER_VECTOR:
                                GLTF::common::getNumberArray(*static_cast<std::vector<double>*>(it->ret), fp);
                                break;
                            case parseinfo::BOOLEAN_VECTOR:
                                GLTF::common::getBooleanArray(*static_cast<std::vector<bool>*>(it->ret), fp);
                                break;
                            case parseinfo::STRING_ARRAY:
                                GLTF::common::getStringStaticArray(*static_cast<char ***>(it->ret), fp, it->arrayLength);
                                break;
                            case parseinfo::INTEGER_ARRAY:
                                GLTF::common::getIntegerStaticArray(*static_cast<int **>(it->ret), fp, it->arrayLength);
                                break;
                            case parseinfo::NUMBER_ARRAY:
                                GLTF::common::getNumberStaticArray(static_cast<double *>(it->ret), fp, it->arrayLength);
                                break;
                            case parseinfo::BOOLEAN_ARRAY:
                                GLTF::common::getBooleanStaticArray(*static_cast<bool **>(it->ret), fp, it->arrayLength);
                                break;
                            case parseinfo::OBJECT:
                                (*static_cast<std::unique_ptr<GLTF::object_>*>(it->ret)).reset(new GLTF::object_(fp));
                                break;
                            case parseinfo::ARRAY:
                                {
                                    char x = 0;
                                    while( (x = fgetc(fp)) != '[');
                                    ungetc(x, fp);
                                    (*static_cast<std::unique_ptr<GLTF::array_>*>(it->ret)).reset(new GLTF::array_(fp));
                                    break;
                                }
                            case parseinfo::DICTIONARY:
                                {
                                    char c;
                                    while ( (c = fgetc(fp)) != '}' )
                                    {
                                        char * temp = NULL;
                                        if ( c == '"' )
                                        {
                                            ungetc(c, fp);
                                            if ( temp )
                                            {
                                                delete[] temp;
                                                temp = NULL;
                                            }
                                            temp = GLTF::common::getString(fp);
                                            (*static_cast<std::map<std::string, std::string>*>(it->ret))[temp] = GLTF::common::getString(fp);
                                        }
                                        else if ( c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == ':' || c == '{' || c == ',' ) continue;
                                        else if ( c == EOF ) break;
                                        else fprintf(stderr, "%s : unknown char [%c] \n", __func__, c);
                                    }
                                }
                                break;
                            case parseinfo::SUB_CLASS: // XXX typ
                                 GLTF::subClassFactory(it->ret, it->subClassName, fp);
                                break;
                            case parseinfo::SUB_CLASS_VECTOR:
                                GLTF::subClassFactoryVector(it->subClassName, it->ret, fp);
                                break;
                            case parseinfo::SUB_CLASS_DICTIONARY:
                                GLTF::subClassFactoryMap(it->subClassName, it->ret, fp);
                                break;
                        }
                        checklist[std::distance(options.begin(), it)] = true;
                        break;
                    }
                }
            }
            else if ( c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == ':' || c == '{' || c == ',' || c == ']' ) continue;
            else if ( c == EOF ) break;
            else fprintf(stderr, _("%s : unknown char [%c] \n"), __func__, c);
        }
        for (std::vector<struct parseinfo>::iterator it = options.begin(); it != options.end(); ++it)
            if ( !(checklist[std::distance(options.begin(), it)]) )
                fprintf(stderr, _("%s :error: could not found a required element [\"%s\"]"), __func__, it->key.c_str());
    }
} // namespace GLTF
