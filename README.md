# esp266-lcd


Implementa funções para escrever em um display LCD com shield I2C em um ESP8266.

## Conexões

Com um display LCD conectado à um shield i2c, conecte os pinos SDA e SCL aos pinos D2 e D1, respectivamente.

Além disso é necessário conectar o shield à alimentação e GND. Ele pode ser conectado à uma fonte externa de até 7V ou nos pinos de 5V ou 3.3V do próprio ESP8266.

## Configurando um display

Para configurar um display inicialize a classe LCD.

Por padrão está definido um display 16x2, que pode ser alterado

A função init inicializa o display, tendo como possibilidade selecionar se o backlight estará ligado ou não, bem como se o cursor aparecerá e se ele estará piscando.

Outras funções úteis são `set_cursor` para alterar a posição do cursor e `write_string_at`, que altera a posição do cursor e escreve uma frase nela.

Um exemplo de utilização está presente no arquivo `main/lcd_main.cpp`

## Configurando o projeto no ESP

Tendo o esp-idf instalado conforme documentação presente em `https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html`, vá para a pasta onde o projeto está clonado e rode o comando:

    make menuconfig

no menu, navegue para `Serial flasher config` > `Default serial port` para configurar a porta serial onde o esp está conectado.

Após isso confirme a seleção, salve em `< Save >` e saia em `< Exit >`.


## Build e Flash

Com todas as configurações realizadas, é possível rodar o comando:

    make flash

Com isso, o projeto será compilado, gerando os componente para o ESP8266.

Após a finalização o display conectado à um shield i2c deve mostrar a mensagem esperada.