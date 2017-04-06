/*
Copyright (c) 2017 Brian Lough. All right reserved.

TwitterApi - An Arduino wrapper for the YouTube API

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


#ifndef TwitterApi_h
#define TwitterApi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>
#include <libb64/cencode.h>

#define TW_API_HOST "api.twitter.com"
#define TW_API_SSL_PORT 443
#define TW_API_TIMEOUT 1500


struct channelStatistics{
  long viewCount;
  long commentCount;
  long subscriberCount;
  bool hiddenSubscriberCount;
  long videoCount;
};

class TwitterApi
{
  public:
    TwitterApi (const char * consumerKey, const char * consumerSecret, Client &client);
    bool encodeBearerCredientials();
    bool updateBearerToken();
    String sendGetToTwitter(String command);
    String getUserStatistics(String screenName);
    void setBearerToken(String bearerToken);

  private:
    const char * _consumerKey;
    const char * _consumerSecret;
    Client *client;
    char *_encodedBearerCreds;
    String _bearerToken;
    const int maxMessageLength = 10000;
};

#endif
