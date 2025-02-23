(PT-BR)
Integração da ESP32-S3 ou ESP32-DevKit1 com o software Elipse E3 para criação de um sistema supervisório.
Siga as instruções para conseguir conectar com sucesso sua ESP32 ao ElipseE3 para projetos de monitoramento, neste projeto utilizei dois sensores conectados a esp, além de um buzzer ativo para um sinal sonoro.
Porém, o projeto pode ser adaptado da maneira que desejar para conseguir utilizar ao seu desejo.

(EN-US)
Integration of the ESP32-S3 or ESP32-DevKit1 with the Elipse E3 software to create a supervisory system.
Follow the instructions to successfully connect your ESP32 to Elipse E3 for monitoring projects. In this project, I used two sensors connected to the ESP, along with an active buzzer for an audible signal.
However, the project can be adapted in any way you desire to suit your needs.

Antes de começar tudo, você precisa configurar um driver modbus em seu ElipseE3, para isso você pode procurar um tutorial simples em vídeo, mas vou estar disponibilizando o arquivo de driver que utilizei para o projeto.

Para configurar o driver, siga as prints que estão aqui;

Na aba modbus, configure para ModBUS TCP:
![Image](https://github.com/user-attachments/assets/2b7503db-8bfd-4dd8-a2d0-4d8ba4244e75)

Em Operations, eu deixei no padrão do aplicativo, mas se quiser adicionar mais registradores, fique a vontade:
![Image](https://github.com/user-attachments/assets/9673d844-de63-4b58-b691-3b3f25ae5187)

Em Setup, você escolherá a camada física como Ethernet
![Image](https://github.com/user-attachments/assets/cb700893-52b5-43c6-b70b-cd2629c93cb1)

E em Ethernet você configurará dessa forma, a porta 502 é a porta padrão do ElipseE3, o IP você irá modificar conforme o que seu código lhe fornecer quando a internet for conectada.
![Image](https://github.com/user-attachments/assets/f52e6fb3-4c8c-4970-b321-400c8caa5a4f)

Na aba do driver, você irá configurar as variáveis que sua ESP32 irá ler e converter para modbus para enviar ao ElipseE3, no caso deste projeto apenas temperatura e co2 foram configurados.
![Image](https://github.com/user-attachments/assets/18988c85-0277-4376-b51a-06ef650a4204)

Aqui está uma configuração de escala
![Image](https://github.com/user-attachments/assets/b5b0c498-b202-43ef-b4dc-3a3c72f380fc)

Após todos esses passos, inicie seu código no VSCODE, utilizando o PlatformIO e copie e cole seu IP na aba Ethernet do driver. Após isso, dê um build e run no seu código.

Com o código "rodando", abre o ElipseE3 e inicie o driver no ícone de play verde na aba Driver, se tudo estiver funcionando os nomes das variávies ficaram AZUIS.
Ao rodar seu sistema supervisório, o programa vai pedir para desativar os drivers e você clicará em SIM, manter os drivers ativos com o ElipseE3 rodando causará instabilidade na aquisição de dados via MODBUS.
//RodBCL
