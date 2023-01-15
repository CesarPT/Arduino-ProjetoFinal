/*
Arduino 1 - Entrada

Não coloquei mais circuitos porque ocupam portas no arduino.
Mas em melhoramentos futuros do projeto adicionava um alarme e
mais lâmpadas e outros sensores.

Botões do comnado:
0 - Fechar porta
1 - Abrir porta
2 - Ligar lâmpadas
3 - Desligar lâmpadas
4 - Controlar velocidade da ventoinha de 1 a 3 (motor)
5 - Desligar ventoinha (motor)
6 - Ligar lâmpada quarto
7 - Ligar lâmpada cozinha
8 - Ligar lâmpada sala
9 - Ver temperatura
*/

//Incluir bibliotecas: Sensor IR, LCD e Servo Motor
#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Servo.h>

//Declaração de variáveis
//Liga o LCD, o Sensor IR, Lâmpadas e Servo Motor nos pinos
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
const int RECV_PIN = 8;
const int SensorT = A0;
const int led1 = 10; //led quarto
const int led2 = 11; //led casa de banho
const int led3 = 12; //led cozinha
int ventoinha = 9;
int i = 0;
int converter = 0;
//Resultado do sensor IR e cria um objeto para o Servo
IRrecv irrecv(RECV_PIN);
decode_results results;
Servo motor;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  lcd.begin(16,2); //LCD com 16 colunas em 2 linhas
  lcd.setCursor(0,0); //Posiciona o cursor na coluna 0, linha 0 
  lcd.print("Porta fechada");
  irrecv.enableIRIn();
  //Posição inicial do motor (porta fechada)
  motor.attach(13);
  motor.write(0);
}

void loop() {
  //Se detetar pressionamento de tecla
  if (irrecv.decode(&results)) {
    //Atribui o valor da tecla para a variável
     unsigned int value = results.value;
     //Verifica qual a tecla e faz o código nela
     switch (value) {
//tecla 0
      case 0xFD00FF:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Porta fechada");
          lcd.setCursor(0, 1);
          lcd.print("");
          delay(50);
          motor.write(0); //roda -90 graus (metade)
      break;
//tecla 1
      case 0xFD08F7:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Porta aberta");
          lcd.setCursor(0, 1);
          lcd.print("Bem vindo :)");
          delay(50);
          motor.write(90); //Roda 90 graus (metade)
      break;
//tecla 2
      case 0xFD8877:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("LEDS ligados");
          lcd.setCursor(0, 1);
          lcd.print("");
       	  digitalWrite(led1, HIGH);
          digitalWrite(led2, HIGH);
          digitalWrite(led3, HIGH);
          delay(50);
      break;
//tecla 3
       case 0xFD48B7:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("LEDS desligados");
          lcd.setCursor(0, 1);
          lcd.print("");
       	  digitalWrite(led1, LOW);
          digitalWrite(led2, LOW);
          digitalWrite(led3, LOW);
          delay(50);
      break;
//tecla 4
      case 0xFD28D7:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Ventoinha");
          lcd.setCursor(0, 1);
          lcd.print("Velocidade:");
            //Verifica se a ventoinha é menor que o máximo,
            //se, aumenta a velocidade de 1 valor até 3.
            if (ventoinha <= 255){
              i+=85.3;
              analogWrite(ventoinha, i);
            }
            //Se chegar ao limite (3) reseta para 85.3
            //e volta à velocidade 1.
          lcd.setCursor(12, 1);
       if (converter < 3){
         //Converter 255 valores para 3 valores
         converter = map(i, 0, 255, 0, 3);
         lcd.print(converter);
       } else {
         //Se chegar ao limite reseta para 1
         converter = 1;
         lcd.print(converter);
         i = 85.3;
       }
          delay(50);
      break;
//tecla 5
      case 0xFDA857:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Ventoinha");
          lcd.setCursor(0, 1);
          lcd.print("desligada");
          i=0;
              analogWrite(ventoinha, i);
          delay(50);
      break;
//tecla 6
      case 0xFD6897:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("LED do quarto");
          lcd.setCursor(0, 1);
          lcd.print("");
       	  digitalWrite(led1, !digitalRead(led1)); //on/off
          delay(50);
      break;
//tecla 7
      case 0xFD18E7:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("LED da cozinha");
          lcd.setCursor(0, 1);
          lcd.print("");
          digitalWrite(led2, !digitalRead(led2)); //on/off
          delay(50);
      break;
//tecla 8
      case 0xFD9867:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("LED da sala");
          lcd.setCursor(0, 1);
          lcd.print("");
          digitalWrite(led3, !digitalRead(led3)); //on/off
          delay(50);
      break;
//tecla 9
      case 0xFD58A7:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Temperatura:");
          /*Faz a leitura da tensao no Sensor de Temperatura
          Converte a tensão lida de 5V para 1024 valores
          Converte a Tensao em graus Celsius e em float
          Imprime a temperatura.*/
          const int SensorTTensao = analogRead(SensorT);
  
          float Tensao = SensorTTensao*5;
          Tensao/=1024;
          //Temperatura
          float TemperaturaC = (Tensao-0.5)*100;
           lcd.setCursor(0,1);
           //Temperatura simulada não corresponde ao ambiente
           lcd.print(TemperaturaC);
          lcd.setCursor(6, 1);
          lcd.print("C");
          i=0;
          delay(50);
      break;
    }
  }
  irrecv.resume(); //Leitura do próximo valor do sensor IR
  delay(200);
}