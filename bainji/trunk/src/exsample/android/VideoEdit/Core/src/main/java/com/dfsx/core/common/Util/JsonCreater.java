package com.dfsx.core.common.Util;

import android.text.TextUtils;
import com.dfsx.core.exception.ApiException;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by liuwb on 2016/8/30.
 */
public class JsonCreater {

    public static JSONObject StringArraryToJson(String... params) throws JSONException {
        JSONObject object = new JSONObject();

        String[] jsonParams = params;
        if (jsonParams != null) {
            for (int i = 0; i < jsonParams.length - 1; i += 2) {
                object.put(jsonParams[i], jsonParams[i + 1]);
            }
        }
        return object;
    }

    public static String getErrorMsgFromApi(String error) {
        if (error != null && !TextUtils.isEmpty(error)) {
            int last = error.lastIndexOf("{");
            if (last != -1) {
                error = error.substring(last);
                try {
                    JSONObject ob = jsonParseString(error);
                    if (ob != null)
                        error = ob.optString("message");
                } catch (ApiException e) {
                    e.printStackTrace();
                }
            }
        }
        return error;
    }

    public static String getErrorMsg(String error) {
        if (error != null && !TextUtils.isEmpty(error)) {
            int last = error.indexOf(":");
            if (last != -1) {
                error = error.substring(last);
            }
        }
        return error;
    }

    public static JSONObject jsonParseString(String response) throws ApiException {

        JSONObject jsonObject = null;
        try {
            response = response.toString().trim();
            if (response.startsWith("[")) {
                jsonObject = new JSONObject();
                jsonObject.put("result", new JSONArray(response));
            } else if (response.startsWith("{")) {
                jsonObject = new JSONObject(response);
            } else {
                int index = response.indexOf("[");
                if (index == -1) {
                    index = response.indexOf("{");
                }
                if (index != 0) {
                    response = response.substring(index);
                }
                jsonObject = new JSONObject(response);
            }
        } catch (Exception e) {
            jsonObject = new JSONObject();
            //throw ApiException.exception(e);
        }
        return jsonObject;
    }
}
