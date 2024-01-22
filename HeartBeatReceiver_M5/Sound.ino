

#define CONFIG_I2S_BCK_PIN 19
#define CONFIG_I2S_LRCK_PIN 33
#define CONFIG_I2S_DATA_PIN 22
#define CONFIG_I2S_DATA_IN_PIN 23

#define SPEAK_I2S_NUMBER I2S_NUM_0

#define MODE_MIC 0
#define MODE_SPK 1

bool Spakeflag = true;
size_t bytes_written;
int audio_position = 0;
extern const unsigned char audio_chocobo[914158];
const int audio_chocobo_length = sizeof(audio_chocobo);

elapsedMillis timeSinceLastHeartbeatSound;


void setupSpeaker() {
  Serial.println("Init Speaker");
  InitI2SSpeakOrMic(MODE_SPK);
  delay(100);
}

bool InitI2SSpeakOrMic(int mode) {
  esp_err_t err = ESP_OK;

  i2s_driver_uninstall(SPEAK_I2S_NUMBER);
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER),
    .sample_rate = 88200,
    .bits_per_sample =
      I2S_BITS_PER_SAMPLE_16BIT,  // is fixed at 12bit, stereo, MSB
    .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 6,
    .dma_buf_len = 60,
  };
  if (mode == MODE_MIC) {
    i2s_config.mode =
      (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
  } else {
    i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
    i2s_config.use_apll = false;
    i2s_config.tx_desc_auto_clear = true;
  }

  //Serial.println("Init i2s_driver_install");

  err += i2s_driver_install(SPEAK_I2S_NUMBER, &i2s_config, 0, NULL);
  i2s_pin_config_t tx_pin_config;

#if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0))
  tx_pin_config.mck_io_num = I2S_PIN_NO_CHANGE;
#endif

  tx_pin_config.bck_io_num = CONFIG_I2S_BCK_PIN;
  tx_pin_config.ws_io_num = CONFIG_I2S_LRCK_PIN;
  tx_pin_config.data_out_num = CONFIG_I2S_DATA_PIN;
  tx_pin_config.data_in_num = CONFIG_I2S_DATA_IN_PIN;

  //Serial.println("Init i2s_set_pin");
  err += i2s_set_pin(SPEAK_I2S_NUMBER, &tx_pin_config);
  //Serial.println("Init i2s_set_clk");
  err += i2s_set_clk(SPEAK_I2S_NUMBER, int(88200), I2S_BITS_PER_SAMPLE_16BIT,
                     I2S_CHANNEL_MONO);

  return true;
}


void playHeartbeatSound() {
  // Serial.println("portMAX_DELAY " + String(portMAX_DELAY));
  long startTime = millis();
  Serial.println("-- Starting to play heartbeat sample now");
  int startByte = 1000;  // Adjust the starting byte position
  // long numBytesToPlay = map(heartbeatRate, 60, 120, 914158, 5000);
  // numBytesToPlay = constrain(numBytesToPlay, 0, 914158 - 1000);

  //this seems to be the sample rate
  int sampleRate = 174;

  //convert the BPM into a ms length
  Serial.println("Heartbeat rate = " + String(heartbeatRate));
  float desiredLengthMs = 60000 / int(heartbeatRate);
  currentInterval = desiredLengthMs;
  Serial.println("Desired ms = " + String(60000 / int(heartbeatRate)));

  //convert the desired ms to a length in smaples
  int desiredLengthSamples = int(desiredLengthMs * sampleRate);

  int numBytesToPlay = desiredLengthSamples;  // Adjust the number of bytes to play
  // numBytesToPlay = myData.heartbeatRate;

  // numBytesToPlay = numBytesToPlay - (myData.heartbeatRate * 10000);
  Serial.println("Num bytes = " + String(numBytesToPlay));

  // Calculate the number of samples to skip
  // int samplesToSkip = int(SAMPLE_RATE * (speedFactor - 1.0));


  i2s_write(SPEAK_I2S_NUMBER, &audio_chocobo[startByte], numBytesToPlay, &bytes_written, portMAX_DELAY);
  long length = millis() - startTime;

  Serial.println("");
  Serial.println("--Playing the sound took " + String(length) + "ms");
  // Serial.println("Samplerate seems to be " + String(numBytesToPlay / length));
  Serial.println("BPM was: " + String(60000 / length));
  Serial.println("");
}

void playHeartbeatSoundAtRate(float speedFactor) {
  //this makes the controller crash for some reason
  int originalSize = sizeof(audio_chocobo);
  int newSize = int(originalSize / speedFactor);

  // Check if the newSize is within reasonable limits
  if (newSize <= 0 || newSize > originalSize) {
    Serial.println("Invalid newSize");
    return;
  }

  // Create an intermediate buffer for interpolated samples
  int* interpolatedBuffer = new int[newSize];

  Serial.println("Heap before stretching = " + String(ESP.getFreeHeap()));

  // Generate interpolated samples and store them in the buffer
  for (int i = 0; i < newSize - 1; i++) {
    // Calculate the position in the original data
    float pos = i * speedFactor;

    // Check if the position is within the valid range
    if (pos < 0 || pos >= originalSize - 1) {
      Serial.println("Invalid position");
      delete[] interpolatedBuffer;
      return;
    }

    // Calculate the integer and fractional parts of the position
    int intPos = int(pos);
    float fracPos = pos - intPos;

    // Linear interpolation
    interpolatedBuffer[i] = int(audio_chocobo[intPos] * (1 - fracPos) + audio_chocobo[intPos + 1] * fracPos);
  }

  Serial.println("Heap after stretching = " + String(ESP.getFreeHeap()));

  // Write the entire interpolated buffer to I2S
  i2s_write(SPEAK_I2S_NUMBER, interpolatedBuffer, newSize * sizeof(int), &bytes_written, portMAX_DELAY);

  // Delete the intermediate buffer to free up memory
  delete[] interpolatedBuffer;
}



void updateSound() {
  if (humanPresence) {
    playHeartbeatSound();
  }
  // playHeartbeatSoundAtRate(1);
  if (timeSinceLastHeartbeatSound > currentInterval) {
    timeSinceLastHeartbeatSound = 0;
  }
}