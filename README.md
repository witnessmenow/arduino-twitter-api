# DEPRACATED in favor of new library I am working on

https://github.com/witnessmenow/TweESP32


## arduino-twitter-api
A wrapper for the [Twitter API](https://dev.twitter.com/overview/api) for Arduino (works on ESP8266)

In the api FAQ twitter mentions that they do not expire bearer tokens, so once you do not generate a new one with the same Credentials it should continue working


## Getting API Key & Secret

- [Go to the following URL](https://apps.twitter.com/app/new) - sign in if you havent already.
- Fill in the fields, For website you can enter any webpage (e.g. http://google.com), and create your app
- Then click on the Keys & Tokens tab. Your Consumer Key and Consumer Secret will be there.


## Getting Bearer Token

I am hoping to make this a little more simple going forward, as we should be able to do this on the board directly, but here they are for now :)

- Get curl on your machine ([Windows: try this](http://callejoabel.blogspot.co.uk/2013/09/making-curl-work-on-windows-7.html) it shoud already be installed on mac and linux)
- Make the following request using the key and token from the above section
```
curl -u "CONSUMER_KEY:CONSUMER_SECRET" --data 'grant_type=client_credentials' 'https://api.twitter.com/oauth2/token'
```
- The response will contain a bearer token (it is the big long one!), use this with the setBearerToken [as shown in the example](https://github.com/witnessmenow/arduino-twitter-api/blob/master/examples/esp8266/settingBearerTokenInSketch/settingBearerTokenInSketch.ino).
