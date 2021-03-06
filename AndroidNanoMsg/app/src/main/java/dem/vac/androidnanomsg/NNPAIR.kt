package dem.vac.androidnanomsg

/**
 * 作者：Vaccae
 * 邮箱：3657447@qq.com
 * 创建时间：2020-02-14 10:56
 * 功能模块说明：
 */
class NNPAIR {
    private var nnjni:NanoMsgJNI = NanoMsgJNI()
    private var pairsocket: Int = -1

   init {
       if (pairsocket == -1) {
           pairsocket = nnjni.init("PAIR")
       }
   }

    //PAIR连接
    fun connect(ipadr: String): Boolean {
        val res = nnjni.connect(pairsocket, ipadr)
        return res >= 0
    }

    //发送数据
    fun send(sendmsg:String):Int{
        return nnjni.send(pairsocket,sendmsg);
    }

    //接收数据
    fun recv():String{
        return nnjni.recv(pairsocket)
    }
}