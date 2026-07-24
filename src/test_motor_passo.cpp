#define STEP_PIN 18
#define DIR_PIN  19

void setup()
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
}

void loop()
{
    // Sentido horário
    digitalWrite(DIR_PIN, HIGH);

    for(int i=0;i<200;i++)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(800);

        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(800);
    }

    delay(1000);

    // Sentido anti-horário
    digitalWrite(DIR_PIN, LOW);

    for(int i=0;i<200;i++)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(800);

        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(800);
    }

    delay(1000);
}
