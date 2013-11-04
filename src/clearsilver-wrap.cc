#include "clearsilver-wrap.h"
#include <iostream>

const char* ClearSilver::javascriptClassName = "ClearSilver";

NEOERR* outputCallBack (void *ctx, char *s){
    ClearSilver::csRenderCtx *csCtx = static_cast<ClearSilver::csRenderCtx *>(ctx);
    csCtx->ClearSilverInstance->outputRender(s, csCtx);
    return STATUS_OK;
}

/////////////////ClearSilver instance method/////////////////////

ClearSilver::ClearSilver():parse(NULL), hdf(NULL), csStringChar(NULL){
    err = hdf_init(&hdf);
    //这个时候才初始化一个parse。在这里传入hdf是为了可以配置cs引擎(可以在数据里指定Config.*等一些字段)
    err = cs_init(&parse, hdf);
}

//如果头文件里面定义了构造函数，那就必须实现，否则就不要写声明
ClearSilver::~ClearSilver() {
    cs_destroy(&parse);
    hdf_destroy(&hdf);
    if (csStringChar != NULL){
        free(csStringChar);
        csStringChar = NULL;
    }
}

bool ClearSilver::CheckError(std::string& errorMessage, const char* prefixMessage){
    if (err != STATUS_OK) {
        errorMessage.append(prefixMessage);

        STRING errString;
        string_init(&errString);
        string_append (&errString, "");
        nerr_error_traceback(err, &errString);

        errorMessage.append(errString.buf);

        //nerr_log_error(err);

        err = STATUS_OK;
        return true;
    }
    return false;
}

v8::Handle<v8::Value> ClearSilver::_render(const Arguments& args){
    HandleScope handle_scope;

    ClearSilver::csRenderCtx ctx;//用局部变量，因为cs_render是同步执行的，不会因为ctx被释放出错
    ctx.ClearSilverInstance = this;
    std::string csRenderResult;
    ctx.result = &csRenderResult;

    err = cs_render(parse, &ctx, outputCallBack);
    if (err != STATUS_OK){
        err = nerr_pass(err);
        std::string errorMessage;
        if (CheckError(errorMessage, "render clearsilver template error...\n\n")){
            return v8::ThrowException(v8::String::New(errorMessage.c_str()));
        }
    }

    Local<v8::String> renderout = v8::String::New(ctx.result->c_str());

    //调用render传进来的回调函数接口

    if (args.Length() > 0){
        const unsigned argc = 1;
        Local<Function> cb = Local<Function>::Cast(args[0]);
        if (!cb.IsEmpty()){
            Local<Value> argv[argc] = { renderout };
            cb->Call(Context::GetCurrent()->Global(), argc, argv);//this 为全局的
        }
    }

    return handle_scope.Close(renderout);
}

v8::Handle<v8::Value> ClearSilver::_setHdfString(const Arguments& args){
    v8::String::Utf8Value  hdfString(args[0]);

    if (hdf != NULL){//set 数据就要把原来的数据全部删除掉
        hdf_destroy(&hdf);
        hdf_init(&hdf);
    }

    err = hdf_read_string(hdf, *hdfString);
    if (err){
        //TODO update the deal way of error
        nerr_log_error(err);
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> ClearSilver::_setCsString(const Arguments& args){

    v8::String::Utf8Value csString(args[0]);

    unsigned long len = csString.length() + 1;
    csStringChar = (char *) malloc (len);
    memset((void*)csStringChar, 0, len);
    memcpy(csStringChar, *csString, len - 1);
    csStringChar[len - 1] = '\0';

    //TODO 如果模板都变了，要全部重新初始化?直接抛出异常吧.

    //if (parse == NULL){
        cs_parse_string(parse, csStringChar, len);
    //}

    return v8::Undefined();
}

v8::Handle<v8::Value> ClearSilver::_setValue(const Arguments& args){
    v8::String::Utf8Value key(args[0]);
    v8::String::Utf8Value val(args[1]);

    err = hdf_set_value(hdf, *key, *val);
    if (err != STATUS_OK){
        nerr_log_error(err);
    }
    return v8::Undefined();
}


void ClearSilver::outputRender(char *result, ClearSilver::csRenderCtx *ctx){
    ctx->result->append(result);
}

bool ClearSilver::parseCsFile(const char* cs_filename){
    //TODO change current dir
    err = cs_parse_file(parse, cs_filename);
    if (err != STATUS_OK){
        err = nerr_pass(err);
        return false;
    }
    return true;
}

bool ClearSilver::readHdfFile(const char* hdf_filename){
    err = hdf_read_file(hdf, hdf_filename);
    if (err != STATUS_OK) return false;
    return true;
}
//////////////////////static method 用来做为javascript的接口//////////////////////////

/**
 *解析cs模板
 */
v8::Handle<v8::Value> ClearSilver::render(const Arguments& args) {
    //一个关于arguments的用法
    //obj->counter_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();

    //TODO 获得当前的工作目录，记录下来，然后切换到baseInclude
    v8::HandleScope handle_scope;
    ClearSilver *_this =  Unwrap<ClearSilver>(args.This());
    //TODO 还原当前同坐目录到目录。这是因为cs引擎内部没有提供更改include的相对路径的方法
    //它是直接以当前工作目录为起点的
    return handle_scope.Close(_this->_render(args));//真正解析的地方
}

v8::Handle<v8::Value> ClearSilver::setHdfString(const Arguments& args) {
    v8::HandleScope handle_scope;
    ClearSilver *_this =  Unwrap<ClearSilver>(args.This());
    return handle_scope.Close(_this->_setHdfString(args));
}

v8::Handle<v8::Value> ClearSilver::setCsString(const Arguments& args) {
    v8::HandleScope handle_scope;
    ClearSilver *_this =  Unwrap<ClearSilver>(args.This());
    return handle_scope.Close(_this->_setCsString(args));
}

v8::Handle<v8::Value> ClearSilver::setValue(const Arguments& args) {
    v8::HandleScope handle_scope;
    ClearSilver *_this =  Unwrap<ClearSilver>(args.This());
    return handle_scope.Close(_this->_setValue(args));
}

//{baseInclude accessor
/**
 *获得当前cs引擎使用的相对路径
 */
v8::Handle<Value> ClearSilver::GetIncludeBase(Local<String> property, const AccessorInfo &info){
    Handle<Object> self = info.Holder();
    //info.Holder() == info.This()->Value();

    ClearSilver *_this =  Unwrap<ClearSilver>(self);
    return v8::String::New(_this->baseInclude.c_str());
}

void ClearSilver::SetIncludeBase(Local<String> property, Local<Value> value, const AccessorInfo &info){
    HandleScope scope;
    Handle<Object> self = info.Holder();
    ClearSilver *_this =  Unwrap<ClearSilver>(self);

    v8::String::Utf8Value utf8_str(value);
    _this->baseInclude.assign(*utf8_str);
}
//}

/**
 *初始化ClearSilver接口
 */
v8::Handle<Function> ClearSilver::Init(v8::Handle<v8::Object> target){

    HandleScope scope;
    Local<FunctionTemplate> csConstructorTpl = FunctionTemplate::New(New);
    csConstructorTpl->SetClassName(String::NewSymbol(javascriptClassName));
    //csConstructorTpl->Set("parseHDF", parseHDF);
    //csConstructorTpl->Set("parseCS", parseCS);//need "baseInclude"

    //构造器对应的Object只有一个数据项，就是这个类(ClearSilver)的实例
    Local<ObjectTemplate> csInstanceTpl = csConstructorTpl->InstanceTemplate();
    //内部有两个数据坑，一个是用来放this指针，另一个放自定义数据baseInclude
    csInstanceTpl->SetInternalFieldCount(2);
    csInstanceTpl->SetAccessor(String::NewSymbol("baseInclude"), GetIncludeBase, SetIncludeBase);

    //给构造器模板添加原型方法
    Local<Template> csPrototype = csConstructorTpl->PrototypeTemplate();
    csPrototype->Set(v8::String::NewSymbol("render"),
                     FunctionTemplate::New(render)->GetFunction());
    csPrototype->Set(v8::String::NewSymbol("setCsString"),
                     FunctionTemplate::New(setCsString)->GetFunction());
    csPrototype->Set(v8::String::NewSymbol("setHdfString"),
                     FunctionTemplate::New(setHdfString)->GetFunction());
    csPrototype->Set(v8::String::NewSymbol("setValue"),
                     FunctionTemplate::New(setValue)->GetFunction());

    //这里为什么要用Persistent＜Function> ?? 它会怎么删除呢?由node模块管理器么?
    Persistent<Function> csConstructor = Persistent<Function>::New(csConstructorTpl->GetFunction());
    //export the interface
    target->Set(String::NewSymbol(javascriptClassName), csConstructor);
    return csConstructor;
}

v8::Handle<v8::Value> ClearSilver::New(const Arguments& args) {
    HandleScope scope;
    if (Context::GetCurrent()->Global() == args.This()){
        //直接执行constructor，是不行的
        return v8::ThrowException(v8::String::New("Can't call ClearSilver constructor directly."));
    }
    std::string errorMessage;

    //ClearSilver init
    ClearSilver* csObj = new ClearSilver();
    if (csObj->CheckError(errorMessage, "init ClearSilver engine error...\n\n")){
        return v8::ThrowException(v8::String::New(errorMessage.c_str()));
    }

    //init with hdf data
    if (!args[1]->IsUndefined()){
        if (!csObj->readHdfFile(*v8::String::Utf8Value(args[1]))){
            csObj->CheckError(errorMessage, "init with HDF data file error...\n\n");
            return v8::ThrowException(v8::String::New(errorMessage.c_str()));
        }
    }

    //init cs template
    if (!args[0]->IsUndefined()){
        if (!csObj->parseCsFile(*v8::String::Utf8Value(args[0]))){
            csObj->CheckError(errorMessage, "parse cs file error...\n\n");
            return v8::ThrowException(v8::String::New(errorMessage.c_str()));
        }
    }

    //args.This()其实就是csInstanceTpl的实例Handle<Object> instance
    //instance在Wrap中会被保存到一个Persistent句柄中，以供其实static方法调用获得真实的ClearSilver对象
    csObj->Wrap(args.This());

    return args.This();
}
