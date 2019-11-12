package com.github.wkigen.RemoteDebugger;

import android.util.Log;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;

public class JdwpSocket {

    private static final String JDWPHandShake = "JDWP-Handshake";
    private static final int MAX_CONNECT_COUNT = 10;

    private String ip = Constant.localJdwpIp;
    private int port = Constant.localJdwpPort;
    private boolean isRunning = false;
    private boolean firstWrite = false;
    private boolean isConnecting = false;

    public JdwpSocket(){
    }

    public JdwpSocket(String ip,int port){
        this.ip = ip;
        this.port = port;
    }

    public void connect(){
        try {

            isRunning = true;
            firstWrite = true;

            SocketChannel channel = SocketChannel.open();
            channel.configureBlocking(false);
            InetSocketAddress jdwpAddress = new InetSocketAddress(ip, port);
            channel.connect(jdwpAddress);

            int tryConnectCount = 0;
            while (!channel.finishConnect()){
                tryConnectCount++;
                if (tryConnectCount > MAX_CONNECT_COUNT){
                    return;
                }
            }

            isConnecting = true;

            Selector selector = Selector.open();
            channel.register(selector,  SelectionKey.OP_READ | SelectionKey.OP_WRITE);

            while (isRunning){
                int readyChannels = selector.select();
                if (0 == readyChannels)
                    continue;

                Set selectKeys = selector.selectedKeys();
                Iterator keyIterator = selectKeys.iterator();
                while (keyIterator.hasNext()) {
                    SelectionKey selectionKey = (SelectionKey) keyIterator.next();

                    if (selectionKey.isConnectable()){

                    }else if (selectionKey.isReadable()){
                        SocketChannel readChannel = (SocketChannel)selectionKey.channel();
                        ByteBuffer bytebuffer = ByteBuffer.allocateDirect(100);
                        int len = readChannel.read(bytebuffer);
                        bytebuffer.flip();
                        byte[] recv = new byte[64];
                        bytebuffer.get(recv, 0, len);
                        Log.e(Constant.LOGTAG,"读取到： " + new String(recv).trim());
                    }else if (selectionKey.isWritable()){
                        if (firstWrite){
                            firstWrite = false;

                            ByteBuffer bufferClient = ByteBuffer.allocate(100);
                            bufferClient.put((JDWPHandShake).getBytes());
                            bufferClient.flip();
                            channel.write(bufferClient);

                        }else {

                        }
                    }
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public boolean isConnecting(){
        return isConnecting;
    }

    public void close(){
        isConnecting = false;
        isRunning = false;
    }

}
