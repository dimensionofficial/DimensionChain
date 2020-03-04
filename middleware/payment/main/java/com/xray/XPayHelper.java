package com.xray;

import android.content.Context;
import android.os.Looper;
import android.util.Log;
import android.widget.EditText;
import android.widget.Toast;

import com.alipay.sdk.util.JsonUtils;
import com.alipay.sdk.util.PayHelper;
import com.mykey.sdk.MYKEYSdk;
import com.mykey.sdk.common.util.JsonUtil;
import com.mykey.sdk.connect.scheme.callback.MYKEYWalletCallback;
import com.mykey.sdk.entity.client.request.AuthorizeRequest;
import com.mykey.sdk.entity.client.request.InitRequest;
import com.mykey.sdk.entity.client.request.InitSimpleRequest;
import com.mykey.sdk.entity.client.request.TransferRequest;
import com.yyjia.sdk.center.GMcenter;
import com.yyjia.sdk.data.ConfigInfo;
import com.yyjia.sdk.util.Constants;
import com.yyjia.sdk.util.MarketAPI;
import com.yyjia.sdk.util.ProgressDialogUtil;
import com.yyjia.sdk.util.Utils;
import com.yyjia.sdk.window.LoginPopWindow;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class XPayHelper {

    private ExecutorService threadPool = null;
    private Context mContext;
    private GMcenter mCenter = null;
    private volatile static XPayHelper mPayHelper = null;
    private XPayHelperListener mPayHelperListener = null;
    private MYKEYSdk mykeySdk = null;
    private XPayConfig mXPayConfig = null;

    private XPayHelper(Context mContext, GMcenter mCenter, XPayConfig mXPayConfig) {
        this.mContext = mContext;
        this.mCenter = mCenter;
        threadPool = Executors.newCachedThreadPool();
        if (mXPayConfig == null) {
            this.mXPayConfig = new XPayConfig();
        } else {
            this.mXPayConfig = mXPayConfig;
        }
        initMyKey();
    }

    private void initMyKey() {
        mykeySdk = MYKEYSdk.getInstance();
        mykeySdk.init(new InitRequest().setAppKey(mXPayConfig.getAppKey())
                .setDappName(mXPayConfig.getAppName())
                .setUuid(mXPayConfig.getUserId())
                .setDappIcon(mXPayConfig.getAppIcon())
                .setCallback(mXPayConfig.getAppBackUrl())
                .setDisableInstall(false)
                .setContractPromptFree(true)
                .setContext(mContext));
        //认证
        InitSimpleRequest initSimpleRequest = new InitSimpleRequest().setDappName(mXPayConfig.getAppName())
                .setDappIcon(mXPayConfig.getAppIcon())
                .setCallback(mXPayConfig.getAppBackUrl())
                .setDisableInstall(false)
                .setContractPromptFree(true)
                .setContext(mContext);
        mykeySdk.initSimple(initSimpleRequest);
    }

    public static XPayHelper getInstance(Context mContext, GMcenter mCenter, XPayConfig mXPayConfig) {
        if (mPayHelper == null) {
            synchronized (PayHelper.class) {
                if (mPayHelper == null) {
                    mPayHelper = new XPayHelper(mContext, mCenter, mXPayConfig);
                }
            }
        }
        return mPayHelper;
    }

    public void setPayHelperLister(XPayHelperListener mPayHelperListener) {
        this.mPayHelperListener = mPayHelperListener;
    }


    public void xPay(
            final String uid,
            final String username,
            final String realamount,
            final String payfrom,
            final String paytitle,
            final String cporderid,
            final String paytype,
            final String to,
            final String symbol,
            final String contractName,
            final String decimal) {

        if (mPayHelperListener == null) {
            throw new RuntimeException("PayHelper must be set PayHelperListener");
        }

        final Map map = new HashMap();
        map.put("uid", uid);
        map.put("username", username);
        map.put("realamount", realamount);
        map.put("payfrom", payfrom);
        map.put("paytitle", paytitle);
        map.put("cporderid", cporderid);
        map.put("paytype", paytype);
        map.put("to", to);
        map.put("symbol", symbol);
        map.put("contractName", contractName);
        map.put("decimal", decimal);

        if (threadPool != null) {
            threadPool.execute(new Runnable() {
                @Override
                public void run() {
                    postMessageToServer(map);
                }
            });
        }
    }

    /**
     * Float money=0.01f;
     * String productname=“商品名称”;
     * String serverId =”服务器编号”;
     * String charId=”角色编号”;
     * String cporderId=”CP订单号”;
     * String callbackInfo=”扩展信息 当充值成功会回传给游戏服务器”;
     * PayListener支付监听事件
     *
     * @return
     */
    public void xPay(final String uid,
                     final String username,
                     final String appid,
                     final String serverid,
                     final String amount,
                     final String realamount,
                     final String remark,
                     final String payfrom,
                     final String paytitle,
                     final String ip,
                     final String location,
                     final String dateline,
                     final String callbackinfo,
                     final String cporderid,
                     final String charid,
                     final String paytype,
                     final String paymoney,
                     final String accountbalance,
                     final String daibi,
                     final String touid,
                     final String tousername,
                     final String ostype,
                     final String discount,
                     final String iospaytest,
                     final String from,
                     final String to,
                     final String symbol,
                     final String contractName,
                     final String decimal,
                     final String memo,
                     final String info,
                     final String callbackUrl
    ) {

        if (mPayHelperListener == null) {
            throw new RuntimeException("PayHelper must be set PayHelperListener");
        }

        final Map<String, String> map = new HashMap();
        map.put("uid", uid);
        map.put("username", username);
        map.put("appid", appid);
        map.put("serverid", serverid);
        map.put("amount", amount);
        map.put("realamount", realamount);
        map.put("remark", remark);
        map.put("payfrom", payfrom);
        map.put("paytitle", paytitle);
        map.put("ip", ip);
        map.put("location", location);
        map.put("dateline", dateline);
        map.put("callbackinfo", callbackinfo);
        map.put("cporderid", cporderid);
        map.put("charid", charid);
        map.put("paytype", paytype);
        map.put("paymoney", paymoney);
        map.put("accountbalance", accountbalance);
        map.put("daibi", daibi);
        map.put("touid", touid);
        map.put("tousername", tousername);
        map.put("ostype", ostype);
        map.put("discount", discount);
        map.put("iospaytest", iospaytest);
        map.put("from", from);
        map.put("to", to);
        map.put("symbol", symbol);
        map.put("contractName", contractName);
        map.put("decimal", decimal);
        map.put("memo", memo);
        map.put("info", info);
        map.put("callbackUrl", callbackUrl);

        Log.e("params", map.toString());
        if (threadPool != null) {
            threadPool.execute(new Runnable() {
                @Override
                public void run() {
                    postMessageToServer(map);
                }
            });
        }

    }

    /**
     * 这次的需求，支付前发送信息到服务器
     * 参数根据后台接口再修改
     *
     * @param
     */
    private void postMessageToServer(Map map) {
        try {
            HttpURLConnection connection = getConnectionPost(mXPayConfig.getServerUrl());
            String params = convertStringParamter(map);
            OutputStream out = connection.getOutputStream();
            out.write(params.getBytes());
            out.flush();
            out.close();
            // 从连接中读取响应信息
            String msg = "";
            int code = connection.getResponseCode();
            if (code == 200) {
                BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
                String line;
                while ((line = reader.readLine()) != null) {
                    msg += line + "\n";
                }
                reader.close();

                Looper.prepare();
                payByMyKey(map);
                Looper.loop();
            } else {
                mPayHelperListener.onPayFaild("Request network failed");
            }

            Log.d("=============", msg);

            // 5. 断开连接
            connection.disconnect();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {

        }
    }

    /**
     * 认证
     * @param
     */
    public void myKeyLogin(final EditText editusername, final EditText editpassword, final Context activity, final ConfigInfo configInfo, final LoginPopWindow loginPopWindow) {
        AuthorizeRequest authorizeRequest = new AuthorizeRequest();

        MYKEYSdk.getInstance().authorize(authorizeRequest, new MYKEYWalletCallback() {
            @Override
            public void onSuccess(String dataJson) {
                String account="";
                // dataJson：{"protocol": "", "version": "", "dapp_key": "", "uuID": "", "public_key": "", "sign": "", "ref": "", "timestamp": "", "account": ""}
                try {
                    JSONObject jsonObject=new JSONObject(dataJson);
                     account =(String) jsonObject.get("account");
                } catch (JSONException e) {
                    e.printStackTrace();
                }
                String username = account;
                String password = account;
                String param = Constants.KEY_EMPTY;
                param = URLEncoder.encode(Utils.toEncode(Utils
                        .getJsonInfo(Constants.KEY_LOGIN,
                                activity, username, password,
                                configInfo)));
                ProgressDialogUtil.startLoad(activity, "");
                String jsonInfo = Utils.getJsonInfo(Constants.KEY_REG,
                        activity, username, password, configInfo);
                ProgressDialogUtil.startLoad(activity, "");
                MarketAPI.registerOne(activity, loginPopWindow,
                        URLEncoder.encode(Utils.toEncode(jsonInfo)));
                try {
                    Thread.sleep(200);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                editusername.setText(username);
                editpassword.setText(password);
                MarketAPI.checkLogin(activity, loginPopWindow,
                        param);
                // Toast.makeText(activity, param, Toast.LENGTH_LONG).show();
            }
            @Override
            public void onError(String payloadJson) {
                // payloadJson: {"errorCode":,"errorMsg":""}
                Toast.makeText(activity, "error，payloadJson:" + payloadJson, Toast.LENGTH_LONG).show();
            }

            @Override
            public void onCancel() {
                Toast.makeText(activity, "cancelled", Toast.LENGTH_LONG).show();
            }
        });
    }

    /**
     * from String 转账账户
     * to String 接受账户
     * amount String 转账数量
     * symbol String 币种 Symbol, e.g. "EOS"
     * contractName String 合约名称, e.g. "eosio.token"
     * decimal String 币种对应的小数位数
     * memo String 链上的MEMO备注
     * info String 备注信息，用于语义化该笔转账交易
     * orderId String 订单ID，dapp提供的订单ID，可为空 e.g. "20190606001"
     * callbackUrl（可选）String dapp server的回调url，上链成功会先回调dapp server,然后再唤醒移动端
     */
    private void payByMyKey(Map map) {

        String to = (String) map.get("to");
        Double amount = Double.valueOf((String) map.get("realamount"));
        String memo = (String) map.get("memo");
        String contractName = (String) map.get("contractName");
        String info = (String) map.get("info");
        String orderId = (String) map.get("cporderid");
        String symbol = (String) map.get("symbol");
        Integer decimal = Integer.valueOf((String) map.get("decimal"));
        //转账
        TransferRequest transferRequest = new TransferRequest()
//                .setFrom("maomao123521")
                .setTo(to)
                .setAmount(amount)
                .setMemo(memo)
                .setContractName(contractName)
                .setSymbol(symbol)
                .setInfo(info)
                .setDecimal(decimal)
                .setOrderId(orderId)
                .setCallBackUrl(mXPayConfig.getServerBackUrl());
        MYKEYSdk.getInstance().transfer(transferRequest, new MYKEYWalletCallback() {
            @Override
            public void onSuccess(String dataJson) {
                if (mPayHelperListener != null) {
                    mPayHelperListener.onPaySuccess(dataJson);
                }
            }

            @Override
            public void onError(String errorJson) {
                if (mPayHelperListener != null) {
                    mPayHelperListener.onPayFaild(errorJson);
                }
            }

            @Override
            public void onCancel() {
                if (mPayHelperListener != null) {
                    mPayHelperListener.onPayCancel();
                }
            }
        });
    }

    private String convertStringParamter(Map map) throws UnsupportedEncodingException {
        String body = "";
        StringBuffer stringBuffer = new StringBuffer();
        for (Object key : map.keySet()) {
            System.out.println("key= " + key + " and value= " + map.get(key));
            stringBuffer.append(key + "=" + URLEncoder.encode(String.valueOf(map.get(key)), "UTF-8") + "&");
        }
        body = stringBuffer.substring(0, stringBuffer.length() - 1);
        return body;
    }

    private HttpURLConnection getConnection(String httpUrl) throws Exception {
        URL url = new URL(httpUrl);
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");
        connection.setRequestProperty("Content-Type", "application/octet-stream");
        connection.setDoInput(true);
        connection.setRequestProperty("accept", "*/*");
        connection.setRequestProperty("Accept-Encoding", "identity");
        connection.setRequestProperty("Connection", "Keep-Alive");
        connection.setReadTimeout(30 * 1000);
        connection.setConnectTimeout(10 * 1000);
        connection.connect();
        return connection;
    }

    private HttpURLConnection getConnectionPost(String httpUrl) throws Exception {
        // 1. 获取访问地址URL
        URL url = new URL(httpUrl);
        // 2. 创建HttpURLConnection对象
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        // 3. 设置请求参数等
        // 请求方式
        connection.setRequestMethod("POST");
        connection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
        // 设置是否输出
        connection.setDoOutput(true);
        // 设置是否读入
        connection.setDoInput(true);
        // 设置是否使用缓存
        connection.setUseCaches(false);
        // 设置此 HttpURLConnection 实例是否应该自动执行 HTTP 重定向
        connection.setInstanceFollowRedirects(true);
        // 连接
        connection.connect();
        return connection;
    }

}
