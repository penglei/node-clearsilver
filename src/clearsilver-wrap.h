#ifndef CLEARSILVER_WRAP_H
#define CLEARSILVER_WRAP_H

#include <string>
#include <string.h>

#include <v8.h>
#include <node.h>

#include "cs.h"
#include "util/neo_str.h"
#include "stdlib.h"


using namespace v8;

class ClearSilver : public node::ObjectWrap{
    private:
        static const char *javascriptClassName;
        //出错信息
        NEOERR *err;
        //cs 模板编译后的指针
        CSPARSE *parse;
        //当前使用的hdf数据指针
        HDF *hdf;
        //解析cs模板遇到include时使用的base地址
        std::string baseInclude;
        //如果是通过setCsString传入模板的，需要把模板存起来。。。cs引擎真坑爹
        char *csStringChar;


    //仔细考虑，要把方法作为prototype就必须把这些方法做成静态的.这是因为先有鸡还是先蛋的问题
    //通过静态方法找到真实的C++(ClearSilver)对象，还是通过C++(ClearSilver)对象找到相应的方法。这是一个问题。
    //目前看来，采用静态方法最简单，即先绑定prototype的方法，在方法里再找到ClearSilver对象，换个思路会比较复杂
    //javascript 访问的方法入口(prototype)
    public:
        //初始化javascript可访问的ClearSilver Constructor
        static v8::Handle<Function> Init(v8::Handle<v8::Object> target);
        //ClearSilver Constructor
        static v8::Handle<v8::Value> New(const v8::Arguments& args);
        //解析模板和数据，返回字符串
        static v8::Handle<v8::Value> render(const Arguments& args);

        static v8::Handle<v8::Value> setHdfString(const Arguments& args);
        static v8::Handle<v8::Value> setCsString(const Arguments& args);

        static v8::Handle<v8::Value> setValue(const Arguments& args);

        //baseInclude 的accessor
        static void SetIncludeBase(Local<String>, Local<Value>, const AccessorInfo&);
        static v8::Handle<v8::Value> GetIncludeBase(Local<String>, const AccessorInfo&);


        class csRenderCtx{
            public:
                ClearSilver *          ClearSilverInstance;//用来保存ClearSilver实例
                std::string *   result;
        };

        void outputRender(char *, csRenderCtx *);

    private:
        v8::Handle<v8::Value> _render(const Arguments& args);
        v8::Handle<v8::Value> _setCsString(const Arguments& args);
        v8::Handle<v8::Value> _setHdfString(const Arguments& args);
        v8::Handle<v8::Value> _setValue(const Arguments& args);

        //////////////////////
        bool readHdfFile(const char*);
        bool parseCsFile(const char*);
        bool CheckError(std::string&, const char*);

    private:
        ClearSilver();
        ~ClearSilver();
};

NEOERR* outputCallBack (void *ctx, char *s);

#endif
