
void bateria(int nivel);

void setup() 
{
 
  for(int i=2; i<8; i++) pinMode(i, OUTPUT);

  for(int i=2; i<8; i++) digitalWrite(i, HIGH);

} 

void loop() 
{
   bateria(analogRead(9)); //Definição da porta de entrada da bateria
} 

//Funções
void bateria(int nivel)
{
  
  if(nivel < 50)
  {
    digitalWrite(24, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
    digitalWrite(28, HIGH); 
    digitalWrite(29,  LOW);
 
  } 
  
  else if(nivel < 205)
  {
    digitalWrite(24,  LOW);
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
    digitalWrite(28, HIGH);
    digitalWrite(29, !digitalRead(7));
    delay(90);
    
  } 

  else if(nivel < 410)
  {
    digitalWrite(24,  LOW);
    digitalWrite(25,  LOW);
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
    digitalWrite(28, HIGH);
    digitalWrite(29, !digitalRead(7));
    delay(150);

    } 

  else if(nivel < 615)
  {
    digitalWrite(24,  LOW);
    digitalWrite(25,  LOW);
    digitalWrite(26,  LOW);
    digitalWrite(27, HIGH);
    digitalWrite(28, HIGH);
    digitalWrite(29, HIGH);
    
  } 

  else if(nivel < 820)
  {
    digitalWrite(24,  LOW);
    digitalWrite(25,  LOW);
    digitalWrite(26,  LOW);
    digitalWrite(27,  LOW);
    digitalWrite(28, HIGH);
    digitalWrite(29, HIGH);
    
  } 

  else if(nivel < 1023)
  {
    digitalWrite(24,  LOW);
    digitalWrite(25,  LOW);
    digitalWrite(26,  LOW);
    digitalWrite(27,  LOW);
    digitalWrite(28,  LOW);
    digitalWrite(29, HIGH);
    
  } 
  
} 



