/*
  UnB - Universidade de Brasilia
  FGA
  Prova Prática 1 - UART - Sistemas Embarcados
  Prof. Renato Sampaio
  
  Data: 07/05/2016

  Codigo do Microcontrolador Arduino para estabelecer comunicação serial com 
  o Raspberry Pi.
  
*/

/* --------------------------------------------------
 *  Configuração das Portas Seriais
 *  
 *  Serial = Comunicação Arduino - PC
 *  Serial = Comuinicação Arduino - Raspberry Pi
 ---------------------------------------------------*/
#define RUN

void setup() {
  // Configura Pino do LED
  pinMode(13, OUTPUT);
  // Configura Serial - PC
#ifdef DEBUG  
  Serial.begin(9600);
  while (!Serial) {
    ; // Aguarda inicialização da Serial
  }
#endif  
  // Configura Serial - Raspberry Pi
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

/*------------------------------------------------------
 *  Configuração de Variáveis Globais
 ----------------------------------------------------- */
#define CMD_TEMPERATURA    0xA1
#define CMD_PRESENCA       0xA2
#define CMD_PORTAS_JANELAS 0xA3
#define CMD_TODOS_OS_SENSORES 0xA4
#define CMD_TODOS_OS_DISPOSITIVOS 0xA5

#define CMD_AR_CONDICIONADO 0xB1
#define CMD_LAMPADA         0xB2
#define CMD_ALARME          0xB3


#define CMD_ERROR 0xE1

float temperatura[3] = {23.1, 28.4, 21.8};
unsigned char presenca[3] = {0, 0, 0};
unsigned char portas_janelas[6] = {0,0,0,0,0,0};
unsigned char ar_condicionado[2] = {0,0};
unsigned char lampada[6] = {0,0,0,0,0,0};
unsigned char alarme[2] = {0,0};

unsigned char dado_recebe;
unsigned char valor;
unsigned int contador_tempo = 0;
int i;
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
void le_uart(int num_bytes) {
  int i = 0;
  while(i < num_bytes) {
    if(Serial.available()) {
      dados_recebidos[i] = Serial.read();
      //Serial.print(dados_recebidos[i]);
      i++;
    }
  }
}

void envia_float(float dado) {
  bytesFloat envia_dado;
  envia_dado.valor_float = dado;
  Serial.write(envia_dado.bytes,4);
}

unsigned char gera_aleatorio() {
  long randNumber;
  randNumber = random(100);
  if(randNumber > 50)
    return 1;
  else
    return 0;
}

/*------------------------------------------------------
 *  Loop Principal
 ----------------------------------------------------- */
void loop() {

if(contador_tempo++ == 6000) {

  Serial.println("### Dados de Sensores Atualizados ###");
  contador_tempo = 0;
  // Atualiza Sensores de Presença e Portas
  for(i = 0; i < 3; i++) {
    presenca[i] = gera_aleatorio();
  }
  for(i = 0; i < 6; i++) {
    portas_janelas[i] = gera_aleatorio();
  }
}

  if(Serial.available()) {
    dado_recebe = Serial.read();
#ifdef DEBUG    
    Serial.print("Comando Recebido: ");
    Serial.println(dado_recebe, HEX);
#endif
    switch (dado_recebe) {   
// ------------------------------------------------------    
// COMANDOS DE LEITURA DE SENSORES
// ------------------------------------------------------
      case CMD_TEMPERATURA:
        dado_recebe = Serial.read();
#ifdef DEBUG        
        Serial.print("Solicitou a Temperatura do sensor: ");
        Serial.println(dado_recebe);
#endif        
        envia_float(temperatura[dado_recebe-1]);
        break;
      case CMD_PRESENCA:
        dado_recebe = Serial.read();
#ifdef DEBUG        
        Serial.print("Solicitou o estado do sensor de Presença:");
        Serial.println(dado_recebe);
        Serial.print("Sensor de Presença [");
        Serial.print(dado_recebe);
        Serial.print("]: ");
        Serial.println(presenca[dado_recebe-1]);
#endif        
        Serial.write(presenca[dado_recebe-1]);
        break;
      case CMD_PORTAS_JANELAS:
        dado_recebe = Serial.read();
#ifdef DEBUG        
        Serial.print("Solicitou o estado do sensor de Portas_Janelas:");
        Serial.println(dado_recebe);
        Serial.print("Sensor de Portas_Janelas [");
        Serial.print(dado_recebe);
        Serial.print("]: ");
        Serial.println(portas_janelas[dado_recebe-1]);
#endif        
        Serial.write(portas_janelas[dado_recebe-1]);
        break;
// ------------------------------------------------------
// COMANDOS EXTRAS
// ------------------------------------------------------
      case CMD_TODOS_OS_SENSORES:
        dado_recebe = Serial.read();
#ifdef DEBUG        
        Serial.println("Solicitou o estado de todos os sensores:");

        Serial.print("Presenca: ");
        for(i = 0; i < 3; i++) {
          Serial.print(presenca[i]);
          Serial.print(" ");
        }
        Serial.print("| Portas/Janelas: ");
        for(i = 0; i < 6; i++) {
          Serial.print(portas_janelas[i]);
          Serial.print(" ");
        }
        Serial.println(" ");
#endif        
        for(i = 0; i < 3; i++) {
          Serial.write(presenca[i]);
        }
        for(i = 0; i < 6; i++) {
          Serial.write(portas_janelas[i]);
        }
        break;
      case CMD_TODOS_OS_DISPOSITIVOS:
        dado_recebe = Serial.read();
#ifdef DEBUG        
        Serial.println("Solicitou o estado de todos os dispositivos:");

        Serial.print("Ar-condicionado: ");
        for(i = 0; i < 2; i++) {
          Serial.print(ar_condicionado[i]);
          Serial.print(" ");
        }
        Serial.print("| Lampadas: ");
        for(i = 0; i < 6; i++) {
          Serial.print(lampada[i]);
          Serial.print(" ");
        }
        Serial.print("| Alarme: ");
        for(i = 0; i < 2; i++) {
          Serial.print(alarme[i]);
          Serial.print(" ");
        }        
        Serial.println(" ");
#endif        
        for(i = 0; i < 2; i++) {
          Serial.write(ar_condicionado[i]);
        }
        for(i = 0; i < 6; i++) {
          Serial.write(lampada[i]);
        }
        for(i = 0; i < 2; i++) {
          Serial.write(alarme[i]);
        }
        break;        
// ------------------------------------------------------
// COMANDOS DE ACIONAMENTO DE DISPOSITIVOS
// ------------------------------------------------------
      case CMD_AR_CONDICIONADO:
        dado_recebe = Serial.read();
        valor = Serial.read();
        ar_condicionado[dado_recebe-1] = valor;
#ifdef DEBUG        
        Serial.print("Ar-condicionado [");
        Serial.print(dado_recebe);
        if(valor == 1)
          Serial.println("] - Ligado");
        else
          Serial.println("] - Desligado");
#endif         
        break;
      case CMD_LAMPADA:
        dado_recebe = Serial.read();
        valor = Serial.read();
        lampada[dado_recebe-1] = valor;
#ifdef DEBUG        
        Serial.print("Lampada [");
        Serial.print(dado_recebe);
        if(valor == 1)
          Serial.println("] - Ligada");
        else
          Serial.println("] - Desligada");
#endif            
        break;
      case CMD_ALARME:
        dado_recebe = Serial.read();
        valor = Serial.read();
        alarme[dado_recebe-1] = valor;
#ifdef DEBUG        
        Serial.print("Alarme - Setor [");
        Serial.print(dado_recebe);
        if(valor == 1)
          Serial.println("] - Ligado");
        else
          Serial.println("] - Desligado");
#endif           
        break;
      default:
        //Serial.write(CMD_ERROR);
#ifdef DEBUG      
        Serial.print("Dado invalido!!!");
#endif        
        break;
      
    } // End Switch
    
  } // End IF INICIAL
  delay(10);
}

