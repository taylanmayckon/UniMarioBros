# UniMarioBros

# Estrutura do código
```
📁 UniMarioBros/
│
├── 📁 assets/                # Contém todos os recursos visuais e sonoros do jogo.
│   ├── 📁 audio/             # Efeitos sonoros.
│   │   └── 🔊 pulo.mp3
│   └── 📁 textures/          # Sprites, texturas e imagens (PNG, JPG).
│       └── 🖼️ mario.png
│
├── 📁 external/              # Bibliotecas e dependências de terceiros.
│   └── 📦 raylib-5.5/        # Cópia local da biblioteca Raylib.
│
├── 📁 lib/                   # Bibliotecas e módulos próprios desenvolvidos pela equipe do projeto.
│   └── 📦 camera/            
│   └── 📦 coin/            
│   └── 📦 collisions/      
│   └── 📦 inimigos/        
│   └── 📦 mario/            
│   └── 📦 mario_animdb/     
│   └── 📦 menu/             
│   └── 📦 platform/         
│   └── 📦 scene/             
│
├── 📁 logs/                  # Arquivos de log gerados durante a execução para depuração.
│   └── 📝 log-23-05-2025.txt
│
├── 📁 saves/                 # Arquivos binários com o progresso salvo do jogo.
│   └── 💾 save-23-05-2025.bin
│
├── 📄 .gitignore             # Arquivos e diretórios a serem ignorados pelo Git.
├── 📄 CMakeLists.txt         # Script de configuração para compilação com CMake.
├── 📄 main.c                 # Ponto de entrada do jogo.
└── 📄 README.md              # Documentação do projeto.
```

# Como compilar
Execute no Prompt Comando:
```sh
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```
Nota: O primeiro comando só precisa ser executado uma vez, após isso para compilar novamente o programa só o segundo precisa ser executado.

# Depois de compilar
O CMakeLists.txt está configurado para gerar o arquivo UniMarioBros.exe na raiz do diretório, então é só executar ou escrever no Terminal:
```sh
UniMarioBros.exe
```
