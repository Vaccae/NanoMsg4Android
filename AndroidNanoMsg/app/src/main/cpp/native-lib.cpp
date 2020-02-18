#include <jni.h>
#include <string>
#include <nn.h>
#include <stdio.h>
#include <protocol.h>
#include <pair.h>
#include <android/log.h>

// log标签
#define  TAG    "NanoErrInfo"
// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

//抛异常类标签
#define ERRCLS "java/lang/IllegalArgumentException"
//抛异常函数
void throwByName(JNIEnv *env, const char *name, const char *msg) {
    jclass cls = env->FindClass(name);
    if (cls != NULL) {
        //检测是否有异常发生
        if (0 != env->ExceptionOccurred()) {
            //清除异常堆栈
            env->ExceptionClear();
        }
        env->ThrowNew(cls, msg);
    }
    env->DeleteLocalRef(cls);
}

//获取NanoMsg连接类型
int getNanotype(const char *ntype) {
    if (strcmp(ntype, "PAIR") == 0) {
        return NN_PAIR;
    } else {
        return -1;
    }
}

extern "C" JNIEXPORT jint JNICALL
Java_dem_vac_androidnanomsg_NanoMsgJNI_init(
        JNIEnv *env,
        jobject,
        jstring ntype_
) {
    int initsocket = -1;

    //设置超时时间
    int timeo = 100;
    //获取连接方式
    const char *ntype = env->GetStringUTFChars(ntype_, 0);

    try {
        initsocket = nn_socket(AF_SP, getNanotype(ntype));
        if (initsocket < 0) {
            throw initsocket;
        }

        //设置超时
        nn_setsockopt(initsocket, 0, NN_SNDTIMEO, &timeo, sizeof(timeo));
        nn_setsockopt(initsocket, 0, NN_RCVTIMEO, &timeo, sizeof(timeo));
    }
    catch (int e) {
        char errmsg[100];
        sprintf(errmsg, "创建Socket连接失败，返回码：%d", e);
        LOGE("%s\n", errmsg);
        throwByName(env, ERRCLS, errmsg);
    }
    //释放资源
    env->ReleaseStringUTFChars(ntype_, ntype);
    return initsocket;
}

extern "C" JNIEXPORT jint JNICALL
Java_dem_vac_androidnanomsg_NanoMsgJNI_bind(
        JNIEnv *env,
        jobject,
        jint socketid_,
        jstring ipadr_
) {
    int bindsocket = -1;

    //获取地址
    const char *ipadress = env->GetStringUTFChars(ipadr_, 0);
    const char *ipadrpre = "tcp://";

    int len = strlen(ipadress) + strlen(ipadrpre);
    char ipstr[len];
    strcpy(ipstr, ipadrpre);
    strcat(ipstr, ipadress);


    try {
        //绑定地址
        bindsocket = nn_bind(socketid_, ipstr);
        if (bindsocket < 0) {
            throw bindsocket;
        }
    } catch (int e) {
        char errmsg[100];
        sprintf(errmsg, "绑定地址失败，返回码：%d", e);
        LOGE("%s\n", errmsg);
        throwByName(env, ERRCLS, errmsg);
    }
    //释放资源
    env->ReleaseStringUTFChars(ipadr_, ipadress);
    return bindsocket;
}

extern "C" JNIEXPORT jint JNICALL
Java_dem_vac_androidnanomsg_NanoMsgJNI_close(
        JNIEnv *env,
        jobject,
        jint socketid_
) {
    int closesocket = -1;
    try {
        //关闭套接字
        closesocket = nn_close(socketid_);
        if (closesocket != 1) {
            throw closesocket;
        }

    } catch (int e) {
        char errmsg[100];
        sprintf(errmsg, "关闭套接字失败，返回码：%d", e);
        LOGE("%s\n", errmsg);
        throwByName(env, ERRCLS, errmsg);
    }
    return closesocket;
}

extern "C" JNIEXPORT jint JNICALL
Java_dem_vac_androidnanomsg_NanoMsgJNI_connect(
        JNIEnv *env,
        jobject,
        jint socketid_,
        jstring ipadr_
) {
    int connectsocket = -1;

    //获取地址
    const char *ipadress = env->GetStringUTFChars(ipadr_, 0);
    const char *ipadrpre = "tcp://";

    int len = strlen(ipadress) + strlen(ipadrpre);
    char ipstr[len];
    strcpy(ipstr, ipadrpre);
    strcat(ipstr, ipadress);


    try {
        //连接服务器
        connectsocket = nn_connect(socketid_, ipstr);
        if (connectsocket < 0) {
            throw connectsocket;
        }
    } catch (int e) {
        char errmsg[100];
        sprintf(errmsg, "连接服务器失败，返回码：%d", e);
        LOGE("%s\n", errmsg);
        throwByName(env, ERRCLS, errmsg);
    }
    //释放资源
    env->ReleaseStringUTFChars(ipadr_, ipadress);
    return connectsocket;
}

extern "C" JNIEXPORT jint JNICALL
Java_dem_vac_androidnanomsg_NanoMsgJNI_send(
        JNIEnv *env,
        jobject,
        jint socketid_,
        jstring sendmsg_
) {
    int count = 0;

    //获取发送字符串
    const char *sendmsg = env->GetStringUTFChars(sendmsg_, 0);

    try {
        //计算发送字节长度
        int str_len = strlen(sendmsg);
        //发送数据
        count = nn_send(socketid_, sendmsg, static_cast<size_t>(str_len), 0);
        if (count < 0) {
            throw count;
        }
    } catch (int e) {
        char errmsg[100];
        sprintf(errmsg, "程序发送数据失败！返回码：%d", e);
        LOGE("%s\n", errmsg);
        throwByName(env, ERRCLS, errmsg);
    }
    //释放资源
    env->ReleaseStringUTFChars(sendmsg_, sendmsg);
    return count;
}

extern "C" JNIEXPORT jstring JNICALL
Java_dem_vac_androidnanomsg_NanoMsgJNI_recv(
        JNIEnv *env,
        jobject,
        jint socketid_
) {
    jstring recvmsg;
    try {
        int nbytes = 0;
        //定义一个空指针
        void *buf = NULL;
        //接收数据
        nbytes = nn_recv(socketid_, &buf, NN_MSG, 0);
        if (nbytes < 0) {
            throw nbytes;
        } else {
            recvmsg = env->NewStringUTF((char *) buf);
            nn_freemsg(buf);
        }

    } catch (int e) {
        char errmsg[100];
        sprintf(errmsg, "接收数据失败！返回码：%d", e);
        LOGE("%s\n", errmsg);
        throwByName(env, ERRCLS, errmsg);
        recvmsg = env->NewStringUTF("");
    }
    return recvmsg;
}