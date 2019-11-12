package com.github.wkigenF.remotesoecketdebug;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.github.wkigen.RemoteDebugger.Debugger;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Enumeration;

public class MainActivity extends AppCompatActivity {

    private TextView ipTv;
    private Button button;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ipTv = findViewById(R.id.tv_ip);
        button = findViewById(R.id.open_debug);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ipTv.setText(getIpAddress(MainActivity.this));

                Debugger.getInstance().open();
            }
        });
    }


    public String getIpAddress(Context context){
        try {
            ConnectivityManager connectivityManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
            if (connectivityManager != null) {
                NetworkInfo info = connectivityManager.getActiveNetworkInfo();
                if (info != null && info.isConnected()) {
                    if (info.getState() == NetworkInfo.State.CONNECTED) {
                        NetworkInfo activeNetInfo = connectivityManager.getActiveNetworkInfo();
                        if (activeNetInfo != null){
                            if (activeNetInfo.getType() == ConnectivityManager.TYPE_WIFI) {
                                WifiManager wifiManager = (WifiManager) context
                                        .getSystemService(Context.WIFI_SERVICE);
                                WifiInfo wifiInfo = wifiManager.getConnectionInfo();
                                int i = wifiInfo.getIpAddress();
                                return int2ip(i);
                            }else if (activeNetInfo.getType() == ConnectivityManager.TYPE_MOBILE){
                                for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements(); ) {
                                    NetworkInterface intf = en.nextElement();
                                    for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements(); ) {
                                        InetAddress inetAddress = enumIpAddr.nextElement();
                                        if (!inetAddress.isLoopbackAddress()) {
                                            return inetAddress.getHostAddress().toString();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } catch (Exception e) {
            return "";
        }
        return "";
    }


    public static String int2ip(int ipInt) {
        StringBuilder sb = new StringBuilder();
        sb.append(ipInt & 0xFF).append(".");
        sb.append((ipInt >> 8) & 0xFF).append(".");
        sb.append((ipInt >> 16) & 0xFF).append(".");
        sb.append((ipInt >> 24) & 0xFF);
        return sb.toString();
    }
}
