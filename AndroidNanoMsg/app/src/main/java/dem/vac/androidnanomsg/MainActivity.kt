package dem.vac.androidnanomsg

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Adapter
import android.widget.ArrayAdapter
import android.widget.SpinnerAdapter
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    private var nanotype = arrayOf("PAIR", "REQREP", "PUBSUB", "SURVEY")

    private var nnpair: NNPAIR? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        var adapter = ArrayAdapter(
            this,
            android.R.layout.simple_list_item_1, nanotype
        )
        spnnanotype.adapter = adapter

        btnConnent.setOnClickListener {
            if (nnpair == null) {
                nnpair = NNPAIR()
            }
            nnpair.let {
                if (it?.connect(edtipadr.text.toString())!!) {
                    tvmsg.text = "PAIR连接成功！\r\n"
                } else {
                    tvmsg.text = "PAIR连接失败！\r\n"
                }
            }
        }


        btnSend.setOnClickListener {

            nnpair.let {
                try {
                    //发送数据
                    it?.send(edtinput.text.toString())
                    //延时50毫秒
                    Thread.sleep(50)
                    //接收数据
                    val recvmsg = it?.recv()
                    tvmsg.append(recvmsg + "\r\n")
                } catch (e: IllegalArgumentException) {
                    tvmsg.append(e.message.toString() + "\r\n")
                }
            }
        }
    }


}
