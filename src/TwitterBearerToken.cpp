/*
   Copyright (c) 2017 Brian Lough. All right reserved.

   Get the twitter bearer token from consumerKey and consumerSecret

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


#include "TwitterBearerToken.h"

TwitterBearerToken::TwitterBearerToken(Client &client)	{
	this->client = &client;
}

char *  TwitterBearerToken::getEncodedBearerCredentials(const char * consumerKey, const char * consumerSecret) {
	char toencodeLen = strlen(consumerKey)+strlen(consumerSecret)+1;
	char *toencode = new char[toencodeLen];
	if(toencode == NULL)
  {
    return NULL;
  }
	char *encodedCredentials = new char[base64_encode_expected_len(toencodeLen)+1];
	if(encodedCredentials == NULL)
  {
    return NULL;
  }

	// They say to URL encode the key and secret for future proofing, but I am a rebel
  sprintf(toencode, "%s:%s", consumerKey, consumerSecret);
  Serial.println(toencode);
  if(base64_encode_chars(toencode, toencodeLen, encodedCredentials) > 0 ){
		Serial.println(String(encodedCredentials));
		return encodedCredentials;
  }

	return NULL;
}

String TwitterBearerToken::getNewToken(const char * consumerKey, const char * consumerSecret) {
	char body[200];

  String encodedCredentials = String(getEncodedBearerCredentials(consumerKey, consumerSecret));
	bool finishedHeaders = false;
	bool currentLineIsBlank = true;
	unsigned long now;
	bool avail;
	// Connect with twitter api over ssl
	Serial.println(".... trying server");
	if (client->connect(TWB_API_HOST, TWB_API_SSL_PORT)) {
		Serial.println(".... connected to server for token");
		String a="";
		char c;
		int ch_count=0;
		client->println("POST /oauth2/token HTTP/1.1");
		client->println("Host: " TWB_API_HOST);
		client->println("User-Agent: arduino/1.0.0");
		client->println("Authorization: Basic " + encodedCredentials);
    client->println("Accept: */*");
		client->println("Content-Length: 29");
    client->println("Content-Type: application/x-www-form-urlencoded");
		client->println();
		client->println("grant_type=client_credentials");
		now=millis();
		avail=false;
		while (millis() - now < TWB_API_TIMEOUT) {
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
						token = root["access_token"].as<String>();
						return token;
					}
				}
				Serial.println("Error with response:");
				Serial.println(body);
				break;
			}
		}
	}
	return "";
}
