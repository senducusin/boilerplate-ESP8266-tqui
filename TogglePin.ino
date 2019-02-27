void TogglePin()
{
  digitalWrite(appliancePin, !digitalRead(appliancePin));
  delay(1);
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}
