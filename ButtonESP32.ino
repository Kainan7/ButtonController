#include <BleKeyboard.h>

#define btn_left 22
#define btn_right 23
#define btn_up 27
#define btn_down 26

BleKeyboard blekeyboard("ESP32 Kainã", "Espressif", 100);

// Variáveis para debounce do botão de pulo
unsigned long lastDebounceTimeUp = 0;
bool isJumping = false;
unsigned long debounceDelay = 50; // Tempo de debounce em milissegundos

void setup() {
  Serial.begin(115200);
  pinMode(btn_left, INPUT_PULLUP);
  pinMode(btn_right, INPUT_PULLUP);
  pinMode(btn_up, INPUT_PULLUP);
  pinMode(btn_down, INPUT_PULLUP);
  blekeyboard.begin();
}

void loop() {
  if (digitalRead(btn_left) == LOW) {
    Serial.println("esquerda");
    if (blekeyboard.isConnected()) {
      blekeyboard.press(KEY_LEFT_ARROW);
    }
  } else {
    blekeyboard.release(KEY_LEFT_ARROW);
  }

  if (digitalRead(btn_right) == LOW) {
    Serial.println("direita");
    if (blekeyboard.isConnected()) {
      blekeyboard.press(KEY_RIGHT_ARROW);
    }
  } else {
    blekeyboard.release(KEY_RIGHT_ARROW);
  }

  // Verifica o botão de pulo com debounce
  if (digitalRead(btn_up) == LOW && millis() - lastDebounceTimeUp > debounceDelay) {
    Serial.println("cima");
    lastDebounceTimeUp = millis();
    if (blekeyboard.isConnected()) {
      if (!isJumping) { // Se o Mario não estiver pulando atualmente
        blekeyboard.press(KEY_UP_ARROW);
        delay(500);
        blekeyboard.release(KEY_UP_ARROW);
        isJumping = true; // Marca que o Mario está pulando
      }
    }
  } else if (digitalRead(btn_up) == HIGH) {
    isJumping = false; // Reinicia a flag de pulo quando o botão é solto
  }

  if (digitalRead(btn_down) == LOW) {
    Serial.println("baixo");
    if (blekeyboard.isConnected()) {
      blekeyboard.press(KEY_DOWN_ARROW);
      delay(100);
      blekeyboard.release(KEY_DOWN_ARROW);
      delay(100); // Aguarda um curto período antes de pressionar a tecla "Enter" para atirar
      blekeyboard.press(KEY_RETURN); // Pressiona a tecla "Enter" para atirar
      delay(50); // Mantém a tecla "Enter" pressionada por um curto período
      blekeyboard.release(KEY_RETURN); // Libera a tecla "Enter"
    }
  }

  // Verifica se o botão de pulo está pressionado junto com o botão de movimento para frente ou para trás
  if (digitalRead(btn_up) == LOW && (digitalRead(btn_left) == LOW || digitalRead(btn_right) == LOW)) {
    Serial.println("pular com movimento");
    if (blekeyboard.isConnected()) {
      if (digitalRead(btn_left) == LOW) {
        blekeyboard.press(KEY_LEFT_ARROW);
      } else {
        blekeyboard.press(KEY_RIGHT_ARROW);
      }
      delay(500); // Ajuste o tempo de pulo conforme necessário
      blekeyboard.release(KEY_LEFT_ARROW);
      blekeyboard.release(KEY_RIGHT_ARROW);
    }
  }

  delay(20); // Pequeno delay para evitar leituras falsas
}
