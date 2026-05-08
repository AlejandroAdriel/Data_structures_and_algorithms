/******************************************************************************

AHORCADOS

- vidas cada error baja 1, hay 15 vidas
- 10 palabras de longitudes diferentes en una estructura de datos
- al azar en cada juego se escoje una de las 10 palabras
- se pintan en un lado las palabras usadas
- hay puntaje
- existe un boton de ctrl z: retrocede todo ( la letra usada, puntaje, vida ( si se perdio ))
- el ctrl z memoriza 10 estados osea maximo retrocedes 10 veces

    _ _ _ _ _


*******************************************************************************/
#include <iostream>
#include <random>

struct ESTADO
{
    int Vidas;
    int Puntaje;
    char LetrasUsadas[28];
    char Progreso[31];

    ESTADO *next;
    ESTADO *prev;

    ESTADO()
    {
        Vidas = 15;
        Puntaje = 0;
        next = prev = nullptr;
        LetrasUsadas[0] = '\0';
        Progreso[0] = '\0';
    }

    ESTADO(const ESTADO &antiguo_estado)
    {
        Vidas = antiguo_estado.Vidas;
        Puntaje = antiguo_estado.Puntaje;

        const char *p = antiguo_estado.LetrasUsadas;
        char *q = LetrasUsadas;

        while (*p != '\0')
        {
            *q = *p;
            p++;
            q++;
        }
        *q = '\0';

        const char *a = antiguo_estado.Progreso;
        char *b = Progreso;

        while (*a != '\0')
        {
            *b = *a;
            a++;
            b++;
        }
        *b = '\0';

        next = prev = nullptr;
    }
};

struct AHORCADOS
{
    char Palabras[10][31] = {"GATO", "PANDA", "EFERVESCENTE", "ALMOHADA", "DESOXIRRIBONUCLEICO",
                             "ESTERNOCLEIDOMASTOIDEO", "WHISKY", "ELIXIR", "NAUFRAGIO", "VAMPIRO"};
    char *Palabra;
    int NEstados;

    ESTADO *HEAD;
    ESTADO *TAIL;

    AHORCADOS()
    {
        std::random_device rd;
        std::mt19937 generador(rd());
        std::uniform_int_distribution<int> distribucion(0, 9);
        int indice_aleatorio = distribucion(generador);

        Palabra = *(Palabras + indice_aleatorio);
        NEstados = 0;
        HEAD = TAIL = nullptr;
    }

    ~AHORCADOS()
    {
        while (NEstados > 0)
        {
            RETOCEDER_ESTADO();
        }
    }

    void JUGAR();
    void DIBUJAR();
    bool NUEVO_ESTADO(char letra);
    bool RETOCEDER_ESTADO();
    bool LIBERAR_ESPACIO();
};

void AHORCADOS::JUGAR()
{
    bool flag = true;
    char letra = '\0';
    NUEVO_ESTADO(letra);

    while (flag)
    {
        std::cout << "\x1B[2J\x1B[H"; // limpiar

        DIBUJAR();

        std::cin >> letra;

        if (letra == '-')
        {
            RETOCEDER_ESTADO();
        }

        else
        {
            NUEVO_ESTADO(letra);
        }

        if (TAIL->Vidas <= 0 || letra == '*')
        {
            flag = false;
        }

        bool win = true;
        char *m = Palabra;
        char *n = TAIL->Progreso;

        while (*m != '\0')
        {
            if (*m != *n)
            {
                win = false;
                break;
            }
            m++;
            n++;
        }

        if (win)
        {
            std::cout << "\x1B[2J\x1B[H\n";
            std::cout << "\033[1;32m";
            std::cout << "           .__________.\n";
            std::cout << "           | /        |\n";
            std::cout << "           |/         |   \n";
            std::cout << "           |              \n";
            std::cout << "           |        \\O/   \n";
            std::cout << "           |         |    \n";
            std::cout << "          _|_       / \\  _|_ \n";
            std::cout << "         |___|     |___||___|\n\n";

            std::cout << "  █░█ █ █▀▀ ▀█▀ █▀█ █▀█ █ ▄▀█ █   █ █\n";
            std::cout << "  ▀▄▀ █ █▄▄ ░█░ █▄█ █▀▄ █ █▀█ ▄   ▀▄▀\n\n";

            std::cout << "\033[1;37m";
            std::cout << "     >> SCORE:   " << TAIL->Puntaje << " PTS\n";
            std::cout << "     >> PALABRA: " << Palabra << "\n";
            std::cout << "  ════════════════════════════════════════\n";
            std::cout << "\033[0m" << std::endl;

            return;
        }
    }

    std::cout << "\x1B[2J\x1B[H\n";

    std::cout << "\033[1;31m";
    std::cout << "           .__________.\n";
    std::cout << "           | /        |\n";
    std::cout << "           |/        ( )\n";
    std::cout << "           |         /|\\\n";
    std::cout << "           |         / \\\n";
    std::cout << "           |           \n";
    std::cout << "          _|_       _|_ \n";
    std::cout << "         |___|     |___|\n\n";

    std::cout << "  █▀▀ ▄▀█ █▀▄▀█ █▀▀   █▀█ █ █ █▀▀ █▀█\n";
    std::cout << "  █▄█ █▀█ █ ▀ █ █▄▄   █▄█ ▀▄▀ █▄▄ █▀▄\n\n";

    std::cout << "\033[1;37m";
    std::cout << "     >> SCORE:   " << TAIL->Puntaje << " PTS\n";
    std::cout << "     >> PALABRA: " << Palabra << "\n";
    std::cout << "  ════════════════════════════════════════\n";
    std::cout << "\033[0m" << std::endl;

    return;
}

void AHORCADOS::DIBUJAR()
{
    std::cout << std::endl;
    std::cout << "  VIDAS: " << TAIL->Vidas << std::endl;
    std::cout << "\n  PUNTAJE: " << TAIL->Puntaje << std::endl;
    std::cout << "\n  LETRAS UTILIZADAS: ";

    char *p = TAIL->LetrasUsadas;
    while (*p != '\0')
    {
        std::cout << *p << " ";
        p++;
    }

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "   ";
    char *q = TAIL->Progreso;
    while (*q != '\0')
    {
        std::cout << *q << " ";
        q++;
    }

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    return;
}

bool AHORCADOS::NUEVO_ESTADO(char letra)
{
    if (!HEAD)
    {
        ESTADO *p = new ESTADO;
        HEAD = TAIL = p;

        char *m = Palabra;
        char *n = TAIL->Progreso;
        while (*m != '\0')
        {
            *n = '_';
            n++;
            m++;
        }
        *n = '\0';
    }
    else
    {
        if (letra >= 'a' && letra <= 'z')
            letra -= 32;

        ESTADO *p = new ESTADO(*TAIL);
        TAIL->next = p;
        p->prev = TAIL;
        TAIL = p;

        char *a = TAIL->LetrasUsadas;

        while (*a != '\0')
            a++;

        *a = letra;
        *(a + 1) = '\0';

        char *c = Palabra;
        char *d = TAIL->Progreso;
        bool acierto = false;

        while (*c != '\0')
        {
            if (letra == *c)
            {
                *d = letra;
                TAIL->Puntaje += 1000;
                acierto = true;
            }
            c++;
            d++;
        }

        if (acierto == false)
            TAIL->Vidas--;
    }

    NEstados++;

    if (NEstados > 11 && NEstados > 1)
        LIBERAR_ESPACIO();

    return true;
}

bool AHORCADOS::RETOCEDER_ESTADO()
{
    ESTADO *p = TAIL;
    TAIL = TAIL->prev;
    delete p;
    NEstados--;
    return true;
}

bool AHORCADOS::LIBERAR_ESPACIO()
{
    ESTADO *p = HEAD;
    HEAD = HEAD->next;
    delete p;
    NEstados--;
    return true;
}

int main()
{

    AHORCADOS juego1;
    juego1.JUGAR();

    return 0;
}