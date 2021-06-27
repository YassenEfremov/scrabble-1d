# Версия на играта Scrabble

## Какви са правилата?
Играта протича по следния начин:

> 1. Във всеки рунд се генерират няколко на брой букви и се изписват на екрана.
> 2. Потребителят въвежда дума, която се състои само от предоставените букви.  
>    - При въвеждане на невалидна дума (която съдържа други букви или повече от подадения брой от дадена буква) се извежда съобщение за грешка и се преминава към стъпка **2**.  
>    - При въвеждане на валидна дума се изчисляват точките ѝ (по подразбиране всяка буква е 1 точка) и се прибавят към общите точки на потребителя.
> 3. Докато не свършат рундовете се връщаме към точка **2**.
> 4. При приключване на последния рунд се изписва крайния резултат на играча и се връща в основното меню.

## Кои думи са валидни?
Използваме зареден от файл речник с думи. Всяка дума, въведена от потребителя, се търси в този речник дали е валидна или не.

За проверката на това дали една дума е валидна в дадения речник използваме дърво [Trie](https://en.wikipedia.org/wiki/Trie).

## Как мога да играя?
**ВАЖНО:** Играта е разработвана главно на **Linux Mint**, затова най-вероятно не функционира на **Windows**.
- Първо изтеглете кода от **Github** чрез:
  - [този линк](https://github.com/YassenEfremov/elsys-10A-scrabble/archive/refs/heads/master.zip)
  - или чрез ```git clone https://github.com/YassenEfremov/elsys-10A-scrabble```
- Навигирайте през терминала до папката ```src/```.
- Пуснете файла **Makefile** като напишете ```make```.
- Стартирайте файла **main** като напишете ```./main```.
- Готово! Би трябвало да видите основното меню на играта!

## Подробности по разработването
### Разпределение на работата
* Логика на играта - *Данаил Божков*
* Четене на речник и генериране на дърво - *Ясен Ефремов*
* Добавяне на дума в речника, записване на дървото във файл, четене на дървото от файл - *Лъчезар Велинов*
* Валидиране на дума в дървото - *Лъчезар Лазаров*
