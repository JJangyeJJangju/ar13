int number = 0;
int sum = 0;

void setup(){
  Serial.begin(9600);
}
void loop() {

  number++;
  sum += number;
  Serial.print("Number=");
  Serial.print(number);
  Serial.print(",Sum=");
  Serial.println(sum);
 
  if(number == 100){
  Serial.println();
  Serial.print("AR13:1+2+...+100=");
  Serial.println(sum);
  delay(1000);
  exit(0);
}
 
  delay(100);
} 
