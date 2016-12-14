package com.phoneToPc;

/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */


import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.conn.params.ConnManagerParams;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.protocol.HTTP;

import com.CommandParser.CommandE;


import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;


/**
 * Provides utility methods for communicating with the server.
 */
final public class Http {
    /** The tag used to log to adb console. */
    private static final String TAG = "NetworkUtilities";
   
    /** Timeout (in ms) we specify for each http request */
    public static final int HTTP_REQUEST_TIMEOUT_MS = 30 * 1000;
    
    
	public static final String HTTP_CODE_SCAN_ADDRESS	= "http://10.4.64.40/phone/codescan/";


    private Http() {
    }

    /**
     * Configures the httpClient to connect to the URL provided.
     */
    public static HttpClient getHttpClient() {
        HttpClient httpClient = new DefaultHttpClient();
        final HttpParams params = httpClient.getParams();
        HttpConnectionParams.setConnectionTimeout(params, HTTP_REQUEST_TIMEOUT_MS);
        HttpConnectionParams.setSoTimeout(params, HTTP_REQUEST_TIMEOUT_MS);
        ConnManagerParams.setTimeout(params, HTTP_REQUEST_TIMEOUT_MS);
        return httpClient;
    }

    /**
     * Connects to the server,
     * @return String request response (or null)
     */
    
    //CommandE 0 位置必须是URL 地址
    public static String httpReq( CommandE command ) {

        final HttpResponse resp;
        String ResString = null;
        final ArrayList<NameValuePair> params = new ArrayList<NameValuePair>();
        assert(command.GetPropertyNum() >= 2);
        String url = command.GetProperty(1).GetPropertyContext();
    	Log.d("HTTP", "httpReq : " );
        for( int i = 0 ; i < command.GetPropertyNum() ; i++ )
        {
        	Log.d("HTTP", command.GetProperty(i).GetPropertyName() + " " + command.GetProperty(i).GetPropertyContext() );
        }
        
        for( int i = 2 ; i < command.GetPropertyNum() ; i++ )
        {
        	params.add(new BasicNameValuePair(command.GetProperty(i).GetPropertyName(), command.GetProperty(i).GetPropertyContext()));
        }
        
        final HttpEntity entity;
        try {
            entity = new UrlEncodedFormEntity(params,HTTP.UTF_8);
        } catch (final UnsupportedEncodingException e) {
            // this should never happen.
            throw new IllegalStateException(e);
        }
        Log.i(TAG, "connect url = " + url);
        final HttpPost post = new HttpPost(url);
        post.addHeader(entity.getContentType());
        post.setEntity(entity);
        try {
            resp = getHttpClient().execute(post);
            
            
            if (resp.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {
                InputStream istream = (resp.getEntity() != null) ? resp.getEntity().getContent()
                        : null;
                if (istream != null) {
                    BufferedReader ireader = new BufferedReader(new InputStreamReader(istream));
                    ResString = ireader.readLine().trim();
					Log.e(TAG, "Http Resp = " + ResString );
                }
            }
            else
            {
            	Log.e(TAG, "getStatusCode = " + resp.getStatusLine().getStatusCode() );
            }
           
        } catch (final IOException e) {
            Log.e(TAG, "getHttpClient().execute(post)", e);
            return null;
        } finally {
            Log.v(TAG, "getAuthtoken completing");
        }
        
        return ResString;
    }


}
