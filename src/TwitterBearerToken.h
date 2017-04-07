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


#ifndef TwitterBearerToken_h
#define TwitterBearerToken_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>
#include <libb64/cencode.h>

#define TWB_API_HOST "api.twitter.com"
#define TWB_API_SSL_PORT 443
#define TWB_API_TIMEOUT 1500

class TwitterBearerToken
{
  public:
    TwitterBearerToken(Client &client);
    String getNewToken(const char * consumerKey, const char * consumerSecret);
    String token;

  private:
    Client *client;
    char * getEncodedBearerCredentials(const char * consumerKey, const char * consumerSecret);
    const int maxMessageLength = 1000;
};

#endif
