int count = 1;
int fnd_index=13;

void setup()
{
  for(int i=2; i<=13; ++i}{
    pinMode(i,OUTPUT);
};

for(int i=10; i<13; ++i}{
  digitalWrite(i,HIGH);//LOW);
  };
}

void loop()
{
  digitalWrite(fnd_index,LOW);
  findDisplay(count);
  delay(1000);

  digitalWrite(fnd_index, HIGH);

  if(count>=4){
    count = 1;
    fnd_index=13;
  }
  else{
    count++;
    fnd_index--;
  }
  delay(100);
}
