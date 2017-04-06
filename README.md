# arduino-twitter-api
A wrapper for the [Twitter API](https://dev.twitter.com/overview/api) for Arduino (works on ESP8266)

WORK IN PROGRESS - API call works if you set the bearer token directly. It should be able to generate the bearer Credentials from the consumer key and secret and make a request to get the token but its not working at the moment.

In the api FAQ twitter mentions that they do not expire bearer tokens, so once you do not generate a new one with the same Credentials it should continue working


## Steps
Sign in to apps.twitter.com

Add number to your twitter account on twitter.com
