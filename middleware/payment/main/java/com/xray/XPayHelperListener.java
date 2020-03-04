package com.xray;

public interface XPayHelperListener {
    void onPaySuccess(String successJson);
    void onPayFaild(String failJson);
    void onPayCancel();
}
