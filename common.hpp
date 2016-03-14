#ifndef GLTF_COMMON_H
#define GLTF_COMMON_H

class glTF;

namespace GLTF
{
    struct parseinfo
    {
        std::string key;
        enum typeInfo
        {
            XXX,
            STRING,
            INTEGER,
            NUMBER,
            BOOLEAN,
            STRING_VECTOR,
            INTEGER_VECTOR,
            NUMBER_VECTOR,
            BOOLEAN_VECTOR,
            STRING_ARRAY,
            INTEGER_ARRAY,
            NUMBER_ARRAY,
            BOOLEAN_ARRAY,
            OBJECT,//map<str,void*>
            ARRAY,
            DICTIONARY,//map str str
            SUB_CLASS,
            SUB_CLASS_VECTOR,
            SUB_CLASS_DICTIONARY,
        };
        typeInfo info;
        void * const ret;
        std::string subClassName;
        size_t arrayLength;
        bool isRequired;
    };

    void parser(std::vector<struct parseinfo> &, FILE *&);

    namespace common
    {
        char *  getString(FILE * &);
        void    getStringArray(std::vector<std::string> &, FILE * &);
        void    getStringStaticArray(std::string *, FILE * &, size_t);
        void    getStringStaticArray(char **, FILE * &, size_t);

        bool    getBoolean(FILE * &);
        void    getBooleanArray(std::vector<bool> &, FILE * &);
        void    getBooleanStaticArray(bool *, FILE * &, size_t);

        int     getInteger(FILE * &);
        void    getIntegerArray(std::vector<int> &, FILE * &);
        void    getIntegerStaticArray(int *, FILE * &, size_t);

        double  getNumber(FILE * &);
        void    getNumberArray(std::vector<double> &, FILE * &);
        void    getNumberStaticArray(double *, FILE * &, size_t);
        void    TODO();
    }
} // namespace GLTF
#endif
