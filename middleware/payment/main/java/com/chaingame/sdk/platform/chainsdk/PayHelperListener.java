package com.yyjia.sdk.demo.haituidata;

public interface PayHelperListener {
    void onPaySuccess(String code, String cporderid);
    void onPayFaild(String faildCode);
    void onPayBack();
}
