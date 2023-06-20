/* Filtro Transparente
 * TCC
 * PROCESSAMENTO DIGITAL DE ÁUDIO - Uma abordagem Didática com Arduino
 * Otavio Leite
 * Eletrônica Industrial
 * Fatec SP 
 */

void setup() {
analogReadResolution(12);  //Define a profundidade de bits do conversor AD
analogWriteResolution(12); //Define a profundidade de bits do conversor DA
}

void loop() {

   analogWrite(DAC0, analogRead(A0)); //Lê o conversor AD e escreve diretamente 
                                      //no conversor DA com ajuste de ganho
}
