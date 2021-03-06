#include <iostream>
#include <sstream>
#include <windows.h>

//COLORS
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define AQUA 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_AQUA 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15

//FIND TYPES
#define ALL 0
#define NUMS 1
#define CHARS 2
#define EMPTY 3

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void color(int foreground, int background)
{
    int color = (background * 16) + foreground;
    SetConsoleTextAttribute(hConsole, color);
}

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(hConsole, c);
}

char getCursorChar()
{
    char c = '\0';
    CONSOLE_SCREEN_BUFFER_INFO con;
    if (hConsole != INVALID_HANDLE_VALUE &&
        GetConsoleScreenBufferInfo(hConsole, &con))
    {
        DWORD read = 0;
        if (!ReadConsoleOutputCharacterA(hConsole, &c, 1,
            con.dwCursorPosition, &read) || read != 1
            )
            c = '\0';
    }
    return c;
}

char readChar(int x, int y)
{
    gotoxy(x, y);
    char c = getCursorChar();
    return c;
}

void vanishCursor()
{
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &info);
}

int priority(char c)
{
	switch (c)
	{
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	}

	return 0;
}

bool checkChar(char c, int type)
{
    char chars[] = {'+', '-', '*', '/', '(', ')', '='};
    int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

    switch(type)
    {
    case ALL:
        for(int i = 0; i < 7; i++)
        {
            if(c == chars[i]) return true;
        }
        for(int i = 0; i < 10; i++)
        {
            if(c - '0' == nums[i]) return true;
        }
        break;
    case NUMS:
        for(int i = 0; i < 7; i++)
        {
            if(c == chars[i]) return true;
        }
        break;
    case CHARS:
        for(int i = 0; i < 10; i++)
        {
            if(c - '0' == nums[i]) return true;
        }
        break;
    case EMPTY:
        if(c == ' ' || c == '\0') return true;
        break;
    default:
        return false;
        break;
    }

    return false;
}

void heapPointer(int y)
{
    char t = '\0';
    
    if(y > 0)
    {
        color(GREEN, GREEN);
        gotoxy(1, 3 + y);
        std::cout << " ";
        gotoxy(13, 3 + y);
        std::cout << " ";
        color(RED, BLACK);
        gotoxy(2, 3 + y);
        for(int i = 0; i <= 10; i++)
        {
            if(checkChar(readChar(2 + i, 3 + y), EMPTY))
            {
                t = readChar(2 + i, 3 + y);
                std::cout << t;
            }
            else std::cout << " ";
        }
    }

    if(y < 27)
    {
        color(GREEN, GREEN);
        gotoxy(1, 5 + y);
        std::cout << " ";
        gotoxy(13, 5 + y);
        std::cout << " ";
        color(RED, BLACK);
        gotoxy(2, 5 + y);
        for(int i = 0; i <= 10; i++)
        {
            if(checkChar(readChar(2 + i, 5 + y), EMPTY))
            {
                t = readChar(2 + i, 5 + y);
                std::cout << t;
            }
            else std::cout << " ";
        }
    }

    color(RED, PURPLE);
    gotoxy(1, 4 + y);
    std::cout << ">";
    gotoxy(13, 4 + y);
    std::cout << "<";

    color(YELLOW, LIGHT_PURPLE);
    gotoxy(2, 4 + y);
    for(int i = 0; i <= 10; i++) std::cout << " ";
}

void displayNumBuilder(int n)
{
    int len;
    int i = n;

    for(len = 0; i > 0; len++)
    {
        i /= 10;
    }

    gotoxy(2, 2);
    color(RED, BLACK);
    std::cout << n;
    for(int j = 0; j < (10 - len); j++)
    {
        std::cout << " ";
    } 
    color(WHITE, BLACK);
}

std::string stringToONP(std::string s, int speed = 500)
{
    heapPointer(0);
	std::string out = "";
	int len = s.length();
    int numBuild = 0;
    int numBuildCount = 1;
	char tmp;

	char heap[100] {};
	for (int i = 0; i < 100; i++) heap[i] = '\0';
	int counter = 0;
    char prev = '\0';

	for (int i = 0; i < len; i++)
	{
        color(WHITE, BLACK);
		gotoxy(50 + i, 2);
        std::cout << " ^";
		tmp = s.c_str()[i];
		switch (tmp)
		{
		case '=':
            numBuildCount = 1;
            out += std::to_string(numBuild);
            numBuild = 0;
            out += ";";
            displayNumBuilder(0);
			for (; counter > 0; counter--)
			{
				out += heap[counter - 1];
                gotoxy(2, 3 + counter);
                color(YELLOW, LIGHT_PURPLE);
                std::cout << " ";
                heapPointer(counter);
			}
            Sleep(500);
            color(RED, BLACK);
            gotoxy(50 + len, 2);
            std::cout << " ";
			color(WHITE, BLACK);
			gotoxy(42, 3);
			std::cout << out;
            heapPointer(counter);
            Sleep(1000);
			return out;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
            numBuild += (numBuildCount * (tmp - '0'));
            numBuildCount *= 10;
            displayNumBuilder(numBuild);
			break;
		case '+':
		case '-':
		case '*':
		case '/':
            numBuildCount = 1;
            if(prev == ')') prev = '\0';
            else {
                out += std::to_string(numBuild);

                numBuild = 0;
                out += ";";
                displayNumBuilder(0);
            }

			while (counter)
			{
				if (priority(tmp) > priority(heap[counter - 1])) break;
				out += heap[--counter];
                heapPointer(counter);
				heap[counter] = '\0';
                gotoxy(2, 3 + counter);
                color(YELLOW, LIGHT_PURPLE);
                if(counter != 0) std::cout << " ";
                color(WHITE, BLACK);
			}
			heap[counter] = tmp;
            heapPointer(counter + 1);
            gotoxy(2, 4 + counter);
            color(YELLOW, LIGHT_PURPLE);
            std::cout << tmp;
			counter++;
            color(WHITE, BLACK);
			break;
		case '(':
			heap[counter] = tmp;
            heapPointer(counter + 1);
            gotoxy(2, 4 + counter);
            color(YELLOW, LIGHT_PURPLE);
            std::cout << tmp;
			counter++;
            color(WHITE, BLACK);
			break;
		case ')':
            prev = ')';
			            numBuildCount = 1;
            out += std::to_string(numBuild);
            numBuild = 0;
            out += ";";
            displayNumBuilder(0);

			while (counter)
			{
                if(heap[counter - 1] == '(') break;
				if (priority(tmp) > priority(heap[counter - 1])) break;
				out += heap[--counter];
                heapPointer(counter);
				heap[counter] = '\0';
                gotoxy(2, 3 + counter);
                color(YELLOW, LIGHT_PURPLE);
                if(counter != 0) std::cout << " ";
                color(WHITE, BLACK);
			}
			heap[counter] = tmp;
            heapPointer(counter + 1);
            gotoxy(2, 4 + counter);
            color(YELLOW, LIGHT_PURPLE);
            std::cout << tmp;
			counter++;
            color(WHITE, BLACK);
			for (; heap[counter] != '('; counter--)
			{
                heapPointer(counter);
				if (priority(heap[counter]) > priority(heap[counter - 1]))
				{
					out += heap[counter];
					heap[counter] = '\0';
                    gotoxy(2, 4 + counter);
                    color(YELLOW, LIGHT_PURPLE);
					std::cout << " ";
				}
                color(WHITE, BLACK);
			}

			heap[counter] = '\0';
            heapPointer(counter + 1);
            gotoxy(2, 4 + counter);
            color(YELLOW, LIGHT_PURPLE);
            std::cout << " ";
            color(WHITE, BLACK);
			break;
		case ' ':
			break;
		}
		gotoxy(42, 3);
		std::cout << out;
		Sleep(speed);
	}

    return out;
}

int calculateONP(std::string s, int speed = 500)
{
    heapPointer(0);
	int out = 0;
    int numBuild = 0;
    int numBuildCount = 1;
	int len = s.length();
	char tmp;
	int counter = 0;
	int calculate = 0;
    int digit = 0;
    char prev = '\0';

	int heap[100]{};
	for (int i = 0; i < 100; i++) heap[i] = 0;

	for (int i = 0; i < len; i++)
	{
        color(LIGHT_GREEN, BLACK);
		gotoxy(41 + i, 4);
        std::cout << " ^";
		tmp = s.c_str()[i];
		switch (tmp)
		{
        case ';':
            heap[counter] = numBuild;
            gotoxy(42 + i, 3);
            color(LIGHT_GREEN, BLACK);
            gotoxy(41 + i, 4);
            std::cout << " ^";
			color(WHITE, BLACK);
			counter++;
            heapPointer(counter);

            gotoxy(2, 3 + counter);
            color(YELLOW, BLACK);
			std::cout << numBuild;

            numBuildCount = 1;
            numBuild = 0;
            displayNumBuilder(0);
            gotoxy(42 + i, 3);
            color(LIGHT_GREEN, BLACK);
            std::cout << tmp;
            gotoxy(41 + i, 4);
            std::cout << " ^";
			color(WHITE, BLACK);
            break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
            numBuild += (numBuildCount * (tmp - '0'));
            numBuildCount *= 10;
            displayNumBuilder(numBuild);
            gotoxy(42 + i, 3);
            color(LIGHT_GREEN, BLACK);
            std::cout << tmp;
            gotoxy(41 + i, 4);
            std::cout << " ^";
			color(WHITE, BLACK);
			break;
		case '+':
			calculate = heap[counter - 2] + heap[counter - 1];
			heap[counter - 2] = 0;
			heap[counter - 1] = 0;
			gotoxy(2, 4 + counter - 2);
            color(YELLOW, BLACK);
			std::cout << "  ";
			gotoxy(2, 4 + counter - 1);
            std::cout << "  ";
			counter -= 2;
            heapPointer(counter);
			heap[counter++] = calculate;
            heapPointer(counter);
            color(YELLOW, BLACK);
			gotoxy(2, 4 + counter - 1);
            std::cout << calculate;
            gotoxy(42 + i, 3);
            color(LIGHT_GREEN, BLACK);
            std::cout << tmp;
            gotoxy(41 + i, 4);
            std::cout << " ^";
			color(WHITE, BLACK);
			break;
		case '-':
            calculate = heap[counter - 2] - heap[counter - 1];
			heap[counter - 2] = 0;
			heap[counter - 1] = 0;
			gotoxy(2, 4 + counter - 2);
            color(YELLOW, BLACK);
			std::cout << "  ";
			gotoxy(2, 4 + counter - 1);
            std::cout << "  ";
			counter -= 2;
            heapPointer(counter);
			heap[counter++] = calculate;
            heapPointer(counter);
            color(YELLOW, BLACK);
			gotoxy(2, 4 + counter - 1);
            std::cout << calculate;
            gotoxy(42 + i, 3);
            color(LIGHT_GREEN, BLACK);
            std::cout << tmp;
            gotoxy(41 + i, 4);
            std::cout << " ^";
			color(WHITE, BLACK);
			break;
		case '*':
            calculate = heap[counter - 2] * heap[counter - 1];
			heap[counter - 2] = 0;
			heap[counter - 1] = 0;
			gotoxy(2, 4 + counter - 2);
            color(YELLOW, BLACK);
			std::cout << "  ";
			gotoxy(2, 4 + counter - 1);
            std::cout << "  ";
			counter -= 2;
            heapPointer(counter);
			heap[counter++] = calculate;
            heapPointer(counter);
            color(YELLOW, BLACK);
			gotoxy(2, 4 + counter - 1);
            std::cout << calculate;
            gotoxy(42 + i, 3);
            color(LIGHT_GREEN, BLACK);
            std::cout << tmp;
            gotoxy(41 + i, 4);
            std::cout << " ^";
			color(WHITE, BLACK);
			break;
		case '/':
			if (heap[counter - 1] == 0)
			{
                color(RED, BLACK);
                gotoxy(43 + len, 3);
                std::cout << "(failure)";
                color(LIGHT_RED, BLACK);
                gotoxy(42 + i, 3);
                std::cout << tmp;
                gotoxy(41 + i, 4);
                std::cout << " ^";
                gotoxy(15, 5);
                color(RED, BLACK);
                std::cout << "Invalid operation at position ";
                color(LIGHT_RED, BLACK);
                std::cout << i + 1;
                color(RED, BLACK);
                std::cout << " (division by zero)";   
				color(WHITE, BLACK);
				return 0;
			}
            calculate = heap[counter - 2] / heap[counter - 1];
			heap[counter - 2] = 0;
			heap[counter - 1] = 0;
			gotoxy(2, 4 + counter - 2);
            color(YELLOW, BLACK);
			std::cout << "  ";
			gotoxy(2, 4 + counter - 1);
            std::cout << "  ";
			counter -= 2;
            heapPointer(counter);
			heap[counter++] = calculate;
            heapPointer(counter);
            color(YELLOW, BLACK);
			gotoxy(2, 4 + counter - 1);
            std::cout << calculate;
            gotoxy(42 + i, 3);
            color(LIGHT_GREEN, BLACK);
            std::cout << tmp;
            gotoxy(41 + i, 4);
            std::cout << " ^";
			color(WHITE, BLACK);
			break;
		}

		Sleep(speed);
	}
	gotoxy(15, 5);
	std::cout << "RESULT: " << heap[counter - 1];
	return heap[counter - 1];
}

bool validate(std::string s, int speed = 250)
{
    int len = s.length();

    if(len < 2)
    {
        gotoxy(51, 1);
        color(LIGHT_RED, BLACK);
        std::cout << s;
        gotoxy(52 + len, 1);
        color(RED, BLACK);
        std::cout << "(failure)    ";
        gotoxy(15, 3);
        color(RED, BLACK);
        std::cout << "Invalid formula (too short)";     
        color(WHITE, BLACK);
        return false;
    }

    char tmp;
    int brackets = 0;

    for (int i = 0; i < len; i++)
	{
		tmp = s.c_str()[i];
		switch (tmp)
        {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
        case '=':
            gotoxy(51 + i, 1);
            color(LIGHT_GREEN, BLACK);
            std::cout << tmp;
            gotoxy(50 + i, 2);
            std::cout << " ^";
            break;
		case '+':
		case '-':
		case '*':
		case '/':
            if(s.c_str()[i - 1] == '+' ||
                s.c_str()[i - 1] == '-' ||
                s.c_str()[i - 1] == '*' ||
                s.c_str()[i - 1] == '/')
            {
                gotoxy(52 + len, 1);
                color(RED, BLACK);
                std::cout << "(failure)    ";
                gotoxy(50 + i, 2);
                color(LIGHT_RED, BLACK);
                std::cout << " ^";
                gotoxy(51 + i, 1);
                std::cout << tmp;
                gotoxy(15, 3);
                color(RED, BLACK);
                std::cout << "Invalid character '";
                color(LIGHT_RED, BLACK);
                std::cout << tmp;
                color(RED, BLACK);
                std::cout << "' at position ";
                color(LIGHT_RED, BLACK);
                std::cout << i + 1;
                color(RED, BLACK);
                std::cout << " (cannot use operator as element)";     
                color(WHITE, BLACK);
                return false;
            }
            gotoxy(51 + i, 1);
            color(LIGHT_GREEN, BLACK);
            std::cout << tmp;
            gotoxy(50 + i, 2);
            std::cout << " ^";
            break;
        case '(':
            gotoxy(51 + i, 1);
            color(LIGHT_GREEN, BLACK);
            std::cout << tmp;
            gotoxy(50 + i, 2);
            std::cout << " ^";
            brackets++;
            break;
        case ')':
            gotoxy(51 + i, 1);
            color(LIGHT_GREEN, BLACK);
            std::cout << tmp;
            gotoxy(50 + i, 2);
            std::cout << " ^";
            brackets--;
            if(brackets < 0)
            {
                gotoxy(52 + len, 1);
                color(RED, BLACK);
                std::cout << "(failure)    ";
                gotoxy(50 + i, 2);
                color(LIGHT_RED, BLACK);
                std::cout << " ^";
                gotoxy(51 + i, 1);
                std::cout << tmp;
                gotoxy(15, 3);
                color(RED, BLACK);
                std::cout << "Invalid character '";
                color(LIGHT_RED, BLACK);
                std::cout << tmp;
                color(RED, BLACK);
                std::cout << "' at position ";
                color(LIGHT_RED, BLACK);
                std::cout << i + 1;
                color(RED, BLACK);
                std::cout << " (brackets do not match)";     
                color(WHITE, BLACK);
                return false;
            }
            break;
        default:
            gotoxy(52 + len, 1);
            color(RED, BLACK);
            std::cout << "(failure)    ";
            gotoxy(50 + i, 2);
            color(LIGHT_RED, BLACK);
            std::cout << " ^";
            gotoxy(51 + i, 1);
            std::cout << tmp;
            gotoxy(15, 3);
            color(RED, BLACK);
            std::cout << "Invalid character '";
            color(LIGHT_RED, BLACK);
            std::cout << tmp;
            color(RED, BLACK);
            std::cout << "' at position ";
            color(LIGHT_RED, BLACK);
            std::cout << i + 1;
            color(RED, BLACK);
            std::cout << " (not present in charset)";            
            color(WHITE, BLACK);
            return false;
        }
        Sleep(speed);
    }

    if(s.c_str()[len - 1] != '=')
    {
            gotoxy(52 + len, 1);
            color(RED, BLACK);
            std::cout << "(failure)    ";
            gotoxy(49 + len, 2);
            color(LIGHT_RED, BLACK);
            std::cout << " ^";
            gotoxy(50 + len, 1);
            std::cout << s.c_str()[len - 1];
            gotoxy(15, 3);
            color(RED, BLACK);
            std::cout << "Invalid character '";
            color(LIGHT_RED, BLACK);
            std::cout << s.c_str()[len - 1];
            color(RED, BLACK);
            std::cout << "' at position ";
            color(LIGHT_RED, BLACK);
            std::cout << len;
            color(RED, BLACK);
            std::cout << " (missing equation mark '=')";            
            color(WHITE, BLACK);
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    std::stringstream tmp1, tmp2, tmp3;
    int sp1, sp2, sp3;
    if(argc >= 4) {
        tmp1 << argv[2];
        tmp2 << argv[3];
        tmp3 << argv[4];
        tmp1 >> sp1;
        tmp2 >> sp2;
        tmp3 >> sp3;
    } else {
        sp1 = 100;
        sp2 = 100;
        sp3 = 100;
    }

	system("cls");

    color(GREEN, GREEN);
    for(int i = 1; i <= 28; i++)
    {
        gotoxy(1, i);
        std::cout << "#";
        gotoxy(13, i);
        std::cout << "#";
    }

    for(int i = 2; i <= 12; i++)
    {
        gotoxy(i, 1);
        std::cout << "#";
        gotoxy(i, 3);
        std::cout << "#";
        gotoxy(i, 28);
        std::cout << "#";
    }

    color(RED, GREEN);
    gotoxy(2, 3);
    std::cout << "MEMORY HEAP";
    gotoxy(2, 1);
    std::cout << "NUM BUILDER";

    color(WHITE, BLACK);

	gotoxy(15, 1);
	std::cout << "EQUATION PROVIDED IN STANDARD FORM: ";
    
    std::string equation = "";
    if(argv[1])
    {
        equation = argv[1];
        std::cout << equation;
    } 
    else std::cin >> equation;
    vanishCursor();
    int len = equation.length();

    gotoxy(52 + len, 1);
    std::cout << "(checking...)";

    if(!validate(equation, sp1))
    {
        system("pause > NUL");
        return 0;
    }

    gotoxy(50 + len, 2);
    std::cout << " ";
    gotoxy(52 + len, 1);
    color(GREEN, BLACK);
    std::cout << "(success)    ";
    Sleep(1000);

    color(WHITE, BLACK);
    gotoxy(15, 3);
    std::cout << "EQUATION CONVERTED TO ONP: ";

	std::string a = stringToONP(equation, sp2);
	int b = calculateONP(a, sp3);

    system("pause > NUL");
    return 0;
}