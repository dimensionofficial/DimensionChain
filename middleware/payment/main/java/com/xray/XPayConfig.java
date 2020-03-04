package com.xray;

import java.util.UUID;

public class XPayConfig {
    private String serverUrl = "http://128.1.136.xx:20000/backstage/inputpayinfo";
    private String serverBackUrl = "http://128.1.136.xx:20000/backstage/inputpayresult";
    private String appKey = "XPay";
    private String appName = "XPayDesign";
    private String appIcon = "https://bihu.com/favicon.ico";
    private UUID userId = UUID.randomUUID();
    private String appBackUrl = "simple://con.mykey.simplewallet/custompatx";

    private String payToAccount = "mteoshowlove";

    public String getServerUrl() {
        return serverUrl;
    }

    public void setServerUrl(String serverUrl) {
        this.serverUrl = serverUrl;
    }

    public String getServerBackUrl() {
        return serverBackUrl;
    }

    public void setServerBackUrl(String serverBackUrl) {
        this.serverBackUrl = serverBackUrl;
    }

    public String getAppKey() {
        return appKey;
    }

    public void setAppKey(String appKey) {
        this.appKey = appKey;
    }

    public String getAppName() {
        return appName;
    }

    public void setAppName(String appName) {
        this.appName = appName;
    }

    public String getAppIcon() {
        return appIcon;
    }

    public void setAppIcon(String appIcon) {
        this.appIcon = appIcon;
    }

    public UUID getUserId() {
        return userId;
    }

    public void setUserId(UUID userId) {
        this.userId = userId;
    }

    public String getAppBackUrl() {
        return appBackUrl;
    }

    public void setAppBackUrl(String appBackUrl) {
        this.appBackUrl = appBackUrl;
    }

    public String getPayToAccount() {
        return payToAccount;
    }

    public void setPayToAccount(String payToAccount) {
        this.payToAccount = payToAccount;
    }

}
