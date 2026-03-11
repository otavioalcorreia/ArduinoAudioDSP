 /* Filtro Passa Baixa
 * TCC
 * PROCESSAMENTO DIGITAL DE ÁUDIO - Uma abordagem Didática com Arduino
 * Otavio Leite
 * Eletrônica Industrial
 * Fatec SP
 * Adaptado de: https://github.com/curiores
 */
 
template <int order> // Ordem - 1 ou 2
class LowPass 
{
  private:
    float a[order];
    float b[order+1];
    float omega0;
    float dt;
    bool adapt;
    float tn1 = 0;
    float x[order+1]; // Valores Brutos
    float y[order+1]; // Valores Filtrados

  public:  
    LowPass(float f0, float fs,bool adaptive){
      // f0: frequêcia de corte (Hz)
      // fs: frequêncas de amostragem (sample rate) (Hz)
      // adaptive: boolean flag, se selecionado 1, o código irá automaticamente selecionar 
      // a frequência de amostragem baseada no histórico de tempo.
      
      omega0 = 6.28318530718*f0;
      dt = 1.0/fs;
      adapt = adaptive;
      tn1 = -dt;
      for(int k = 0; k < order+1; k++){
        x[k] = 0;
        y[k] = 0;        
      }
      setCoef();
    }

    void setCoef(){
      if(adapt){
        float t = micros()/1.0e6;
        dt = t - tn1;
        tn1 = t;
      }
      
      float alpha = omega0*dt;
      if(order==1){
        a[0] = -(alpha - 2.0)/(alpha+2.0);
        b[0] = alpha/(alpha+2.0);
        b[1] = alpha/(alpha+2.0);        
      }
      if(order==2){
        float alphaSq = alpha*alpha;
        float beta[] = {1, sqrt(2), 1};
        float D = alphaSq*beta[0] + 2*alpha*beta[1] + 4*beta[2];
        b[0] = alphaSq/D;
        b[1] = 2*b[0];
        b[2] = b[0];
        a[0] = -(2*alphaSq*beta[0] - 8*beta[2])/D;
        a[1] = -(beta[0]*alphaSq - 2*beta[1]*alpha + 4*beta[2])/D;      
      }
    }

    float filt(float xn){
      // Fornecido o valor bruto x
      // Devolve o valor filtrado y
      if(adapt){
        setCoef(); // Atualiza os coeficiente se necessário     
      }
      y[0] = 0;
      x[0] = xn;
      // Calcula os valores filtrados
      for(int k = 0; k < order; k++){
        y[0] += a[k]*y[k+1] + b[k]*x[k];
      }
      y[0] += b[order]*x[order];

      // Salva o histórico de valores
      for(int k = order; k > 0; k--){
        y[k] = y[k-1];
        x[k] = x[k-1];
      }
  
      // Retorna os valores filtrados    
      return y[0];
    }
};

// Instância de filtragem
LowPass<2> lp(5000,44.1e3,false);

void setup() {

  analogReadResolution(12);  //Define a profundidade de bits do conversor AD
  analogWriteResolution(12); //Define a profundidade de bits do conversor DA
}

void loop() {
  
  // Lê o pino do conversor AD
  int analogValue = analogRead(A0);
  float xn = (analogValue);

  // Computa o sinal filtrado
  float yn = lp.filt(xn);
 
  //Escreve o sinal filtrado no pino do conversor DA
  analogWrite(DAC0, (yn));
  
}
