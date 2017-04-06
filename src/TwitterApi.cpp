/*
   Copyright (c) 2017 Brian Lough. All right reserved.

   TwitterApi - An Arduino wrapper for the Twitter API

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */


#include "TwitterApi.h"

TwitterApi::TwitterApi(const char * consumerKey, const char * consumerSecret, Client &client)	{
	_consumerKey = consumerKey;
	_consumerSecret = consumerSecret;
	this->client = &client;
}

bool TwitterApi::encodeBearerCredientials() {
	char toencodeLen = strlen(_consumerKey)+strlen(_consumerSecret)+1;
	char *toencode = new char[toencodeLen];
	if(toencode == NULL)
	  return false;
	_encodedBearerCreds = new char[base64_encode_expected_len(toencodeLen)+1];
	if(_encodedBearerCreds == NULL)
	  return false;

	// They say to URL encode the key and secret for future proofing, but I am a rebel
	encodeBearerCredientials();
  sprintf(toencode, "%s:%s", _consumerKey, _consumerSecret);
  if(base64_encode_chars(toencode, toencodeLen, _encodedBearerCreds) > 0 ){
		Serial.println(String(_encodedBearerCreds));
		return true;
  }

	return false;
}

void TwitterApi::setBearerToken(String bearerToken){
	_bearerToken = bearerToken;
}

bool TwitterApi::updateBearerToken() {
	char body[200];

	bool finishedHeaders = false;
	bool currentLineIsBlank = true;
	unsigned long now;
	bool avail;
	// Connect with twitter api over ssl
	Serial.println(".... trying server");
	if (client->connect(TW_API_HOST, TW_API_SSL_PORT)) {
		Serial.println(".... connected to server");
		String a="";
		char c;
		int ch_count=0;
		client->println("POST /oauth2/token HTTP/1.1");
		client->println("Host: " TW_API_HOST);
		client->println("User-Agent: arduino/1.0.0");
		client->println("Authorization: Basic " + String(_encodedBearerCreds));
		client->println("Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
		client->println("Content-Length: 29");
		client->println("");
		client->println("grant_type=client_credentials");
		client->println("");
		now=millis();
		avail=false;
		while (millis() - now < TW_API_TIMEOUT) {
			while (client->available()) {

				// Allow body to be parsed before finishing
				avail = finishedHeaders;
				char c = client->read();
				Serial.write(c);

				if(!finishedHeaders){
					if (currentLineIsBlank && c == '\n') {
						finishedHeaders = true;
					}
				} else {
					if (ch_count < maxMessageLength)  {
						body[ch_count] = c;
						ch_count++;
					}
				}

				if (c == '\n') {
					currentLineIsBlank = true;
				}else if (c != '\r') {
					currentLineIsBlank = false;
				}
			}
			if (avail) {
				DynamicJsonBuffer jsonBuffer;
				JsonObject& root = jsonBuffer.parseObject(body);
				if (root.success()) {
					if (root.containsKey("access_token")) {
						_bearerToken = root["access_token"].as<String>();
						return true;
					}
				}
				Serial.println("Error with response:");
				Serial.println(body);
				break;
			}
		}
	}
	return false;
}

String TwitterApi::sendGetToTwitter(String command) {
	String body;
	String headers;
	bool finishedHeaders = false;
	bool currentLineIsBlank = true;
	unsigned long now;
	bool avail;
	// Connect with twitter api over ssl
	if (client->connect(TW_API_HOST, TW_API_SSL_PORT)) {
		Serial.println(".... connected to server");
		String a="";
		char c;
		int ch_count=0;
		client->println("GET " + command +  " HTTP/1.1");
		client->println("Host: " TW_API_HOST);
		client->println("User-Agent: arduino/1.0.0");
		client->println("Authorization: Bearer " + _bearerToken);
		client->println("");
		now=millis();
		avail=false;
		while (millis() - now < TW_API_TIMEOUT) {
			while (client->available()) {

				// Allow body to be parsed before finishing
				avail = finishedHeaders;
				char c = client->read();
				//Serial.write(c);

				if(!finishedHeaders){
					if (currentLineIsBlank && c == '\n') {
						finishedHeaders = true;
						Serial.println(headers);
					}
					else{
						headers = headers + c;

					}
				} else {
					if (ch_count < maxMessageLength)  {
						body=body+c;
						ch_count++;
					}
				}

				if (c == '\n') {
					currentLineIsBlank = true;
				}else if (c != '\r') {
					currentLineIsBlank = false;
				}
			}
			if (avail) {
				Serial.println("Body:");
				Serial.println(body);
				Serial.println("END");
				break;
			}
		}
	}

	return body;
}

String TwitterApi::getUserStatistics(String screenName){
	String command="/1.1/users/show.json?screen_name="+screenName;
	//Serial.println(command);
	return sendGetToTwitter(command);  //recieve reply from twitter
}
