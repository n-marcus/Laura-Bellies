// Insert the MAC address of the receiver board: B
uint8_t broadcastAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

esp_now_peer_info_t peerInfo;

void setupESPNow() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(reinterpret_cast<esp_now_recv_cb_t>(OnDataRecv));
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void sendData() {
  //if this is pod a, send it to pod b and in reverse
  messageToSend.target = POD_IDENTIFIER == 'a' ? 'b' : 'a';

  messageToSend.heartbeatRate = 0;
  messageToSend.humanPresence = false;
  messageToSend.breathingsPerMinute = 0;


  //include whether this one is being touched or not
  messageToSend.beingTouched = beingTouched;
  //signal that this message is about being touched or not
  messageToSend.isAboutTouch = true;
  messageToSend.breathingPercentage = -1;
  Serial.println("Sending message about touch being: " + String(messageToSend.beingTouched));

  // Serial.println("About to send message");
  // printStructMessage(messageToSend);


  //try to send the message
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&messageToSend, sizeof(messageToSend));

  if (result == ESP_OK) {
    messagesSend++;
    Serial.println("ESP Now message sent with success #" + String(messagesSend));
  } else {
    Serial.println("Error sending the data");
  }
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  messagesReceived++;
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  bool targetIsMe = receivedData.target == POD_IDENTIFIER;
  Serial.print("---Received data #" + String(messagesReceived) + " for " + String(receivedData.target));
  if (targetIsMe) Serial.print(" which is me!");
  Serial.println("");

  if (targetIsMe && receivedData.breathingsPerMinute == 0 && receivedData.humanPresence == 0 && receivedData.heartbeatRate == 0 && receivedData.breathingPercentage == -1) {
    Serial.println("All readings are 0 and not about being touched");
    Serial.println(" ");
  } else if (targetIsMe) {
    //if everything was not 0
    Serial.print("breathingsPerMinute: " + String(receivedData.breathingsPerMinute));
    Serial.print(" Human presence: " + String(receivedData.humanPresence));
    Serial.print(" Breathing percentage: " + String(receivedData.breathingPercentage));
    Serial.println(" Heartrate: " + String(receivedData.heartbeatRate));
    Serial.println(" ");
  }

  if (targetIsMe) {

    if (receivedData.isAboutTouch) {
      Serial.println("Received info about other pod being touched " + String(receivedData.beingTouched));

      //save whether the other pod is being touched localy
      setOtherPodTouched(receivedData.beingTouched);

    } else if (receivedData.breathingPercentage != -1) {
      Serial.println("Received breathing percentage: " + String(receivedData.breathingPercentage));
      breathCyclePercentage = receivedData.breathingPercentage;
    } else {
      //if this is not about being touched, but a message from the heartbeat sensor
      //save the received human presence
      humanPresence = receivedData.humanPresence;

      //if there is human presence, we should be breathing
      if (humanPresence == 1) {
        shouldBeBreathing = true;
      }

      //if there is a valid breath bpm detected, reflect in the bpm
      if (receivedData.breathingsPerMinute > 0) {
        //it will crash because of a divide by zero error if it receives zero for the bpm
        breathingBPM = receivedData.breathingsPerMinute;
        msPerBreathCycle = 60000 / breathingBPM;
      } else {
        humanPresence = false;
      }

      if (receivedData.breathingsPerMinute == 0 || receivedData.humanPresence == false) {
        //if there is no human detected, return to a verrry slow bpm
        breathingBPM = defaultBreathingBPMWithoutHumanDetected;
        msPerBreathCycle = 60000 / breathingBPM;
      }
    }

  } else {
    Serial.println("Got message for different pod, namely: " + String(receivedData.target));
  }
}