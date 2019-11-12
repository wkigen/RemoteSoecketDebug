package com.github.wkigen.RemoteDebugger;

import android.content.Context;

public class Debugger {

    private static Debugger debugger;

    static {
        System.loadLibrary("remotesocket");
    }

    private Debugger(){ }

    public static Debugger getInstance(){
        if (debugger == null){
            synchronized (Debugger.class){
                if (debugger == null){
                    debugger = new Debugger();
                }
            }
        }
        return debugger;
    }

    public void open(){
        jswpSocketDebugOpen();
    }


    public void close(){

    }





    private static native void jswpSocketDebugOpen();
}
