## Сборка

```bash

g++ -std=c++17 -I./include src/main.cpp src/core/Card.cpp src/core/Deck.cpp src/game/HandEvaluator.cpp src/game/PokerGame.cpp src/players/BotPlayer.cpp src/players/HumanPlayer.cpp src/ui/TerminalView.cpp -o PokerTrainer
```

## Схема классов
https://miro.com/app/board/uXjVGXffgJ0=/?share_link_id=160304482530 - интерфейсы

https://miro.com/app/board/uXjVGXe4Eag=/?share_link_id=31171774876 - классы

## Функционал

- Выбор уровня сложности и количества ботов
- Расчет вероятности победы для игрока
- Отображение аутов (карты, улучшающие руку)
- Показ карт оппонентов (опционально, по завершении раздачи)
- Настройка начального баланса и размера блайндов


## Поведени ботов

Расчет шансов в `BotPlayer::evaluateHandStrength`:

### сложность 1-3

Fold при силе руки < 0.3, Check (30%), Call (30%). Не учитывает силу руки детально.

### сложность 4-6

Fold при силе руки < 0.4 и pot odds < 0.3, Raise (30% вероятность) при силе руки > 0.7, иначе Call, Call при pot odds > 0.5, иначе Check

### сложность 7-10

При силе руки > 0.8: Raise (вероятность 0.6) или Call
Fold при силе руки < 0.3 и pot odds < 0.25, Call при pot odds > 0.4, иначе Check
- Вероятность блефа = 0.3 при силе руки > 0.4
