#include <iostream>
#include <windows.h>
#include <vector>
#include <cstring>
#include <fstream>

int charFrom, charTo, alpha;

bool is_alpha(char c);

void only_lower(std::vector<std::string> & str);

void encrypt(std::vector<std::string> & str, int k);

void decrypt(std::vector<std::string> & str, int k);

void hack(std::vector<std::string> str);

int main()
{
    setlocale(LC_ALL, "RUS");

    char language[3];
    do
    {
        std::cout << "Выбирите язык:\nru - русский\nen - английский\n-";
        std::cin.get(language, 3);
        std::cin.get();
    } while (stricmp(language, "ru") && stricmp(language, "en"));

    if (!stricmp(language, "ru"))
    {
        charFrom = -32; charTo = -1;
        alpha = 31;
    }
    else
    {
        charFrom = 97; charTo = 122;
        alpha = 26;
    }

    bool exit = false;
    while (!exit)
    {
        std::ofstream f_out("string.txt", std::ios_base::out | std::ios_base::trunc);
        if (!f_out)
        {
            perror("File not create");
            return 1;
        }
        f_out << "Введите здесь строку, сохраните файл и выберите необходимую опцию";
        f_out.close();
        system("string.txt");

        std::vector<std::string> str;

        std::ifstream f_in("string.txt", std::ios_base::in);
        if (!f_in)
        {
            perror("File not opened");
            return 1;
        }
        std::string cat_str;
        while (!f_in.eof())
        {
            std::string f_str;
            f_in >> f_str;
            cat_str += f_str;
            cat_str += ' ';
            f_str.clear();
        }
        f_in.close();

        str.push_back(cat_str);
        cat_str.clear();
        only_lower(str);

        char option[8];
        while (true)
        {
            std::cout << "Выберите опцию:\nencrypt - зашифровать\ndecrypt - расшифровать\nhack - взломать\nexit - выход\n-";
            std::cin.get(option, 8);
            std::cin.get();
            if (!stricmp(option, "encrypt"))
                break;
            else if (!stricmp(option, "decrypt"))
                break;
            else if (!stricmp(option, "hack"))
                break;
            else if (!stricmp(option, "exit"))
                break;
            else
                std::cout << "Неверно введена опция\n\n";
        }

        int bias;

        if (!stricmp(option, "encrypt"))
        {
            std::cout << "Введите смещение: ";
            std::cin >> bias;
            std::cin.get();
            encrypt(str, bias);
            std::ofstream f_enc("StringCode.txt", std::ios_base::out | std::ios_base::trunc);
            if (!f_enc)
            {
                perror("File not opened");
                return 1;
            }
            for (int i = 0; i < str.size(); i++)
            {
                f_enc << str[i];
                f_enc << '\n';
            }
            f_enc.close();
            system("StringCode.txt");
        }
        else if (!stricmp(option, "decrypt"))
        {
            std::cout << "Введите смещение: ";
            std::cin >> bias;
            std::cin.get();
            decrypt(str, bias);
            std::ofstream f_dec("StringDecode.txt", std::ios_base::out | std::ios_base::trunc);
            if (!f_dec)
            {
                perror("File not opened");
                return 1;
            }
            for (int i = 0; i < str.size(); i++)
            {
                f_dec << str[i];
                f_dec << '\n';
            }
            f_dec.close();
            system("StringDecode.txt");
        }
        else if (!stricmp(option, "hack"))
        {
            hack(str);
            system("HackCode.txt");
        }
        else if (!stricmp(option, "exit"))
            exit = true;
        str.clear();
    }
    DeleteFile("string.txt");
    system("pause");
    return 0;
}

void encrypt(std::vector<std::string> & str, int k)
{
    std::vector<std::string> tmp_str;
    for (int i = 0; i < str.size(); i++)
    {
        std::string codeStr;
        for (int j = 0;  j < str[i].size(); j++)
        {
            if (!is_alpha(str[i][j]))
                codeStr.push_back(str[i][j]);
            else
            {
                int curr_char = str[i][j] + k;
                if (curr_char > charTo)
                {
                    codeStr.push_back((char) charFrom + (curr_char - charTo) - 1);
                }
                else if (curr_char < charFrom)
                {
                    codeStr.push_back((char) charTo - (charFrom - curr_char) + 1);
                }
                else
                {
                    codeStr.push_back((char)curr_char);
                }
            }
        }
        tmp_str.push_back(codeStr);
    }
    str.clear();
    for (int i = 0; i < tmp_str.size(); i++)
        str.push_back(tmp_str[i]);
    tmp_str.clear();
}

void decrypt(std::vector<std::string> & str, int k)
{
    std::vector<std::string> tmp_str;
    for (int i = 0; i < str.size(); i++)
    {
        std::string codeStr;
        for (int j = 0;  j < str[i].size(); j++)
        {
            if (!is_alpha(str[i][j]))
                codeStr.push_back(str[i][j]);
            else
            {
                int curr_char = str[i][j] - k;
                if (curr_char < charFrom)
                {
                    codeStr.push_back((char) charTo - (charFrom - curr_char) + 1);
                }
                else if (curr_char > charTo)
                {
                    codeStr.push_back((char) charFrom + (curr_char - charTo) - 1);
                }
                else
                {
                    codeStr.push_back((char)curr_char);
                }
            }
        }
        tmp_str.push_back(codeStr);
    }
    str.clear();
    for (int i = 0; i < tmp_str.size(); i++)
        str.push_back(tmp_str[i]);
    tmp_str.clear();
}

void hack(std::vector<std::string> str)
{
    std::vector<std::string> tmp_str;

    for (int l = 0; l < (alpha + 1); l++)
    {
        for (int i = 0; i < str.size(); i++)
        {
            std::string codeStr;
            for (int j = 0;  j < str[i].size(); j++)
            {
                if (!is_alpha(str[i][j]))
                    codeStr.push_back(str[i][j]);
                else
                {
                    int curr_char = str[i][j] - (l + 1);
                    if (curr_char < charFrom)
                    {
                        codeStr.push_back((char) charTo - (charFrom - curr_char) + 1);
                    }
                    else if (curr_char > charTo)
                    {
                        codeStr.push_back((char) charFrom + (curr_char - charTo) - 1);
                    }
                    else
                    {
                        codeStr.push_back((char)curr_char);
                    }
                }
            }
            tmp_str.push_back(codeStr);
            codeStr.clear();
        }
    }

    std::ofstream f_out("HackCode.txt", std::ios_base::out | std::ios_base::trunc);
    if (!f_out)
    {
        perror("File not opened");
        exit(1);
    }
    for (int i = 0; i < (alpha + 1); i++)
    {
        f_out << tmp_str[i];
        f_out << '\n';
    }
    f_out.close();
    tmp_str.clear();
}

void only_lower(std::vector<std::string> & str)
{
    std::vector<std::string> tmp_str;
    for (int i = 0; i < str.size(); i++)
    {
        std::string str_acc;
        for (int j = 0; j < str[i].size(); j++)
            if (isupper(str[i][j]))
                str_acc.push_back(tolower(str[i][j]));
            else
                str_acc.push_back(str[i][j]);
        tmp_str.push_back(str_acc);
    }
    str.clear();
    for (int i = 0; i < tmp_str.size(); i++)
        str.push_back(tmp_str[i]);
}

bool is_alpha(char c)
{
    if ((c >= 65 && c <= 90)  ||
        (c >= 97 && c <= 122) ||
        (c >= -64 && c <= -1))
        return true;
    else return false;
}
