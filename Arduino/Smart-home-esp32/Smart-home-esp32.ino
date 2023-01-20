/*
// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyANa7DTJhWLqkP0f_ZW728w1i34fquIjkc",
  authDomain: "smart-home-e56a9.firebaseapp.com",
  databaseURL: "https://smart-home-e56a9-default-rtdb.europe-west1.firebasedatabase.app",
  projectId: "smart-home-e56a9",
  storageBucket: "smart-home-e56a9.appspot.com",
  messagingSenderId: "237676998781",
  appId: "1:237676998781:web:e2cf27c6c962b42f530114"
};
*/

#include "DHT.h"
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"


// Insert Firebase project API Key
#define API_KEY "AIzaSyANa7DTJhWLqkP0f_ZW728w1i34fquIjkc"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://smart-home-e56a9-default-rtdb.europe-west1.firebasedatabase.app" 

#define WIFI_SSID "WL500gPc"
#define WIFI_PASS "Maco070765"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseData fbdo_write;
FirebaseAuth auth;
FirebaseConfig config;

#define TEMP_PATH "/home/temp"
#define MODE_PATH "/home/mode"
#define HUM_PATH "/home/hum"
#define PIR_PATH "/home/pir"
#define PLUG_PATH "/home/plug"

#define DHTPIN 4        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

#define PIR_1_PIN 27

#define PLUG_PIN 12
#define LAMP_PIN 13


volatile bool motion_detected = false;
volatile unsigned long last_visible_motion_timestamp = 0;

enum Mode
{
    AUTO,
    MANUAL
};

Mode mode = Mode::AUTO;

#define STATE_ARRAY_PLUG_IDX 0
#define STATE_ARRAY_LAMP_IDX 1

bool state_array[2] = { 0,0 };

bool state_array_always_on[2] = { 0,0 };


// Callback function that runs on database changes
void streamCallback(FirebaseStream data)
{
    Serial.println("Firebase Stream callback function (DATABASE WAS MODIFIED");


    if (strcmp(data.dataPath().c_str(), "/plug") == 0)
    {
        String state = data.stringData();
        Serial.print("Database event: Plug value was modified. Actual value:"); Serial.println(state);

        if (state == "OFF")
        {
            state_array[STATE_ARRAY_PLUG_IDX] = 0;
            digitalWrite(PLUG_PIN, HIGH);
        }
        else if (state == "ON")
        {
            state_array[STATE_ARRAY_PLUG_IDX] = 1;
            digitalWrite(PLUG_PIN, LOW);
        }
    }

    if (strcmp(data.dataPath().c_str(), "/lamp") == 0)
    {
        String state = data.stringData();
        Serial.print("Database event: Home lamp was modified. Actual value:"); Serial.println(state);

        if (state == "OFF")
        {
            state_array[STATE_ARRAY_LAMP_IDX] = 0;
            digitalWrite(LAMP_PIN, HIGH);
        }
        else if (state == "ON")
        {
            state_array[STATE_ARRAY_LAMP_IDX] = 1;
            digitalWrite(LAMP_PIN, LOW);
        }
    }

    if (strcmp(data.dataPath().c_str(), "/mode") == 0)
    {
        String state = data.stringData();
        Serial.print("Database event: Home lamp was modified. Actual value:"); Serial.println(state);

        if (state == "AUTO")
        {
            mode = Mode::AUTO;
        }
        else if (state == "MANUAL")
        {
            last_visible_motion_timestamp = millis();
            mode = Mode::MANUAL;
        }
    }

    if (strcmp(data.dataPath().c_str(), "/auto/lamp") == 0)
    {
        String state = data.stringData();
        Serial.print("Database event: Home lamp was modified. Actual value:"); Serial.println(state);

        if (state == "AON")
        {
            state_array_always_on[STATE_ARRAY_LAMP_IDX] = 1;
        }
        else
        {
            state_array_always_on[STATE_ARRAY_LAMP_IDX] = 0;

        }
    }

    if (strcmp(data.dataPath().c_str(), "/auto/plug") == 0)
    {
        String state = data.stringData();
        Serial.print("Database event: Home lamp was modified. Actual value:"); Serial.println(state);

        if (state == "AON")
        {
            state_array_always_on[STATE_ARRAY_PLUG_IDX] = 1;
        }
        else
        {
            state_array_always_on[STATE_ARRAY_PLUG_IDX] = 0;

        }
    }


    
}

// Timeout callback
void streamTimeoutCallback(bool timeout)
{
    if (timeout)
    {
        Serial.println("Stream timeout - streamCallback() related");
    }
    if (!fbdo.httpConnected())
    {
        Serial.printf("Error code: %d, reason: %s\n", fbdo.httpCode(), fbdo.errorReason().c_str());
    }
}

void IRAM_ATTR ISR() {
    motion_detected = true;
    last_visible_motion_timestamp = millis();
}

// Wi-Fi Connection
void wifiSetup()
{
    WiFi.mode(WIFI_STA);
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connecting...");
        delay(500);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}


void setup() {
    Serial.begin(115200);
    Serial.println();

    pinMode(PLUG_PIN, OUTPUT);
    digitalWrite(PLUG_PIN, LOW);

    pinMode(LAMP_PIN, OUTPUT);
    digitalWrite(LAMP_PIN, LOW);

    Serial.println(("DHT11 Begin!"));
    dht.begin();
    Serial.println(("DHT11 END!\n"));

    Serial.println(("motionSensor Begin!\n"));

    pinMode(PIR_1_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIR_1_PIN), ISR, RISING);

    // Wi-Fi connection
    wifiSetup();

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

    Firebase.reconnectWiFi(true);

    /* Sign up */
    if (Firebase.signUp(&config, &auth, "", "")) {
        Serial.println("SIGNUP OK");
    }
    else {
        Serial.printf("%s\n", config.signer.signupError.message.c_str());
        while (1)
        {
            Serial.println("Could not sign up .... re-plug the device");
            delay(1000);
        }
    }

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

    // Assign the maximum retry of token generation
    config.max_token_generation_retry = 5;

    Firebase.begin(&config, &auth);

    if (!Firebase.RTDB.beginStream(&fbdo, "/home/"))
        Serial.printf("stream begin error, %s\n\n", fbdo.errorReason().c_str());

    Firebase.RTDB.setStreamCallback(&fbdo, streamCallback, streamTimeoutCallback);

    Serial.println("End Firebase setup");
    Serial.println("~~~ Setup complete ~~~");
}


const long intervalUpdateDHT = 10000;                // interval at which to read sensor (milliseconds)
const long intervalUpdatePIR = 10000;                // interval at which to read sensor (milliseconds)
unsigned long interval_manual_to_auto = 20000;       // interval at which to read sensor (milliseconds)


void loop()
{
    if (Firebase.isTokenExpired())
    {
        Firebase.refreshToken(&config);
        Serial.println("Refresh token");
    }

    if (Firebase.ready())
    {
        // do nothing
    }

    unsigned long currentMillis = millis();
    handlerDHT(currentMillis);
    handlerPIR(currentMillis);

    switch (mode)
    {
        case Mode::AUTO:
        {
            if (state_array_always_on[STATE_ARRAY_PLUG_IDX])
            {
                digitalWrite(PLUG_PIN, LOW);
            }
            else
            {
                digitalWrite(PLUG_PIN, HIGH);
            }

            if (state_array_always_on[STATE_ARRAY_LAMP_IDX])
            {
                digitalWrite(LAMP_PIN, LOW);
            }
            else
            {
                digitalWrite(LAMP_PIN, HIGH);
            }
           
            break;
        }
        case Mode::MANUAL:
        {
            if (state_array[STATE_ARRAY_LAMP_IDX] == 1)
            {
                digitalWrite(LAMP_PIN, LOW);
            }
            else
            {
                digitalWrite(LAMP_PIN, HIGH);
            }

            if (state_array[STATE_ARRAY_PLUG_IDX] == 1)
            {
                digitalWrite(PLUG_PIN, LOW);
            }
            else
            {
                digitalWrite(PLUG_PIN, HIGH);
            }

            break;
        }
    }

}


void handlerDHT(unsigned long main_time)
{
    static unsigned long previousMillisDHT = 0;

    if (main_time - previousMillisDHT >= intervalUpdateDHT)
    {
        previousMillisDHT = main_time;

        float h = dht.readHumidity();
        float t = dht.readTemperature();

        Firebase.RTDB.setFloatAsync(&fbdo_write, TEMP_PATH, t);
        Firebase.RTDB.setFloatAsync(&fbdo_write, HUM_PATH, h);
    }
}

void handlerPIR(unsigned long main_time)
{
    static unsigned long previousMillisPir = 0;
    if (main_time - previousMillisPir >= intervalUpdatePIR)
    {
        previousMillisPir = main_time;
        Serial.println("Last motion "); Serial.print(millis() - last_visible_motion_timestamp); Serial.println("Seconds ago");
        Firebase.RTDB.setFloatAsync(&fbdo_write, PIR_PATH, main_time - last_visible_motion_timestamp);
    }

    if (motion_detected)
    {
        Serial.println("Motion Detected by PIR. Changed to MANUAL");
        motion_detected = false;

        mode = Mode::MANUAL;
        Firebase.RTDB.setStringAsync(&fbdo_write, MODE_PATH, "MANUAL");
    }
    else
    {
        if (mode == Mode::MANUAL)
        {
            if (main_time - last_visible_motion_timestamp >= interval_manual_to_auto)
            {
                Serial.println("Motion not Detected by PIR for more than 30 sec. Changed to AUTO");

                mode = Mode::AUTO;
                Firebase.RTDB.setStringAsync(&fbdo_write, MODE_PATH, "AUTO");
            }
        }
    }
}