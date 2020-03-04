package com.yyjia.sdk.demo.haituidata;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import com.alibaba.fastjson.JSONObject;
import com.xray.XPayConfig;
import com.xray.XPayHelper;
import com.xray.XPayHelperListener;
import com.yyjia.sdk.center.GMcenter;
import com.yyjia.sdk.data.Information;
import com.yyjia.sdk.listener.InitListener;
import com.yyjia.sdk.listener.LoginListener;
import com.yyjia.sdk.util.Utils;


public class MainActivity extends Activity implements XPayHelperListener {
    private GMcenter mCenter = null;
    private Button loginBtn, payOneBtn, payTwoBtn, btnLogin;
    private XPayConfig mXPayConfig = null;
    private XPayHelper mPayHelper = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();

        if (mCenter == null) {
            mCenter = GMcenter.getInstance(MainActivity.this);

            // 登录登出监听器
            mCenter.setLoginListener(new LoginListener() {
                // 登录监听方法
                @Override
                public void loginSuccessed(String code) {
                    if (code == Information.LOGIN_SUSECCEDS) {
                        // 登录成功 游戏(服务端)请求 登录验证
                        ToastUtil.showShortToast(MainActivity.this, "LOGIN SUCCESS");
                    } else {
                        ToastUtil.showShortToast(MainActivity.this, "LOGIN FAIL");
                    }
                }

                // 登出监听方法
                public void logoutSuccessed(String code) {
                    if (code == Information.LOGOUT_SUSECCED) {
                        // 账号 退出 游戏需要重启到 登录界面
                        ToastUtil.showShortToast(MainActivity.this, "EXIT SUCCESS");
                    } else {
                        ToastUtil.showShortToast(MainActivity.this, "EXIT FAIL");
                    }
                }

                @Override
                public void logcancelSuccessed(String code) {
                    if (code == Information.LOGCANCEL_SUSECCED) {
                        ToastUtil.showShortToast(MainActivity.this, "CACEL LOGIN");
                    }
                }
            });

            mCenter.setInitListener(new InitListener() {
                @Override
                public void initSuccessed(String code) {

                    if (code == Information.INITSUCCESSEDS) {
                        mCenter.checkLogin();
                    }
                }
            });

        }
        Utils.E("onCreate");
        mCenter.onCreate(MainActivity.this);
        GMcenter.getConfigInfo().setKEFU("service@magicoranges.com");

        mXPayConfig = new XPayConfig();
        mPayHelper = XPayHelper.getInstance(this, mCenter, mXPayConfig);
        mPayHelper.setPayHelperLister(this);
    }


    private void initView() {
        loginBtn = (Button) findViewById(R.id.logout);
        payOneBtn = (Button) findViewById(R.id.btn_toPay_oneyuan);
        payTwoBtn = (Button) findViewById(R.id.btn_toPay_twoyuan);
        btnLogin = (Button) findViewById(R.id.btnLogin);
        loginBtn.setOnClickListener(onClick);
        payOneBtn.setOnClickListener(onClick);
        payTwoBtn.setOnClickListener(onClick);
        btnLogin.setOnClickListener(onClick);
    }

    private View.OnClickListener onClick = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.btnLogin:
                    mCenter.checkLogin();
                    break;
                case R.id.btn_toPay_oneyuan:
                    break;
                case R.id.btn_toPay_twoyuan:
              double number=Math.random()*100;
                    mPayHelper.xPay(
                            "1112",
                            "HT7777777",
                            "60",
                            "7777777",
                            "0.0001",
                            "0.0001",
                            "www.baidu.com",
                            "1",
                            "XXXX",
                            "192.168.1.1",
                            "",
                            "1882748395",
                            "123456",
                            number+"",
                            "90DEFGCFHHFFSDF",
                            "1",
                            "0.0001",
                            "0.00",
                            "1110.00",
                            "13",
                            "111",
                            "android",
                            "1.00",
                            "0",
                            "maomao123521",
                            "mteoshowlove",
                            "EOS",
                            "eosio.token",
                            "4",
                            number+"",
                            "测试info",
                            "callbackUrl"
                    );

                    break;
                case R.id.logout:
                    mCenter.logout();
                    break;
                default:
                    break;
            }
        }
    };


    private void ShowCode(String errorCode) {
        if (Config.CODE0.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "用户主动取消操作");
        } else if (Config.CODE1.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "操作成功");

        } else if (Config.CODE2.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "操作失败");

        } else if (Config.CODE10001.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "未知异常导致无法唤醒MYKEY");

        } else if (Config.CODE10002.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "MYKEY未安装");

        } else if (Config.CODE10003.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "MYKEY账户被冻结");

        } else if (Config.CODE10004.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "未初始化");

        } else if (Config.CODE10005.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "转账或合约方法调用上链超时");

        } else if (Config.CODE10006.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "已经绑定过，执行绑定操作时抛出");

        } else if (Config.CODE10007.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "未绑定，执行方法操作时抛出");

        } else if (Config.CODE10008.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "dapp已绑定，但是MYKEY未绑定，执行方法操作时抛出");

        } else if (Config.CODE10009.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "MYKEY已绑定，执行绑定操作时抛出");

        } else if (Config.CODE10010.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "dapp与MYKEY都已绑定，但是并不匹配");

        } else if (Config.CODE10011.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "MYKEY未注册，执行方法操作时抛出");

        } else if (Config.CODE10012.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "参数非法");

        } else if (Config.CODE10013.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "余额不足");

        } else if (Config.CODE10014.endsWith(errorCode)) {
            ToastUtil.showLongToast(MainActivity.this, "MYKEY账户不可用");

        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        mCenter.onActivityResult(requestCode, resultCode, data);
    }



    @Override
    public void finish() {
        super.finish();
        Utils.E("onFinish");
        exitGame();
        System.exit(0);
    }

    private void exitGame() {
        mCenter.exitGame();
        mCenter = null;
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {

    }

    @Override
    protected void onPause() {
        super.onPause();
        mCenter.onPause();
        Utils.E("onPause");
    }

    @Override
    protected void onStop() {
        super.onStop();
        mCenter.onStop();
        Utils.E("onStop");
    }

    @Override
    protected void onDestroy() {
        mCenter.onDestroy();
        super.onDestroy();
        Utils.E("onDestroy");
    }


    @Override
    protected void onResume() {
        super.onResume();
        mCenter.onResume();
        Utils.E("onResume");
    }

    @Override
    protected void onStart() {
        super.onStart();
        mCenter.onStart();
        Utils.E("onStart");
    }

    @Override
    protected void onRestart() {
        super.onRestart();
        mCenter.onRestart();
        Utils.E("onRestart");
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
    }


    @Override
    public void onPaySuccess(String successJson) {
        Log.e("successJson", successJson);
        Toast.makeText(this, successJson, Toast.LENGTH_LONG).show();
    }

    @Override
    public void onPayFaild(String failJson) {
        Log.e("failJson", failJson);
        Toast.makeText(this, failJson, Toast.LENGTH_LONG).show();

        JSONObject obj = JSONObject.parseObject(failJson);
        String errorCode = obj.getString("errorCode");
        ShowCode(errorCode);
    }

    @Override
    public void onPayCancel() {
        Log.e("cancel", "user cancel");
        Toast.makeText(this, "user cancel", Toast.LENGTH_LONG).show();
    }
}