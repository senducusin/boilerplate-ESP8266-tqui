float getCelsius(int rawAnalog, int rawDigital){
  float millivolts = (rawAnalog/1024.0) * 3300; //3300 is the voltage provided by NodeMCU
  float celsius = millivolts/10 - 8;
  
  return celsius;
}
