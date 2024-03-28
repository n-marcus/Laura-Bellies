void printStructMessage(const struct_message &msg) {
  Serial.print("target: ");
  Serial.println(msg.target);

  Serial.print("heartbeatRate: ");
  Serial.println(msg.heartbeatRate);

  Serial.print("humanPresence: ");
  Serial.println(msg.humanPresence ? "true" : "false");

  Serial.print("breathingsPerMinute: ");
  Serial.println(msg.breathingsPerMinute);

  Serial.print("beingTouched: ");
  Serial.println(msg.beingTouched);

  Serial.print("isAboutBeingTouched: ");
  Serial.println(msg.isAboutTouch);
}