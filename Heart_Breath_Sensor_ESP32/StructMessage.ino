void printStructMessage(const struct_message &msg) {
  Serial.print("a: ");
  Serial.println(msg.a);

  Serial.print("heartbeatRate: ");
  Serial.println(msg.heartbeatRate);

  Serial.print("humanPresence: ");
  Serial.println(msg.humanPresence ? "true" : "false");

  Serial.print("breathingsPerMinute: ");
  Serial.println(msg.breathingsPerMinute);
}