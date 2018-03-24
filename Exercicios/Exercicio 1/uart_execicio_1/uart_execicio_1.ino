/*
  UnB - Universidade de Brasilia
  FGA
  Exercício 1 - UART - Sistemas Embarcados
  Prof. Renato Sampaio
  
  Data: 07/04/2016

  Codigo do Microcontrolador Arduino para estabelecer comunicação serial com 
  o Raspberry Pi.
  
*/

/* --------------------------------------------------
 *  Configuração das Portas Seriais
 *  
 *  Serial = Comunicação Arduino - PC
 *  Serial1 = Comuinicação Arduino - Raspberry Pi
 ---------------------------------------------------*/

void setup() {
  // Configura Pino do LED
  pinMode(13, OUTPUT);
  // Configura Serial - PC
  Serial.begin(9600);
  //while (!Serial) {
  //  ; // Aguarda inicialização da Serial
  //}
  // Configura Serial - Raspberry Pi
  //Serial1.begin(9600);
}

/*------------------------------------------------------
 *  Configuração de Variáveis Globais
 ----------------------------------------------------- */
#define CMD_SOLICITA_INT    0xA1
#define CMD_SOLICITA_FLOAT  0xA2
#define CMD_SOLICITA_STRING 0xA3

#define CMD_ENVIA_INT    0xB1
#define CMD_ENVIA_FLOAT  0xB2
#define CMD_ENVIA_STRING 0xB3

long int dado_inteiro = 41987;
float dado_real = 3.141516;
char dado_string[] = "Mensagem de Teste pela UART";

unsigned char dado_recebe;

char dados_recebidos[256];

typedef union {
 float valor_float;
 byte bytes[4];
} bytesFloat;

typedef union {
 long int valor_int;
 byte bytes[4];
} bytesInt;

/*------------------------------------------------------
 *  Funções Auxiliares
 ----------------------------------------------------- */
//void le_uart(int num_bytes) {
//  int i = 0;
//  while(i < num_bytes) {
//    if(Serial1.available()) {
//      dados_recebidos[i] = Serial1.read();
//      //Serial.print(dados_recebidos[i]);
//      i++;
//    }
//  }
//}

//void le_matricula() {
//  le_uart(4);
//  Serial.print("Matricula: ");
//  for(int i = 0; i < 4; i++) {
//    Serial.print(dados_recebidos[i]);
//  }
//  Serial.println(" ");
//}

//int le_inteiro() {
//  int num_inteiro = 0;
//  le_uart(4);
//  memcpy(&num_inteiro, dados_recebidos, 4);
//  Serial.print("Inteiro recebido: ");
//  Serial.println(num_inteiro);
//  //return valor.valor_int;
//  return num_inteiro;
//}

//float le_float() {
//  float num_float;
//  le_uart(4);
//  memcpy(&num_float, dados_recebidos, 4);
//  Serial.print("Float recebido: ");
//  Serial.println(num_float);
//  return num_float;  
//}

//void le_string() {
//  int tamanho;
//  le_uart(1);
//  tamanho = (int) dados_recebidos[0];
//  Serial.print("String recebida [");
//  Serial.print(tamanho);
//  Serial.print("]: ");
//  le_uart(tamanho);
//  for (int i = 0; i < tamanho; ++i)
//  {
//    Serial.print(dados_recebidos[i]);
//  }
//  Serial.println("");
//}

void envia_int(long int dado) {
  bytesInt envia_dado;
  envia_dado.valor_int = dado;
  Serial.write(envia_dado.bytes,4);
}

void envia_float(float dado) {
  bytesFloat envia_dado;
  envia_dado.valor_float = dado;
  Serial.write(envia_dado.bytes,4);
}

/*void envia_string(char * dado, int tamanho) {
  Serial.write((char) tamanho);
  Serial.write(dado,tamanho);   
}*/

/*------------------------------------------------------
 *  Loop Principal
 ----------------------------------------------------- */
void loop() {

  if(Serial.available()) {
    dado_recebe = Serial.read();
    //Serial.print("Comando Recebido: ");
    //Serial.println(dado_recebe, HEX);

    switch (dado_recebe) {
      case CMD_SOLICITA_INT:
        //le_matricula();
        //Serial.println("Solicitou um INTEIRO");
        envia_int(dado_inteiro);
        break;
      case CMD_SOLICITA_FLOAT:
        //le_matricula();
        //Serial.println("Solicitou um FLOAT");
        envia_float(dado_real);
        break;
      case CMD_SOLICITA_STRING:
        //le_matricula();
        //Serial.println("Solicitou uma STRING");
        //envia_string(dado_string, strlen(dado_string));
        break;
      case CMD_ENVIA_INT:
        //le_inteiro();
        //le_matricula();
        break;
      case CMD_ENVIA_FLOAT:
        //le_float();
        //le_matricula();
        break;
      case CMD_ENVIA_STRING:
        //le_string();
        //le_matricula();
        break;
      default:
        Serial.print("Dado invalido!!!");
        break;
      
    } // End Switch
    
  } // End IF INICIAL
  delay(10);
}
