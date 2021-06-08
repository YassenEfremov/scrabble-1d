#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//  Funkciq za generirane na bykwi za edin round i printiraneto im
extern void letter_generation(int letters){
    int random_letter;
    int array[letters + 1];
    
    srand(time(0)); // generira mi random chislo, ot koeto zavisqt random chislata
    
    for(int i=0; i < letters; i++){
        // izpolzvam formula za generiraneto na slychaina bykwa -> (rand() % (upper - lower + 1)) + lower;
        random_letter = (rand() % (122 - 97 + 1)) + 97; 
        array[i] = random_letter;
        printf("%c  | ", array[i]);
    }
}

extern void startGame(int letters, int rounds) {
    letter_generation(letters);
    
    /*
    1. Във всеки рунд се генерират няколко на брой букви и се изписват на екрана.
    2. Потребителят въвежда дума, която се състои само от предоставените букви.
        - При въвеждане на невалидна дума (която съдържа други букви или повече от подадения брой от дадена буква) се
        извежда съобщение за грешка и се преминава към стъпка 2.
        - При въвеждане на валидна дума се изчисляват точките ѝ (по подразбиране всяка буква е 1 точка) и се прибавят към
        общите точки на потребителя.
    3. Докато не свършат рундовете се връщаме към точка 2.
    4. При приключване на последния рунд се изписва крайния резултат на играча и се връща в основното меню.

    */
}
